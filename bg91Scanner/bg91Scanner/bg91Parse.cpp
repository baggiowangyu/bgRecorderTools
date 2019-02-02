#include "stdafx.h"
#include "bg91Parse.h"
#include <string>
#include <sstream>

bg91Parse::bg91Parse()
: current_page_index_(1)
//, http_client_session_(NULL)
//, https_client_session_(NULL)
{
	//
}

bg91Parse::~bg91Parse()
{
	//
}

int bg91Parse::Initialize(const char *root_url)
{
	int errCode = 0;
	std::string errstr;

	Poco::URI uri(root_url);
	schame_ = uri.getScheme();
	host_ = uri.getHost();

	try
	{
		if (_stricmp(schame_.c_str(), "https") == 0)
		{
			//Poco::Net::Context::Ptr context = new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", "", "", Poco::Net::Context::VERIFY_NONE, 9, false);
			//https_client_session_ = new Poco::Net::HTTPSClientSession(host_.c_str(), uri.getPort(), context);
			//if (https_client_session_ == NULL)
			//	errCode = -1;

			use_ssl_ = true;
		}
		else
		{
			//http_client_session_ = new Poco::Net::HTTPClientSession(host_.c_str(), uri.getPort());
			//if (http_client_session_ == NULL)
			//	errCode = -2;

			use_ssl_ = false;
		}
	}
	catch (Poco::Exception ex)
	{
		errstr = ex.what();
		errCode = ex.code();
	}
	

	return errCode;
}

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) 
{
	std::string data((const char*) ptr, (size_t) size * nmemb);

	*((std::stringstream*) stream) << data << std::endl;

	return size * nmemb;
}

int bg91Parse::ScanFirstPage()
{
	int errCode = 0;
	std::string errstr;

	CURLcode res; 
	CURL *curl = curl_easy_init();

	std::string url = schame_ + "://" + host_ + "/v.php?next=watch";

	// set params
	std::stringstream response;
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // url  

	if (use_ssl_)
	{
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https  
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false  
	}
	
	//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);  
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);  
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);  
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);  
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);  
	curl_easy_setopt(curl, CURLOPT_HEADER, 0);  
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30); // set transport and time out time  
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);

	// start req  
	res = curl_easy_perform(curl);

	std::string result = response.str();

	// utf-8תgbk(���İ�ϵͳĬ��ΪGBK)
	UTF8ToACP(result);
	
	// ����ҳ��
	errCode = AnalyzeListPage(result);

	return errCode;
}




void bg91Parse::UTF8ToACP(std::string &convert_string)
{
	// Ԥ��-�������п��ֽڵĳ���  
	int unicodeLen = MultiByteToWideChar(CP_UTF8, 0, convert_string.c_str(), -1, NULL, 0);
	// ��ָ�򻺳�����ָ����������ڴ�  
	wchar_t *pUnicode = (wchar_t*)malloc(sizeof(wchar_t)*unicodeLen);
	// ��ʼ�򻺳���ת���ֽ�  
	MultiByteToWideChar(CP_UTF8, 0, convert_string.c_str(), -1, pUnicode, unicodeLen);

	std::wstring ret_str = pUnicode;
	int char_len = WideCharToMultiByte(CP_ACP, 0, ret_str.c_str(), -1, NULL, 0, NULL, NULL);

	char *buffer = new char[char_len + 1];
	WideCharToMultiByte(CP_ACP, 0, ret_str.c_str(), -1, buffer, char_len, NULL, NULL);

	convert_string = buffer;

	free(pUnicode);
	pUnicode = NULL;

	delete [] buffer;
	buffer = NULL;
}

int bg91Parse::AnalyzeListPage(std::string &page_data)
{
	int errCode = 0;

	videos_.clear();

	// ѭ����ȡ
	std::string analyze_data = page_data;
	while (true)
	{
		VIDEO_INFO video_info;

		// �����ҵ���һ���ؼ��֡�class=\"listchannel\"��
		int list_begin_pos = analyze_data.find("class=\"listchannel\"");
		if (list_begin_pos < 0)
			break;

		// ��ȡ�ַ��Ӵ�
		std::string list_element_begin = analyze_data.substr(list_begin_pos + strlen("class=\"listchannel\""));

		// ���ŵ�ַ
		std::string video_page;
		{
			// �Ҳ���ҳ��URL,�ؼ��֡�<a target=blank href=\"������������\"��
			int begin_pos = list_element_begin.find("<a target=blank href=\"");
			if (begin_pos < 0)
				break;

			// ��ȡ�ַ��Ӵ�
			analyze_data = list_element_begin.substr(begin_pos + strlen("<a target=blank href=\""));
			int end_pos = analyze_data.find("\"");
			if (end_pos < 0)
				break;

			// �ҵ�����ҳ���ַ
			video_info.video_page_ = analyze_data.substr(0, end_pos);

			analyze_data = analyze_data.substr(end_pos);
		}

		// Ԥ��ͼ��ַ
		std::string video_thumbnail;
		{
			// ��Ԥ��ͼ��ַ,�ؼ��֡�<img src=\"������������\"��
			int begin_pos = analyze_data.find("<img src=\"");
			if (begin_pos < 0)
				break;

			// ��ȡ�ַ��Ӵ�
			analyze_data = analyze_data.substr(begin_pos + strlen("<img src=\""));
			int end_pos = analyze_data.find("\"");
			if (end_pos < 0)
				break;

			// �ҵ�����ҳ���ַ
			video_info.video_thumbnail_ = analyze_data.substr(0, end_pos);

			analyze_data = analyze_data.substr(end_pos);
		}

		// ��Ƶ����
		std::string video_name;
		{
			// ��Ԥ��ͼ��ַ,�ؼ��֡�<img src=\"������������\"��
			int begin_pos = analyze_data.find("title=\"");
			if (begin_pos < 0)
				break;

			// ��ȡ�ַ��Ӵ�
			analyze_data = analyze_data.substr(begin_pos + strlen("title=\""));
			int end_pos = analyze_data.find("\"");
			if (end_pos < 0)
				break;

			// �ҵ�����ҳ���ַ
			video_info.video_name_ = analyze_data.substr(0, end_pos - 1);

			analyze_data = analyze_data.substr(end_pos);
		}

		// ��Ƶʱ��
		std::string video_duration;
		{
			// ��Ԥ��ͼ��ַ,�ؼ��֡�<img src=\"������������\"��
			//int begin_pos = analyze_data.find("<span class=\"info\">ʱ��:</span>");
			int begin_pos = analyze_data.find("<span class=\"info\">Runtime:</span>");
			if (begin_pos < 0)
				break;

			// ��ȡ�ַ��Ӵ�
			analyze_data = analyze_data.substr(begin_pos + strlen("<span class=\"info\">Runtime:</span>"));
			int end_pos = analyze_data.find("\x0a");
			if (end_pos < 0)
				break;

			// �ҵ�����ҳ���ַ
			video_info.video_duration_ = analyze_data.substr(0, end_pos);

			analyze_data = analyze_data.substr(end_pos);
		}

		// ����
		std::string video_author;
		{
			// ��Ԥ��ͼ��ַ,�ؼ��֡�<img src=\"������������\"��
			int begin_pos = analyze_data.find("<span class=\"info\">From:</span>");
			if (begin_pos < 0)
				break;

			// ��ȡ�ַ��Ӵ�
			analyze_data = analyze_data.substr(begin_pos + strlen("<span class=\"info\">From:</span>"));
			int end_pos = analyze_data.find("\x0a");
			if (end_pos < 0)
				break;

			// �ҵ�����ҳ���ַ
			video_info.video_author_ = analyze_data.substr(0, end_pos);

			analyze_data = analyze_data.substr(end_pos);
		}

		videos_.push_back(video_info);
	}

	return errCode;
}