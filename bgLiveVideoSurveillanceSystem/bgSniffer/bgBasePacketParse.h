#ifndef _BG_BASE_PACKET_PARSE_H_
#define _BG_BASE_PACKET_PARSE_H_

#include <Windows.h>

class bgBasePacketParse
{
public:
	virtual int Parse(unsigned char *header, unsigned char *data, int size) = 0;
};

#endif//_BG_BASE_PACKET_PARSE_H_