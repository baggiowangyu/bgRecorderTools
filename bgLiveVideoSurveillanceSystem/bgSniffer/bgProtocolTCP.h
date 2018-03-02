#ifndef _BG_PROTOCOL_TCP_H_
#define _BG_PROTOCOL_TCP_H_

#include "bgBasePacketParse.h"
#include "bgProtocolHttp.h"
#include "bgProtocolRtmp.h"
#include <vector>

class bgProtocolTCP : public bgBasePacketParse
{
public:
	bgProtocolTCP(bgSnifferNotifer *notifer)
		: http_(new bgProtocolHttp(notifer))
		, rtmp_(new bgProtocolRtmp(notifer))
		, notifer_(notifer)
	{
		parses_.push_back(http_);
		parses_.push_back(rtmp_);
	}

	~bgProtocolTCP()
	{
		delete http_;
		delete rtmp_;

		http_ = NULL;
		rtmp_ = NULL;

		notifer_ = NULL;
	}

public:
	virtual int Parse(unsigned char *header, const unsigned char *data, int size);

private:
	bgProtocolHttp *http_;
	bgProtocolRtmp *rtmp_;

	std::vector<bgBasePacketParse *> parses_;

private:
	bgSnifferNotifer *notifer_;
};

#endif//_BG_PROTOCOL_TCP_H_