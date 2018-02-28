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
	// ö�����������豸
	int EnumAllNetworkDevices(int &count, PNETWORK_DEVICE_DESC device_descs);

	// ������
	int OpenNetworkDevice(NETWORK_DEVICE_DESC device_desc);

	// �ر�����
	void CloseNetworkDevice();

public:
	// ���ؽ�����
	int LoadDataParsePlugins(bgBasePacketParse *plugins);

public:
	std::vector<bgBasePacketParse*> plugins_;
};

#endif