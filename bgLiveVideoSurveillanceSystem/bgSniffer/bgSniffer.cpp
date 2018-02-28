#include "stdafx.h"
#include "bgSniffer.h"

bgSniffer::bgSniffer()
{

}

bgSniffer::~bgSniffer()
{

}

int bgSniffer::EnumAllNetworkDevices(int &count, PNETWORK_DEVICE_DESC device_descs)
{
	// 先枚举所有的设备
	pcap_if_t *alldevs;
	char errbuf[PCAP_ERRBUF_SIZE + 1] = {0};
	int errCode = pcap_findalldevs(&alldevs, errbuf);
	if (errCode < 0)
	{
		return errCode;
	}

	// 如果输出缓冲为空，则只返回网卡个数
	if (device_descs == NULL)
	{

	}
}