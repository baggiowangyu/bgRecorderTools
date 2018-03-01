#include "stdafx.h"
#include "bgNetworkProtocolStruct.h"
#include "bgProtocolHttp.h"


bgProtocolHttp::bgProtocolHttp()
{

}

bgProtocolHttp::~bgProtocolHttp()
{

}

int bgProtocolHttp::Parse(unsigned char *header, const unsigned char *data, int size)
{
	int errCode = -6002;

	// 这里进来已经是HTTP报文了
	// HTTP报文分类两类：
	// 1. 请求报文：[操作] + [对象] + [版本号] + \r\n\r\n
	//		[操作]：GET | POST | HEAD | PUT | DELETE | OPTIONS | TRACE | CONNECT
	//      [版本号]：HTTP/1.1
	// 2. 应答报文：[HTTP版本号] + [响应码] + [响应状态]

	int verb_count = 2;
	const char *verb[] = {
		{"GET"},
		{"POST"},
		{"HEAD"},
		{"PUT"},
		{"DELETE"},
		{"OPTIONS"},
		{"TRACE"},
		{"CONNECT"}
	};

	int version_count = 1;
	const char* version[] = {
		{"HTTP/1.1"}
	};

	std::string http_data = (const char *)data;
	OutputDebugStringA(http_data.c_str());
	OutputDebugStringA("\n");

	// 先检查是否是HTTP请求报文
	bool is_http_data = false;
	for (int index = 0; index < verb_count; ++index)
	{
		int verb_pos = http_data.find(verb[index]);
		if (verb_pos == -1)
		{
			// 没找到请求报文中的元素，接下来检查是否是应答报文
			verb_pos = http_data.find(version[0]);
			if (verb_pos >= 0)
				is_http_data = true;

			break;
		}

		verb_pos = http_data.find(version[0]);
		if (verb_pos > 0)
		{
			is_http_data = true;
			break;
		}
	}

	if (is_http_data)
	{
		OutputDebugStringA("HTTP 数据包\n");

		// 这里我们只处理HTTP头部
		int CRLF_pos = http_data.find("\r\n\r\n");
		std::string HTTP_HEAD = http_data.substr(0, CRLF_pos + strlen("\r\n\r\n"));

		// 分析头部，拿出HOST和OBJECT
		// OBJECT

		errCode = 0;
	}
	
	return errCode;
}

bgHttpHeader::bgHttpHeader(std::string http_header)
{

}