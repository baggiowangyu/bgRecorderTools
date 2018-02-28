#ifndef _BG_PROTOCOL_ETHERNET_II_H_
#define _BG_PROTOCOL_ETHERNET_II_H_

#include "bgBasePacketParse.h"

class bgProtocolEthernetII : public bgBasePacketParse
{
public:
	bgProtocolEthernetII(){}
	~bgProtocolEthernetII(){}

public:
	virtual int Parse(unsigned char *header, unsigned char *data, int size);
};

#endif//_BG_PROTOCOL_ETHERNET_II_H_