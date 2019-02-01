#ifndef _bg91Parse_H_
#define _bg91Parse_H_

#include <string>
#include <vector>

#include "Poco/URI.h"
#include "Poco/StreamCopier.h"
#include "Poco/UTF8Encoding.h"
#include "Poco/ASCIIEncoding.h"
#include "Poco/TextConverter.h"
#include "Poco/Net/MediaType.h"
#include "Poco/JSON/JSON.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Exception.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/Context.h"

#include "curl/curl.h"

typedef struct _VIDEO_INFO_
{
	std::string video_name_;		// 名称
	std::string video_duration_;	// 时长
	std::string video_author_;		// 作者
	std::string video_thumbnail_;	// 缩略图地址
	std::string video_page_;		// 播放页面

} VIDEO_INFO, *PVIDEO_INFO;

class bg91Parse
{
public:
	bg91Parse();
	~bg91Parse();

public:
	int Initialize(const char *root_url);

public:
	int ScanFirstPage();
	int ScanNextPage();
	int ScanPreviousPage();
	int Scan(int page_index);

public:
	std::string schame_;
	std::string host_;
	bool use_ssl_;

public:
	int current_page_index_;
	std::vector<VIDEO_INFO> videos_;

public:
	//Poco::Net::HTTPClientSession *http_client_session_;
	//Poco::Net::HTTPSClientSession *https_client_session_;
};

#endif//_bg91Parse_H_
