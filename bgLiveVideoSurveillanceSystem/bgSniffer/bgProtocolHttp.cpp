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

	// �ȼ���Ƿ���HTTP������
	bool is_http_data = false;
	bool is_request = false;
	for (int index = 0; index < verb_count; ++index)
	{
		int verb_pos = http_data.find(verb[index]);
		if (verb_pos == -1)
		{
			// û�ҵ��������е�Ԫ�أ�����������Ƿ���Ӧ����
			verb_pos = http_data.find(version[0]);
			if (verb_pos == 0)
			{
				// ����Ӧ����
				is_http_data = true;
				break;
			}
			
			// �а汾�ţ�����λ�ò��ڿ�ͷ��˵��Ӧ������������������
			continue;
		}

		// �ҵ���Verb
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

		// ��������ֻ����HTTPͷ��
		int CRLF_pos = http_data.find("\r\n\r\n");
		if (CRLF_pos == -1)
		{
			// û�ҵ�������httpͷ�������ˣ���ô���Ǿʹ���β������\r\n\r\n
			http_data += "\r\n\r\n";
			CRLF_pos = http_data.find("\r\n\r\n");
		}
		std::string HTTP_HEAD = http_data.substr(0, CRLF_pos + strlen("\r\n\r\n"));

		// ����ͷ�����ó�HOST��OBJECT
		bgHttpHeader http_header_(HTTP_HEAD, is_request);

		// ƴ��URL
		if (is_request)
		{
			// �����ȼ��object���м�������
			// 1. ���object�Ƿ��С�?��
			// 2. ���û�У��������һ����ʲô��׺�����磺��.flv������.m3u8��
			// 3. ����У���ضϡ�?����������ݣ��ٰ��� 2 ���߼����д���
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
					// ���URL�ǿ��õģ�������Ϣ�׳�ȥ
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
		// ��һ�У������У�����verb obj version
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

		// ����ʣ�µĶ���key-value��ʽ�����ݣ���:Ϊ�ָ�ַ�ǰ�������пո�
		while (true)
		{
			pos = remain_data.find(": ");
			std::string key = remain_data.substr(0, pos);
			
			remain_data = remain_data.substr(pos + strlen(": "));
			pos = remain_data.find('\n');
			std::string value = remain_data.substr(0, pos - 1);

			header_values_.insert(std::pair<std::string, std::string>(key, value));

			// һ�м���������ɣ�����Ƿ񵽽�β��
			remain_data = remain_data.substr(pos + strlen("\n"));
			pos = remain_data.find('\r\n\r\n');
			
			if (pos == 0)
				break;
		}
	}
	else
	{
		// ��һ�У�Ӧ����
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

		// ����ʣ�µĶ���key-value��ʽ�����ݣ���:Ϊ�ָ�ַ�ǰ�������пո�
		while (true)
		{
			pos = remain_data.find(": ");
			std::string key = remain_data.substr(0, pos);

			remain_data = remain_data.substr(pos + strlen(": "));
			pos = remain_data.find('\n');
			std::string value = remain_data.substr(0, pos - 1);

			header_values_.insert(std::pair<std::string, std::string>(key, value));

			// һ�м���������ɣ�����Ƿ񵽽�β��
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