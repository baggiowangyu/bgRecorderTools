#ifndef _BG_SNIFFER_H_
#define _BG_SNIFFER_H_

#include "bgBasePacketParse.h"
#include "pcap.h"

typedef struct _NETWORK_DEVICE_DESC_
{
	string name_;
	string descript_;
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
	int OpenNetworkDevice(NETWORK_DEVICE_DESC device_desc);

	// 关闭网卡
	void CloseNetworkDevice();

public:
	// 加载解包插件
	int LoadDataParsePlugins(bgBasePacketParse *plugins);

public:
	std::vector<bgBasePacketParse*> plugins_;
};

#endif