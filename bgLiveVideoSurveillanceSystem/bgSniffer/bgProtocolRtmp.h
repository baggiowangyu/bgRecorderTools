#ifndef _BG_PROTOCOL_RTMP_H_
#define _BG_PROTOCOL_RTMP_H_

#include <string>
#include "bgBasePacketParse.h"


class bgSnifferNotifer;

class bgProtocolRtmp : public bgBasePacketParse
{
public:
	bgProtocolRtmp(bgSnifferNotifer *notifer);
	~bgProtocolRtmp();

public:
	virtual int Parse(unsigned char *header, const unsigned char *data, int size);

private:
	bgSnifferNotifer *notifer_;
	std::string url_section_1_;
	std::string url_section_2_;
};

#endif//_BG_PROTOCOL_RTMP_H_