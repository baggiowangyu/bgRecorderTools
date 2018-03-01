#ifndef _BG_PROTOCOL_ETHERNET_II_H_
#define _BG_PROTOCOL_ETHERNET_II_H_

#include "bgBasePacketParse.h"
#include "bgProtocolIPv4.h"

class bgProtocolEthernetII : public bgBasePacketParse
{
public:
	bgProtocolEthernetII(){}
	~bgProtocolEthernetII(){}

public:
	virtual int Parse(unsigned char *header, const unsigned char *data, int size);

private:
	bgProtocolIPv4 ipv4_;
};

#endif//_BG_PROTOCOL_ETHERNET_II_H_