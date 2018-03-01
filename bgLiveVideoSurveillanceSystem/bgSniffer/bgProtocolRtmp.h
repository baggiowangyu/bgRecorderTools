#ifndef _BG_PROTOCOL_RTMP_H_
#define _BG_PROTOCOL_RTMP_H_

#include "bgBasePacketParse.h"

class bgProtocolRtmp : public bgBasePacketParse
{
public:
	bgProtocolRtmp();
	~bgProtocolRtmp();

public:
	virtual int Parse(unsigned char *header, const unsigned char *data, int size);
};

#endif//_BG_PROTOCOL_RTMP_H_