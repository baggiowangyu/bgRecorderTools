#include "stdafx.h"
#include "bgProtocolIPv4.h"
#include "bgNetworkProtocolStruct.h"



int bgProtocolIPv4::Parse(unsigned char *header, const unsigned char *data, int size)
{
	int errCode = -6002;

	// 这里拿到的数据应当是IPv4的数据报了
	struct pcap_pkthdr *pcap_header = (struct pcap_pkthdr *)header;
	struct bgIPv4 *ipv4_header_ = (struct bgIPv4 *)data;

	// 输出IPv4数据包的相关信息
	char trace_msg[4096] = {0};
	sprintf_s(trace_msg, 4096, "%d.%d.%d.%d >>> %d.%d.%d.%d\n",
		ipv4_header_->src_ip_[0],
		ipv4_header_->src_ip_[1],
		ipv4_header_->src_ip_[2],
		ipv4_header_->src_ip_[3],
		ipv4_header_->des_ip_[0],
		ipv4_header_->des_ip_[1],
		ipv4_header_->des_ip_[2],
		ipv4_header_->des_ip_[3]);
	OutputDebugStringA(trace_msg);

	// 计算出IPv4包的数据域
	int ipv4_header_len_ = (ipv4_header_->header_len_) * 4;
	int ipv4_data_len_ = ntohs(ipv4_header_->ipv4_len_) - ipv4_header_len_;
	unsigned char *ipv4_data = (unsigned char *)ipv4_header_ + ipv4_header_len_;
	

	int val = ntohs(ipv4_header_->protocl_);
	switch (val)
	{
	case TCP_PROTOCAL:
		errCode = tcp_.Parse(header, ipv4_data, ipv4_data_len_);
		break;
	default:
		break;
	}

	return errCode;
}