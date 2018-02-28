#ifndef _BG_PROTOCOL_IPV4_H_
#define _BG_PROTOCOL_IPV4_H_

#include "bgBasePacketParse.h"

class bgProtocolIPv4 : public bgBasePacketParse
{
public:
	bgProtocolIPv4(){}
	~bgProtocolIPv4(){}

public:
	virtual int Parse(unsigned char *header, unsigned char *data, int size);
};

#endif//_BG_PROTOCOL_IPV4_H_