#include "Scanner.h"
#include "Poco/StreamCopier.h"
#include "Poco/UTF8Encoding.h"
#include "Poco/ASCIIEncoding.h"
#include "Poco/TextConverter.h"
#include "Poco/Net/MediaType.h"
#include "Poco/JSON/JSON.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/Statement.h"
#include "Poco/Exception.h"
#include <istream>
#include <ostream>
#include <sstream>


Scanner::Scanner()
: http_client_session_(new Poco::Net::HTTPClientSession(ROOT_URL))
{
	Poco::Data::SQLite::Connector::registerConnector();

	Poco::Timespan timeout_span(50, 0);
	http_client_session_->setTimeout(timeout_span);
}

Scanner::~Scanner()
{
	Poco::Data::SQLite::Connector::unregisterConnector();
}

int Scanner::Connect()
{
	// ������ҳ����ȡ��һ��Cookie
	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, "/");
	request.add("Host", ROOT_URL);
	request.add("Connection", "keep-alive");
	request.add("Accept", "application/json, text/javascript, */*; q=0.01");
	request.add("Origin", "http://www.91gxfl.com");
	request.add("X-Requested-With", "XMLHttpRequest");
	request.add("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/66.0.3359.181 Safari/537.36");
	request.add("Referer", "http://www.91gxfl.com");
	request.add("Accept-Language", "zh-CN,zh;q=0.9");
	http_client_session_->sendRequest(request);

	Poco::Net::HTTPResponse response;
	std::istream& rs = http_client_session_->receiveResponse(response);

	Poco::Net::HTTPResponse::HTTPStatus status = response.getStatus();
	if (status != Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK)
		return status;
	
	response.getCookies(http_cookies_);

	// ȡ��body
	std::ostringstream ostr;
	Poco::StreamCopier::copyStream(rs, ostr);
	//std::cout<<ostr.str()<<std::endl;
	return status;
}

int Scanner::Login(const char *username, const char *password)
{
	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, "/api/login.html");

	request.add("Host", ROOT_URL);
	request.add("Connection", "keep-alive");
	request.add("Accept", "application/json, text/javascript, */*; q=0.01");
	request.add("Origin", "http://www.91gxfl.com");
	request.add("X-Requested-With", "XMLHttpRequest");
	request.add("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/66.0.3359.181 Safari/537.36");
	request.add("Referer", "http://www.91gxfl.com");
	request.add("Accept-Language", "zh-CN,zh;q=0.9");
	

	// ����Cookie
	std::vector<Poco::Net::HTTPCookie>::iterator iter;
	for (iter = http_cookies_.begin(); iter != http_cookies_.end(); ++iter)
	{
		Poco::Net::NameValueCollection cookie;
		cookie.add(iter->getName(), iter->getValue());
		request.setCookies(cookie);
	}

	char body[4096] = {0};
	sprintf_s(body, 4096, "userName=%s&password=%s", username, password);
	std::string http_body(body);
	request.setContentLength((int)http_body.length());

	http_client_session_->sendRequest(request) << http_body;

	Poco::Net::HTTPResponse response;
	std::istream& rs = http_client_session_->receiveResponse(response);

	Poco::Net::HTTPResponse::HTTPStatus status = response.getStatus();
	if (status != Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK)
		return status;

	response.getCookies(http_cookies_);

	std::ostringstream ostr;
	Poco::StreamCopier::copyStream(rs, ostr);
	//std::cout<<ostr.str()<<std::endl;

	return status;
}

int Scanner::DoScan()
{
	int errCode = 0;

	// ��ʼ�����ݿ�Ự
	Poco::Data::Session *session = new Poco::Data::Session("SQLite", "data.db3");
	Poco::Data::Statement insert_(*session);
	Poco::Data::Statement select_(*session);

	// �Ȳ�ѯ�Ѿ���������һ��  select * from data order by myindex desc limit 0,1;
	std::vector<std::string> indexs;
	select_ << "select id from data order by myindex desc limit 0,1", Poco::Data::Keywords::into(indexs), Poco::Data::Keywords::now;
	select_.reset(*session);

	int begin_index = indexs.size() == 0 ? 1810 : atoi(indexs[0].c_str()) + 1;
	int max_index = 15000;

	// ������Կ������������ִ�У�����������������

	for (int index = begin_index; index < max_index; ++index)
	{

SEND_REQUEST:
		char body[4096] = {0};
		sprintf_s(body, 4096, "id=%d&surePlay=1&isTrySee=false", index);

		char referrer[4096] = {0};
		sprintf_s(referrer, 4096, "http://www.2018gxfl.com/video/play/id/%d.html", index);

		Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, "/api/payvideo.html");
		request.setVersion("HTTP/1.1");
		request.setKeepAlive(true);
		request.setHost(ROOT_URL, 80);
		request.add("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
		request.add("Accept", "application/json, text/javascript, */*; q=0.01");
		request.add("Origin", "http://www.2018gxfl.com");
		request.add("X-Requested-With", "XMLHttpRequest");
		request.add("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/66.0.3359.181 Safari/537.36");
		request.add("Referer", referrer);
		request.add("Accept-Language", "zh-CN,zh;q=0.9");
		//request.add("Accept-Encoding", "gzip, deflate");
		request.add("Cookie", "__cfduid=d4127cc07c4b6bddb5d34fd276032e0b41526484179; UM_distinctid=163698c14c6c98-02c67dba2d55c8-f373567-1fa400-163698c14c729; PHPSESSID=f2ds0o0hta6r4hkps4u01155b6");

		std::string http_body(body);
		request.setContentLength((int)http_body.length());

		//// ����Cookie
		//std::vector<Poco::Net::HTTPCookie>::iterator iter;
		//for (iter = http_cookies_.begin(); iter != http_cookies_.end(); ++iter)
		//{
		//	Poco::Net::NameValueCollection cookie;
		//	cookie.add(iter->getName(), iter->getValue());
		//	request.setCookies(cookie);
		//}


		std::cout<<"��ʼ������Ƶ >>> ID��"<<index<<std::endl;
		DWORD tick_start = GetTickCount();

		try
		{
			http_client_session_->sendRequest(request) << http_body;
		}
		catch (Poco::Exception& exception)
		{
			DWORD tick_stop = GetTickCount();
			DWORD tick_span = tick_stop - tick_start;

			std::string msg = exception.message();
			std::cout<<"�����쳣��";
			if (exception.code() == WSAETIMEDOUT)
			{
				// ��ʱ�ˣ������Ǿ�����һ��
				std::cout<<"����ʱ����ʱ��"<<tick_span<<"���롣����..."<<std::endl;
				errCode = WSAETIMEDOUT;
				break;
			}
			else
			{
				std::cout<<"�쳣��Ϊ"<<exception.code()<<"����ʱ��"<<tick_span<<"���롣����..."<<std::endl;
			}
			continue;
		}
		

		//
		//
		// ����Ҫ�����쳣
		Poco::Net::HTTPResponse response;
		std::istream *ptr_rs = NULL;
		try
		{
			std::istream &rs = http_client_session_->receiveResponse(response);
			ptr_rs = &rs;
		}
		catch (Poco::Exception& exception)
		{
			DWORD tick_stop = GetTickCount();
			DWORD tick_span = tick_stop - tick_start;

			std::string msg = exception.message();
			std::cout<<"�����쳣��";
			if (exception.code() == WSAETIMEDOUT)
			{
				// ��ʱ�ˣ������Ǿ�����һ��
				std::cout<<"����ʱ����ʱ��"<<tick_span<<"���롣����..."<<std::endl;
				errCode = WSAETIMEDOUT;
				break;
			}
			else
			{
				std::cout<<"�쳣��Ϊ"<<exception.code()<<"����ʱ��"<<tick_span<<"���롣����..."<<std::endl;
			}
			continue;
		}
		

		Poco::Net::HTTPResponse::HTTPStatus status = response.getStatus();
		if (status != Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK)
		{
			std::cout<<"���������أ�"<<status<<std::endl;
			continue;
		}

		//std::string encoding = response.getTransferEncoding();

		// ȡ��body
		std::ostringstream ostr;
		Poco::StreamCopier::copyStream(*ptr_rs, ostr);

		DWORD tick_stop = GetTickCount();
		DWORD tick_span = tick_stop - tick_start;

		// תΪJson
		Poco::JSON::Parser parser;
		Poco::Dynamic::Var json = parser.parse(ostr.str());
		Poco::JSON::Object::Ptr jsonObject = json.extract<Poco::JSON::Object::Ptr>();

		Poco::Dynamic::Var resultCode = jsonObject->get("resultCode");
		int code = resultCode.convert<int>();

		if (code == 0)
		{
			std::cout<<"�����ɹ�����ʼ���...������ʱ��"<<tick_span<<"����"<<std::endl;

			// ����dataĿ¼�����õ������videoinfoĿ¼���õ�title��url
			Poco::Dynamic::Var data = jsonObject->get("data");
			Poco::JSON::Object::Ptr ptr_data = data.extract<Poco::JSON::Object::Ptr>();
			Poco::Dynamic::Var videoinfo = ptr_data->get("videoInfo");
			Poco::JSON::Object::Ptr ptr_videoinfo = videoinfo.extract<Poco::JSON::Object::Ptr>();

			Poco::Dynamic::Var title = ptr_videoinfo->get("title");
			Poco::Dynamic::Var url = ptr_videoinfo->get("url");

			std::string real_title = title.toString();
			std::string real_url = url.toString();

			// �ɹ�ת���ɺ���
			wchar_t tmp[4096] = {0};
			MultiByteToWideChar(CP_UTF8, 0, real_title.c_str(), -1, tmp, 4096);

			//std::cout<<real_url.c_str()<<std::endl;

			// ���ݿ����
			try
			{
				insert_ << "INSERT INTO data(id, name, url, is_download) VALUES(\"%d\", \"%s\", \"%s\", 0)", index, real_title.c_str(), real_url.c_str();
				size_t res_line = insert_.execute();

				if (res_line == 1)
					std::cout<<"������..."<<std::endl;
				else
					std::cout<<"���ʧ��..."<<std::endl;

				insert_.reset(*session);
			}
			catch (Poco::Exception& exception)
			{
				std::string msg = exception.message();
				int err = exception.code();
			}
		}
		else
		{
			std::cout<<"����ʧ�ܣ�������ʱ��"<<tick_span<<"����"<<std::endl;
		}
	}

	delete session;
	session = NULL;
	
	return errCode;
}