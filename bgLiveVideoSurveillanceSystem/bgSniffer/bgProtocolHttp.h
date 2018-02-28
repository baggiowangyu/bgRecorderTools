#ifndef _BG_PROTOCOL_HTTP_H_
#define _BG_PROTOCOL_HTTP_H_

#include "bgBasePacketParse.h"

class bgProtocolHttp : public bgBasePacketParse
{
public:
	bgProtocolHttp();
	~bgProtocolHttp();

public:
	virtual int Parse(unsigned char *header, unsigned char *data, int size);
};

#endif//_BG_PROTOCOL_HTTP_H_