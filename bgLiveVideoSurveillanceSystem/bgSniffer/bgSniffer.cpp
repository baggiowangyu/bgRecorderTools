#include "stdafx.h"
#include "bgSniffer.h"
#include <shlwapi.h>
#include <tchar.h>
#include "pcap.h"

#include "bgNetworkProtocolStruct.h"

#define HAVE_REMOTE
#include "remote-ext.h"

bgSniffer::bgSniffer(bgSnifferNotifer *notifer)
: adapter_handle_(NULL)
, thread_exit_(true)
, notifer_(notifer)
, ethernetii_(new bgProtocolEthernetII(notifer))
{

}

bgSniffer::~bgSniffer()
{
	delete ethernetii_;
	ethernetii_ = NULL;
}

int bgSniffer::EnumAllNetworkDevices(int &count, PNETWORK_DEVICE_DESC device_descs)
{
	// 先枚举所有的设备
	pcap_if_t *alldevs = NULL;
	char errbuf[PCAP_ERRBUF_SIZE + 1] = {0};
	int errCode = pcap_findalldevs(&alldevs, errbuf);
	if (errCode < 0)
		return errCode;

	// 先遍历一遍，计算数量
	int device_count = 0;
	pcap_if_t *current_dev = alldevs;
	do 
	{
		++device_count;
		current_dev = current_dev->next;
	} while (current_dev->next != NULL);

	// 数量不足，反馈需要的数量
	if (count < device_count)
	{
		count = device_count;
		return -1;
	}

	current_dev = alldevs;
	PNETWORK_DEVICE_DESC device = device_descs;
	for (int index = 0; index < device_count; ++index)
	{
		device_descs[index].name_ = current_dev->name;

		// 转换为在系统中展示的名称
		// 查询注册表：HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Network\{4D36E972-E325-11CE-BFC1-08002BE10318}\[GUID]\Connection => Name
		{
			// 先从名称里面提取GUID，寻找下划线
			int pos = device_descs[index].name_.find("_");
			std::string guid = device_descs[index].name_.substr(pos + 1);
			
			// 组织注册表子路径
			std::string subpath = "SYSTEM\\CurrentControlSet\\Control\\Network\\{4D36E972-E325-11CE-BFC1-08002BE10318}\\";
			subpath += guid;
			subpath += "\\Connection";

			DWORD val_type = 0;
			DWORD sys_name_len = 4096;
			char sys_name[4096] = {0};

			HKEY hKey = NULL;
			LONG ret = RegOpenKeyExA(HKEY_LOCAL_MACHINE, subpath.c_str(), 0, KEY_QUERY_VALUE|KEY_READ, &hKey);
			ret = RegQueryValueExA(hKey, "Name", NULL, &val_type, (LPBYTE)sys_name, &sys_name_len);
			RegCloseKey(hKey);
			device_descs[index].sys_name_ = (const char *)sys_name;
		}
		

		device_descs[index].descript_ = current_dev->description;

		// ip
		sockaddr_in *a = (sockaddr_in *)current_dev->addresses->addr;
		char ipaddress[32] = {0};
		sprintf_s(ipaddress, 32, "%d.%d.%d.%d", a->sin_addr.S_un.S_un_b.s_b1, a->sin_addr.S_un.S_un_b.s_b2, a->sin_addr.S_un.S_un_b.s_b3, a->sin_addr.S_un.S_un_b.s_b4);
		device_descs[index].net_ip_ = (char *)ipaddress;

		// 掩码
		device_descs[index].mask_ip_ = ((struct sockaddr_in*)(current_dev->addresses->netmask))->sin_addr.S_un.S_addr;

		current_dev = current_dev->next;
	} while (current_dev->next != NULL);

	return 0;
}

int bgSniffer::OpenNetworkDevice(const char *device_name, unsigned int mask_ip /* = 0 */)
{
	char error_buf[PCAP_ERRBUF_SIZE] = {0};
	adapter_handle_ = pcap_open_live(device_name, 65536, 1, 1000, error_buf);
	if (adapter_handle_ == NULL)
		return -1;

	// 这里是否需要加载一个tcp的过滤器
	if (mask_ip != NULL)
	{
		// 编译过滤器
		struct bpf_program fcode;
		//char packet_filter[] = "ip and tcp";
		//char packet_filter[] = "tcp dst port 80";
		//char packet_filter[] = "tcp port 80";
		char packet_filter[] = "tcp port 1935";

		if(pcap_compile((pcap_t *)adapter_handle_, &fcode, packet_filter, 1, mask_ip) < 0)
			return -2;  

		// 设置过滤器  
		if(pcap_setfilter((pcap_t *)adapter_handle_, &fcode) < 0)
			return -3;
	}

	// 插件加载完成，启动工作线程
	HANDLE hThread = CreateThread(NULL, 0, bgSniffer::WorkingThread, this, 0, NULL);
	if (hThread == NULL)
	{
		int errCode = GetLastError();
		CloseNetworkDevice();
		return errCode;
	}

	CloseHandle(hThread);

	return 0;
}

void bgSniffer::CloseNetworkDevice()
{
	// 关闭网卡
	pcap_close((pcap_t *)adapter_handle_);
	adapter_handle_ = NULL;

	// 停止工作线程
	CreateEvent(NULL, FALSE, FALSE, _T("_wlr_LVSS_thread_exit_"));
	thread_exit_ = true;
	WaitForSingleObject(thread_exited_, INFINITE);

	// 清理工作插件
	//plugins_.clear();
}

DWORD bgSniffer::WorkingThread(LPVOID lpParam)
{
	int errCode = 0;
	bgSniffer *sniffer = (bgSniffer *)lpParam;
	sniffer->thread_exit_ = false;

	// 开始抓包
	struct pcap_pkthdr *header;
	const u_char *pkt_data;
	while ((errCode = pcap_next_ex((pcap_t *)sniffer->adapter_handle_, &header, &pkt_data)) >= 0)
	{
		// 外部停止线程命令
		if (sniffer->thread_exit_)
			break;
		
		if (errCode == 0)
			continue;

		// 以太包分析
		errCode = sniffer->ethernetii_->Parse((unsigned char *)header, pkt_data, 0);
	}

	SetEvent(sniffer->thread_exited_);
	return 0;
}