#ifndef _BG_PROTOCOL_ETHERNET_II_H_
#define _BG_PROTOCOL_ETHERNET_II_H_

#include "bgBasePacketParse.h"
#include "bgProtocolIPv4.h"

class bgProtocolEthernetII : public bgBasePacketParse
{
public:
	bgProtocolEthernetII(bgSnifferNotifer *notifer)
		: notifer_(notifer)
		, ipv4_(new bgProtocolIPv4(notifer))
	{}

	~bgProtocolEthernetII()
	{
		notifer_ = NULL;
		delete ipv4_;
		ipv4_ = NULL;
	}

public:
	virtual int Parse(unsigned char *header, const unsigned char *data, int size);

private:
	bgProtocolIPv4 *ipv4_;

private:
	bgSnifferNotifer *notifer_;
};

#endif//_BG_PROTOCOL_ETHERNET_II_H_