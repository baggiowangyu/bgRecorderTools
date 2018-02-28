#ifndef _BG_BASE_PACKET_PARSE_H_
#define _BG_BASE_PACKET_PARSE_H_

class bgBasePacketParse
{
public:
	virtual Parse(unsigned char *data, int size) = 0;
};

#endif//_BG_BASE_PACKET_PARSE_H_