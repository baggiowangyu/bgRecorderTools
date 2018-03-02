#ifndef _BG_PROTOCOL_IPV4_H_
#define _BG_PROTOCOL_IPV4_H_

#include "bgBasePacketParse.h"
#include "bgProtocolTCP.h"

class bgProtocolIPv4 : public bgBasePacketParse
{
public:
	bgProtocolIPv4(bgSnifferNotifer *notifer)
		: notifer_(notifer)
		, tcp_(new bgProtocolTCP(notifer))
	{}

	~bgProtocolIPv4()
	{
		notifer_ = NULL;
		delete tcp_;
		tcp_ = NULL;
	}

public:
	virtual int Parse(unsigned char *header, const unsigned char *data, int size);

private:
	bgProtocolTCP *tcp_;

private:
	bgSnifferNotifer *notifer_;
};

#endif//_BG_PROTOCOL_IPV4_H_