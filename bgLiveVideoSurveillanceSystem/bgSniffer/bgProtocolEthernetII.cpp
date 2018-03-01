#include "stdafx.h"
#include "bgProtocolEthernetII.h"
#include "pcap.h"
#include "bgNetworkProtocolStruct.h"


int bgProtocolEthernetII::Parse(unsigned char *header, const unsigned char *data, int size)
{
	// �����õ�������̫�����ݱ�
	struct pcap_pkthdr *pcap_header = (struct pcap_pkthdr *)header;
	struct bgEthernetII *ethernet_ii_header = (struct bgEthernetII *)data;

	// �����̫���еĲ���
	char timestr[16] = {0};
	time_t local_tv_sec = pcap_header->ts.tv_sec;
	struct tm *ltime = localtime(&local_tv_sec);
	strftime(timestr, sizeof(timestr), "%H:%M:%S", ltime);
	char trace_msg[4096] = {0};
	sprintf_s(trace_msg, 4096, "��̫������ʱ��:%s.%.6d\t������:%d\n",
		timestr,
		pcap_header->ts.tv_usec,
		pcap_header->len
		);
	OutputDebugStringA(trace_msg);

	if (pcap_header->len < 200)
		return 0;

	// ������̫��ȥ����ͷ���������
	int ethernet_len = sizeof(bgEthernetII);
	const unsigned char *ethernetii_data = (const unsigned char *)data + ethernet_len;

	// ������̫�����ݱ���Э�飬ȥ����̫��ͷ�����ݷַ�����������ģ��
	int val = ntohs(ethernet_ii_header->type_);
	switch (val)
	{
	case ETHERTYPE_IPV4:
		// ����IPv4���ݰ�
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