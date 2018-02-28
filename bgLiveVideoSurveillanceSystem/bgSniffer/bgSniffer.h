#ifndef _BG_SNIFFER_H_
#define _BG_SNIFFER_H_

#include <string>
#include <vector>
#include "bgBasePacketParse.h"



typedef struct _NETWORK_DEVICE_DESC_
{
	std::string name_;		// 名称
	std::string descript_;	// 描述
	std::string net_ip_;	// 网卡IP
	unsigned int mask_ip_;	// 子网掩码
	std::string sys_name_;	// 在系统中展示的名称
} NETWORK_DEVICE_DESC, *PNETWORK_DEVICE_DESC;

class bgSniffer
{
public:
	bgSniffer();
	~bgSniffer();

public:
	// 枚举所有网络设备
	int EnumAllNetworkDevices(int &count, PNETWORK_DEVICE_DESC device_descs);

	// 打开网卡
	int OpenNetworkDevice(const char *device_name, unsigned int mask_ip = 0);

	// 关闭网卡
	void CloseNetworkDevice();

public:
	// 加载解包插件
	int LoadDataParsePlugins(bgBasePacketParse *plugin);

	int DispatchPacket(const unsigned char *data, int len);

public:
	static DWORD WINAPI WorkingThread(LPVOID lpParam);

public:
	void *adapter_handle_;
	std::vector<bgBasePacketParse*> plugins_;
	bool thread_exit_;
	HANDLE thread_exited_;
};

#endif