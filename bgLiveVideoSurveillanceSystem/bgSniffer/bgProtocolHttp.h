#ifndef _BG_PROTOCOL_HTTP_H_
#define _BG_PROTOCOL_HTTP_H_

#include "bgBasePacketParse.h"
#include <string>
#include <map>


class bgSnifferNotifer;

class bgProtocolHttp : public bgBasePacketParse
{
public:
	bgProtocolHttp(bgSnifferNotifer *notifer);
	~bgProtocolHttp();

public:
	virtual int Parse(unsigned char *header, const unsigned char *data, int size);

private:
	bgSnifferNotifer *notifer_;
};


class bgHttpHeader
{
public:
	bgHttpHeader(std::string http_header, bool is_request);
	~bgHttpHeader();

public:
	std::string GetVerb();
	std::string GetObject();
	std::string GetVersion();
	std::string GetResultCode();
	std::string GetResultState();
	std::string GetHeadFieldValue(std::string key);

private:
	std::string verb_;
	std::string obj_;
	std::string version_;
	std::string code_;
	std::string state_;
	std::map<std::string, std::string> header_values_;
};

#endif//_BG_PROTOCOL_HTTP_H_