#include "stdafx.h"
#include "bgSniffer.h"
#include <shlwapi.h>
#include <tchar.h>
#include "pcap.h"

#include "bgNetworkProtocolStruct.h"

#define HAVE_REMOTE
#include "remote-ext.h"

bgSniffer::bgSniffer()
: adapter_handle_(NULL)
, thread_exit_(true)
{

}

bgSniffer::~bgSniffer()
{

}

int bgSniffer::EnumAllNetworkDevices(int &count, PNETWORK_DEVICE_DESC device_descs)
{
	// ��ö�����е��豸
	pcap_if_t *alldevs = NULL;
	char errbuf[PCAP_ERRBUF_SIZE + 1] = {0};
	int errCode = pcap_findalldevs(&alldevs, errbuf);
	if (errCode < 0)
		return errCode;

	// �ȱ���һ�飬��������
	int device_count = 0;
	pcap_if_t *current_dev = alldevs;
	do 
	{
		++device_count;
		current_dev = current_dev->next;
	} while (current_dev->next != NULL);

	// �������㣬������Ҫ������
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

		// ת��Ϊ��ϵͳ��չʾ������
		// ��ѯע���HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Network\{4D36E972-E325-11CE-BFC1-08002BE10318}\[GUID]\Connection => Name
		{
			// �ȴ�����������ȡGUID��Ѱ���»���
			int pos = device_descs[index].name_.find("_");
			std::string guid = device_descs[index].name_.substr(pos + 1);
			
			// ��֯ע�����·��
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

		// ����
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

	// �����Ƿ���Ҫ����һ��tcp�Ĺ�����
	if (mask_ip != NULL)
	{
		// ���������
		struct bpf_program fcode;
		char packet_filter[] = "ip and tcp";

		if(pcap_compile((pcap_t *)adapter_handle_, &fcode, packet_filter, 1, mask_ip) < 0)
			return -2;  

		// ���ù�����  
		if(pcap_setfilter((pcap_t *)adapter_handle_, &fcode) < 0)
			return -3;
	}

	// �豸�ɹ��򿪣����ظ������
	// 1. ����HTTPЭ��������
	// 2. ����RTMPЭ��������
	// 2. ����RTSPЭ��������

	// ���������ɣ����������߳�
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
	// �ر�����
	pcap_close((pcap_t *)adapter_handle_);
	adapter_handle_ = NULL;

	// ֹͣ�����߳�
	CreateEvent(NULL, FALSE, FALSE, _T("_wlr_LVSS_thread_exit_"));
	thread_exit_ = true;
	WaitForSingleObject(thread_exited_, INFINITE);

	// ���������
	plugins_.clear();
}

int bgSniffer::LoadDataParsePlugins(bgBasePacketParse *plugin)
{
	plugins_.push_back(plugin);
	return 0;
}

int bgSniffer::DispatchPacket(const unsigned char *data, int len)
{
	return 0;
}

DWORD bgSniffer::WorkingThread(LPVOID lpParam)
{
	int errCode = 0;
	bgSniffer *sniffer = (bgSniffer *)lpParam;
	sniffer->thread_exit_ = false;

	// ��ʼץ��
	struct pcap_pkthdr *header;
	const u_char *pkt_data;
	while ((errCode = pcap_next_ex((pcap_t *)sniffer->adapter_handle_, &header, &pkt_data)) >= 0)
	{
		// �ⲿֹͣ�߳�����
		if (sniffer->thread_exit_)
			break;
		
		if (errCode == 0)
			continue;

		// �ȵ��Դ�ӡ���ݰ�������Ϣ

		// ��ʱ���ת���ɿ�ʶ��ĸ�ʽ
		char timestr[16] = {0};
		time_t local_tv_sec = header->ts.tv_sec;
		struct tm *ltime = localtime(&local_tv_sec);
		strftime(timestr, sizeof(timestr), "%H:%M:%S", ltime);
		ip_header *ih = (ip_header *)(pkt_data + 14); //��̫��ͷ������

		char trace_msg[4096] = {0};
		sprintf_s(trace_msg, 4096, "%s.%.6d\tlength:%d\t%d.%d.%d.%d >>> %d.%d.%d.%d\n",
			timestr,
			header->ts.tv_usec,
			header->len,
			ih->saddr.byte1,  
			ih->saddr.byte2,  
			ih->saddr.byte3,  
			ih->saddr.byte4,  
			ih->daddr.byte1,  
			ih->daddr.byte2,  
			ih->daddr.byte3,  
			ih->daddr.byte4);
		OutputDebugStringA(trace_msg);

		// ��ԭʼ������ȡ��TCP��
		struct bgEthernetII *ethernet_ii_header = (struct bgEthernetII *)pkt_data;

		switch (ethernet_ii_header->type_)
		{
		case ETHERTYPE_IPV4:
			// ����IPv4���ݰ�
			break;
		case ETHERTYPE_ARP:
			break;
		case ETHERTYPE_RARP:
			break;
		case ETHERTYPE_SNMP:
			break;
		case ETHERTYPE_IPX:
			break;
		case ETHERTYPE_IPV6:
			break;
		case ETHERTYPE_PPP:
			break;
		default:
			break;
		}

		// �����ݰ��ֱ��ӽ�������ģ��
		errCode = sniffer->DispatchPacket(pkt_data, header->len);
	}

	SetEvent(sniffer->thread_exited_);
	return 0;
}