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

	// ��������Ѿ���HTTP������
	// HTTP���ķ������ࣺ
	// 1. �����ģ�[����] + [����] + [�汾��] + \r\n\r\n
	//		[����]��GET | POST | HEAD | PUT | DELETE | OPTIONS | TRACE | CONNECT
	//      [�汾��]��HTTP/1.1
	// 2. Ӧ���ģ�[HTTP�汾��] + [��Ӧ��] + [��Ӧ״̬]

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

	// �ȼ���Ƿ���HTTP������
	bool is_http_data = false;
	for (int index = 0; index < verb_count; ++index)
	{
		int verb_pos = http_data.find(verb[index]);
		if (verb_pos == -1)
		{
			// û�ҵ��������е�Ԫ�أ�����������Ƿ���Ӧ����
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
		OutputDebugStringA("HTTP ���ݰ�\n");

		// ��������ֻ����HTTPͷ��
		int CRLF_pos = http_data.find("\r\n\r\n");
		std::string HTTP_HEAD = http_data.substr(0, CRLF_pos + strlen("\r\n\r\n"));

		// ����ͷ�����ó�HOST��OBJECT
		// OBJECT

		errCode = 0;
	}
	
	return errCode;
}

bgHttpHeader::bgHttpHeader(std::string http_header)
{

}