#include "stdafx.h"
#include "bg91Parse.h"

bg91Parse::bg91Parse()
: current_page_index_(1)
, http_client_session_(NULL)
, https_client_session_(NULL)
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

int bg91Parse::ScanFirstPage()
{
	int errCode = 0;
	std::string errstr;

	CURLcode res; 
	CURL *curl = curl_easy_init();

	std::string url = schame_ + "://" + host_ + "/v.php?next=watch";

	// set params
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // url  
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https  
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false  
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);  
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);  
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);  
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);  
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);  
	curl_easy_setopt(curl, CURLOPT_HEADER, 1);  
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3); // set transport and time out time  
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);  
	// start req  
	res = curl_easy_perform(curl);  

	try
	{
		char query_string[4096] = {0};
		sprintf_s(query_string, 4096,"/v.php?next=watch");
		std::string uri = query_string;
		Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, uri, Poco::Net::HTTPRequest::HTTP_1_1);

		if (use_ssl_)
			https_client_session_->sendRequest(request);
		else
			http_client_session_->sendRequest(request);

		std::ostringstream ostr;
		Poco::Net::HTTPResponse response;
		if (use_ssl_)
		{
			std::istream &is = https_client_session_->receiveResponse(response);

			// 判断服务器返回信息
			Poco::Net::HTTPResponse::HTTPStatus status = response.getStatus();
			if (Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK != status)
			{
				errCode = status;
				return errCode;
			}

			Poco::StreamCopier::copyStream(is, ostr);
		}
		else
		{
			std::istream &is = http_client_session_->receiveResponse(response);

			// 判断服务器返回信息
			Poco::Net::HTTPResponse::HTTPStatus status = response.getStatus();
			if (Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK != status)
			{
				errCode = status;
				return errCode;
			}

			Poco::StreamCopier::copyStream(is, ostr);
		}

		std::string json_str = ostr.str();
		if (json_str.empty())
		{
			return -1;
		}

		// 清空旧的记录
		videos_.clear();
	}
	//catch(Poco::Exception::)
	catch (Poco::Exception ex)
	{
		errstr = ex.displayText();
		errCode = ex.code();
	}

	

	return errCode;
}