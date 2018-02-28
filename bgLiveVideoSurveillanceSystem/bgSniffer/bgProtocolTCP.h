#ifndef _BG_PROTOCOL_TCP_H_
#define _BG_PROTOCOL_TCP_H_

#include "bgBasePacketParse.h"

class bgProtocolTCP : public bgBasePacketParse
{
public:
	bgProtocolTCP(){}
	~bgProtocolTCP(){}

public:
	virtual int Parse(unsigned char *header, unsigned char *data, int size);
};

#endif//_BG_PROTOCOL_TCP_H_