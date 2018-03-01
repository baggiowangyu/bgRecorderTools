#include "stdafx.h"
#include "bgProtocolEthernetII.h"
#include "pcap.h"
#include "bgNetworkProtocolStruct.h"


int bgProtocolEthernetII::Parse(unsigned char *header, const unsigned char *data, int size)
{
	// 这里拿到的是以太网数据报
	struct pcap_pkthdr *pcap_header = (struct pcap_pkthdr *)header;
	struct bgEthernetII *ethernet_ii_header = (struct bgEthernetII *)data;

	// 输出以太包中的参数
	char timestr[16] = {0};
	time_t local_tv_sec = pcap_header->ts.tv_sec;
	struct tm *ltime = localtime(&local_tv_sec);
	strftime(timestr, sizeof(timestr), "%H:%M:%S", ltime);
	char trace_msg[4096] = {0};
	sprintf_s(trace_msg, 4096, "以太包发生时间:%s.%.6d\t包长度:%d\n",
		timestr,
		pcap_header->ts.tv_usec,
		pcap_header->len
		);
	OutputDebugStringA(trace_msg);

	if (pcap_header->len < 200)
		return 0;

	// 计算以太包去掉包头后的数据域
	int ethernet_len = sizeof(bgEthernetII);
	const unsigned char *ethernetii_data = (const unsigned char *)data + ethernet_len;

	// 根据以太网数据报的协议，去掉以太包头将数据分发给各个处理模块
	int val = ntohs(ethernet_ii_header->type_);
	switch (val)
	{
	case ETHERTYPE_IPV4:
		// 解析IPv4数据包
		ipv4_.Parse(header, ethernetii_data, size);
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
	return 0;
}