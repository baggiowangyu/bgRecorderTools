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
	// ��ö�����е��豸
	pcap_if_t *alldevs;
	char errbuf[PCAP_ERRBUF_SIZE + 1] = {0};
	int errCode = pcap_findalldevs(&alldevs, errbuf);
	if (errCode < 0)
	{
		return errCode;
	}

	// ����������Ϊ�գ���ֻ������������
	if (device_descs == NULL)
	{

	}
}