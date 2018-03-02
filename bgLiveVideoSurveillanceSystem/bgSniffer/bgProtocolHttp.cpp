#include "stdafx.h"
#include "bgNetworkProtocolStruct.h"
#include "bgProtocolHttp.h"


bgProtocolHttp::bgProtocolHttp(bgSnifferNotifer *notifer)
: notifer_(notifer)
{

}

bgProtocolHttp::~bgProtocolHttp()
{
	notifer_ = NULL;
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

	// 先检查是否是HTTP请求报文
	bool is_http_data = false;
	bool is_request = false;
	for (int index = 0; index < verb_count; ++index)
	{
		int verb_pos = http_data.find(verb[index]);
		if (verb_pos == -1)
		{
			// 没找到请求报文中的元素，接下来检查是否是应答报文
			verb_pos = http_data.find(version[0]);
			if (verb_pos == 0)
			{
				// 这是应答报文
				is_http_data = true;
				break;
			}
			
			// 有版本号，但是位置不在开头，说明应该是其他的请求类型
			continue;
		}

		// 找到了Verb
		verb_pos = http_data.find(version[0]);
		if (verb_pos > 0)
		{
			is_request = true;
			is_http_data = true;
			break;
		}
	}

	if (is_http_data)
	{
		//OutputDebugStringA(http_data.c_str());
		//OutputDebugStringA("\n");

		// 这里我们只处理HTTP头部
		int CRLF_pos = http_data.find("\r\n\r\n");
		if (CRLF_pos == -1)
		{
			// 没找到，可能http头部超长了，那么我们就此在尾部加上\r\n\r\n
			http_data += "\r\n\r\n";
			CRLF_pos = http_data.find("\r\n\r\n");
		}
		std::string HTTP_HEAD = http_data.substr(0, CRLF_pos + strlen("\r\n\r\n"));

		// 分析头部，拿出HOST和OBJECT
		bgHttpHeader http_header_(HTTP_HEAD, is_request);

		// 拼接URL
		if (is_request)
		{
			// 这里先检查object，有几个策略
			// 1. 检查object是否有“?”
			// 2. 如果没有，则检查最后一截是什么后缀，例如：“.flv”、“.m3u8”
			// 3. 如果有，则截断“?”后面的数据，再按照 2 的逻辑进行处理
			std::string object = http_header_.GetObject();

			std::string url = "http://";
			url += http_header_.GetHeadFieldValue("Host");
			url += object;

			std::string pure_object;
			int pos = object.find('?');
			if (pos >= 0)
				pure_object = object.substr(0, pos - 1);
			else
				pure_object = object;

			int exts_count = 3;
			const char *exts[] = {
				{".flv"}, 
				{".m3u8"}, 
				{".jpg"}, 
				{""}
			};

			for (int ext_index = 0; ext_index < exts_count; ++ext_index)
			{
				pos = pure_object.find(exts[ext_index]);
				if (pos >= 0)
				{
					// 这个URL是可用的，发送消息抛出去
					notifer_->SnifferResultReport("http", url.c_str());
					break;
				}
			}

			// 

			OutputDebugStringA(url.c_str());
			OutputDebugStringA("\n");
		}

		errCode = 0;
	}
	
	return errCode;
}

bgHttpHeader::bgHttpHeader(std::string http_header, bool is_request)
{
	if (is_request)
	{
		// 第一行：请求行，包含verb obj version
		std::string line_1 = http_header;
		int pos = line_1.find(' ');
		verb_ = line_1.substr(0, pos);

		std::string remain_data = line_1.substr(pos + 1);
		pos = remain_data.find(' ');
		obj_ = remain_data.substr(0, pos);

		remain_data = remain_data.substr(pos + 1);
		pos = remain_data.find('\n');
		version_ = remain_data.substr(0, pos);

		remain_data = remain_data.substr(pos + 1);

		// 后面剩下的都是key-value形式的数据，以:为分割，字符前后不能留有空格
		while (true)
		{
			pos = remain_data.find(": ");
			std::string key = remain_data.substr(0, pos);
			
			remain_data = remain_data.substr(pos + strlen(": "));
			pos = remain_data.find('\n');
			std::string value = remain_data.substr(0, pos - 1);

			header_values_.insert(std::pair<std::string, std::string>(key, value));

			// 一行即将处理完成，检查是否到结尾了
			remain_data = remain_data.substr(pos + strlen("\n"));
			pos = remain_data.find('\r\n\r\n');
			
			if (pos == 0)
				break;
		}
	}
	else
	{
		// 第一行：应答行
		std::string line_1 = http_header;
		int pos = line_1.find(' ');
		version_ = line_1.substr(0, pos);

		std::string remain_data = line_1.substr(pos + 1);
		pos = remain_data.find(' ');
		code_ = remain_data.substr(0, pos);

		remain_data = remain_data.substr(pos + 1);
		pos = remain_data.find('\n');
		state_ = remain_data.substr(0, pos);

		remain_data = remain_data.substr(pos + 1);

		// 后面剩下的都是key-value形式的数据，以:为分割，字符前后不能留有空格
		while (true)
		{
			pos = remain_data.find(": ");
			std::string key = remain_data.substr(0, pos);

			remain_data = remain_data.substr(pos + strlen(": "));
			pos = remain_data.find('\n');
			std::string value = remain_data.substr(0, pos - 1);

			header_values_.insert(std::pair<std::string, std::string>(key, value));

			// 一行即将处理完成，检查是否到结尾了
			remain_data = remain_data.substr(pos + strlen("\n"));
			pos = remain_data.find('\r\n\r\n');

			if (pos == 0)
				break;
		}
	}
}

bgHttpHeader::~bgHttpHeader()
{

}

std::string bgHttpHeader::GetVerb()
{
	return verb_;
}

std::string bgHttpHeader::GetObject()
{
	return obj_;
}

std::string bgHttpHeader::GetVersion()
{
	return version_;
}

std::string bgHttpHeader::GetResultCode()
{
	return code_;
}

std::string bgHttpHeader::GetResultState()
{
	return state_;
}

std::string bgHttpHeader::GetHeadFieldValue(std::string key)
{
	std::map<std::string, std::string>::iterator iter = header_values_.find(key);
	return iter->second;
}