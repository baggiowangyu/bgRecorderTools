#include "stdafx.h"
#include "bgLiveBoxBusiness.h"
#include "curl/curl.h"

//#define USE_OTHER
//#define USE_QIUMINGSHAN

#ifdef USE_QIUMINGSHAN
#define ROOT_URL	"qm3.top"
#endif

#ifdef USE_OTHER
#define ROOT_URL	"api.hclyz.cn"
#endif

#ifdef USE_XIAOJIEJIE
#define ROOT_URL	"xxjj.live"
#endif



bgLiveBoxBusiness::bgLiveBoxBusiness(bgLiveBoxBusinessObserver *observer)
: observer_(observer)
{
	
	
}

bgLiveBoxBusiness::~bgLiveBoxBusiness()
{

}

int bgLiveBoxBusiness::UpdateApps()
{
	char msg[4096] = {0};
	int errCode = 0;
	observer_->StateNotify("����ˢ��ƽ̨��Ϣ...");

#ifdef USE_QIUMINGSHAN
	Poco::Net::HTTPClientSession *http_client_session_ = new Poco::Net::HTTPClientSession(ROOT_URL);
#endif

#ifdef USE_OTHER
	Poco::Net::HTTPClientSession *http_client_session_ = new Poco::Net::HTTPClientSession(ROOT_URL, 81);
#endif

#ifdef USE_XIAOJIEJIE
	Poco::Net::HTTPClientSession *http_client_session_ = new Poco::Net::HTTPClientSession(ROOT_URL);
#endif

	Poco::Timespan timeout_span(50, 0);
	http_client_session_->setTimeout(timeout_span);

#ifdef USE_QIUMINGSHAN
	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, "/wp-content/themes/qm3/zb/load.php", Poco::Net::HTTPRequest::HTTP_1_1);
	request.setVersion("HTTP/1.1");
	request.setKeepAlive(true);
	request.setHost(ROOT_URL, 80);
	request.add("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
	request.add("Accept", "*/*");
	request.add("Origin", "http://zb.qm3.top");
	request.add("X-Requested-With", "XMLHttpRequest");
	request.add("User-Agent", "Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/67.0.3396.99 Safari/537.36");
	request.add("Referer", "http://zb.qm3.top/");
	request.add("Accept-Language", "zh-CN,zh;q=0.9");
	request.add("Cookie", "Hm_lvt_4f65099a014207c7b35752793e197904=1531720093; Hm_lpvt_4f65099a014207c7b35752793e197904=1531725338");

	char body[4096] = {0};
	sprintf_s(body, 4096, "a=all");

	std::string http_body(body);
	request.setContentLength((int)http_body.length());

	try
	{
		http_client_session_->sendRequest(request) << http_body;
	}
#endif

#ifdef USE_OTHER
	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, "/mf/json.txt", Poco::Net::HTTPRequest::HTTP_1_1);
	request.setVersion("HTTP/1.1");
	request.setKeepAlive(true);
	request.setHost(ROOT_URL, 81);
	request.add("Accept", "text/html, application/xhtml+xml, */*");
	request.add("Content-Type", "application/json");

	try
	{
		http_client_session_->sendRequest(request);
	}

#endif

#ifdef USE_XIAOJIEJIE
	// һ��
#endif

	catch (Poco::Exception& exception)
	{
		delete http_client_session_;
		sprintf_s(msg, 4096, "���Ͳ�ѯƽ̨��Ϣ����ʧ�ܣ������룺%d��������Ϣ��%s", exception.code(), exception.displayText().c_str());
		observer_->StateNotify(msg);
		return exception.code();
	}

	Poco::Net::HTTPResponse response;
	std::istream *ptr_rs = NULL;
	try
	{
		std::istream &rs = http_client_session_->receiveResponse(response);
		ptr_rs = &rs;
	}
	catch (Poco::Exception& exception)
	{
		delete http_client_session_;
		sprintf_s(msg, 4096, "���ղ�ѯƽ̨��ϢӦ��ʧ�ܣ������룺%d��������Ϣ��%s", exception.code(), exception.displayText().c_str());
		observer_->StateNotify(msg);
		return exception.code();
	}
	
	Poco::Net::HTTPResponse::HTTPStatus status = response.getStatus();
	if (status != Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK)
	{
		delete http_client_session_;
		sprintf_s(msg, 4096, "��ѯƽ̨��ϢӦ��ʧ�ܣ������룺%d", status);
		observer_->StateNotify(msg);
		return status;
	}

	//std::string encoding = response.getTransferEncoding();

	// ȡ��body
	std::ostringstream ostr;
	Poco::StreamCopier::copyStream(*ptr_rs, ostr);
	std::string result = ostr.str();
	if (result.empty())
		return errCode;

	if (result.compare("null") == 0)
		return errCode;

	int pos = result.find("���");
	if (pos >= 0)
		return errCode;

	//OutputDebugStringA(result.c_str());

	// תΪJson
	Poco::JSON::Parser parser;
	Poco::Dynamic::Var json = parser.parse(result);
	Poco::JSON::Object::Ptr jsonObject = json.extract<Poco::JSON::Object::Ptr>();

#ifdef USE_QIUMINGSHAN
	// ����Ľ��Ӧ�ó���һ��xml�ܲ��ܽ��������ﻹҪ����һ���Ƿ���Ҫ������ת��
	Poco::Dynamic::Var pt = jsonObject->get("pt");
	std::string pt_str = pt.toString();

	// ������һ�±���ת��
	int pt_str_unicode_size = pt_str.size() + 1;
	wchar_t *pt_str_unicode = new wchar_t[pt_str_unicode_size];
	ZeroMemory(pt_str_unicode, pt_str_unicode_size * sizeof(wchar_t));

	errCode = MultiByteToWideChar(CP_UTF8, 0, pt_str.c_str(), -1, pt_str_unicode, pt_str_unicode_size);
	errCode = GetLastError();

	int pt_str_ansi_size = pt_str_unicode_size;
	char *pt_str_ansi = new char[pt_str_ansi_size];
	ZeroMemory(pt_str_ansi, pt_str_ansi_size * sizeof(char));

	errCode = WideCharToMultiByte(CP_ACP, 0, pt_str_unicode, -1, pt_str_ansi, pt_str_ansi_size, NULL, NULL);
	errCode = GetLastError();

	// ʵ�ڲ��У������Ǿ�ֻ���˹�������
	std::string pt_data = pt_str_ansi;

	delete [] pt_str_unicode;
	pt_str_unicode = NULL;
	delete [] pt_str_ansi;
	pt_str_ansi = NULL;

	apps.clear();
	while (true)
	{
		// �ҵ�����Ԫ�ؽ�β
		int pos = pt_data.find("</li>");
		if (pos < 0)
			break;
		
		// ��ȡԪ��
		std::string pt_element = pt_data.substr(0, pos + strlen("</li>"));

		// ��ȡid
		int id_pre_pos = pt_element.find("id='");
		int id_post_pos = pt_element.find("' class=");
		int id_len = id_post_pos - id_pre_pos - strlen("id='");
		std::string pt_id = pt_element.substr(id_pre_pos + strlen("id='"), id_len);

		//// ��ȡͼƬurl
		//int url_pre_pos = pt_element.find("<img src='");
		//int url_post_pos = pt_element.find("'><p>");
		//int url_len = url_post_pos - url_pre_pos - strlen("<img src='");
		//std::string pt_url = pt_element.substr(url_pre_pos + strlen("<img src='"), url_len);

		// ��ȡ����
		int title_pre_pos = pt_element.find("<p>");
		int title_post_pos = pt_element.find("</p>");
		int title_len = title_post_pos - title_pre_pos - strlen("<p>");
		std::string pt_title = pt_element.substr(title_pre_pos + strlen("<p>"), title_len);

		struct app_info info;
		info.app_id = pt_id;
		//info.app_img_url = pt_url;
		info.app_name = pt_title;
		apps.push_back(info);

		pt_data = pt_data.substr(pos + strlen("</li>"));
	}
#endif

#ifdef USE_OTHER
	Poco::JSON::Array::Ptr pt = jsonObject->getArray("pingtai");
	int pt_count = pt->size();
	apps.clear();
	for (int index = 0; index < pt_count; ++index)
	{
		// 
		Poco::Dynamic::Var element = pt->get(index);
		Poco::JSON::Object::Ptr elementObject = element.extract<Poco::JSON::Object::Ptr>();

		struct app_info info;
		info.app_id = elementObject->get("address").toString();
		info.app_img_url = elementObject->get("xinimg").toString();

		// ����Ҫ����ת��һ��
		int pt_str_unicode_size = elementObject->get("title").toString().size() + 1;
		wchar_t *pt_str_unicode = new wchar_t[pt_str_unicode_size];
		ZeroMemory(pt_str_unicode, pt_str_unicode_size * sizeof(wchar_t));

		errCode = MultiByteToWideChar(CP_UTF8, 0, elementObject->get("title").toString().c_str(), -1, pt_str_unicode, pt_str_unicode_size);
		errCode = GetLastError();

		int pt_str_ansi_size = pt_str_unicode_size;
		char *pt_str_ansi = new char[pt_str_ansi_size];
		ZeroMemory(pt_str_ansi, pt_str_ansi_size * sizeof(char));

		errCode = WideCharToMultiByte(CP_ACP, 0, pt_str_unicode, -1, pt_str_ansi, pt_str_ansi_size, NULL, NULL);
		errCode = GetLastError();

		char n[4096] = {0};
		sprintf_s(n, 4096, "%s(%s)", pt_str_ansi, elementObject->get("Number").toString().c_str());
		info.app_name = n;

		apps.push_back(info);

		delete [] pt_str_unicode;
		pt_str_unicode = NULL;
		delete [] pt_str_ansi;
		pt_str_ansi = NULL;
	}
#endif

	// ƽ̨�������
	delete http_client_session_;

	// ֪ͨ�����������
	observer_->AppUpdated();
	observer_->StateNotify("ƽ̨��Ϣˢ�����...");
	return errCode;
}

int bgLiveBoxBusiness::UpdateRooms(const char *app_id, const char *app_name)
{
	char msg[4096] = {0};
	int errCode = 0;
	observer_->StateNotify("����ˢ�·�����Ϣ...");

#ifdef USE_QIUMINGSHAN
	Poco::Net::HTTPClientSession *http_client_session_ = new Poco::Net::HTTPClientSession(ROOT_URL);
	Poco::Timespan timeout_span(50, 0);
	http_client_session_->setTimeout(timeout_span);
	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, "/wp-content/themes/qm3/zb/int.php", Poco::Net::HTTPRequest::HTTP_1_1);
	request.setVersion("HTTP/1.1");
	request.setKeepAlive(true);
	request.setHost(ROOT_URL, 80);
	request.add("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
	request.add("Accept", "*/*");
	request.add("Origin", "http://zb.qm3.top");
	request.add("X-Requested-With", "XMLHttpRequest");
	request.add("User-Agent", "Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/67.0.3396.99 Safari/537.36");
	request.add("Referer", "http://zb.qm3.top/");
	request.add("Accept-Language", "zh-CN,zh;q=0.9");
	request.add("Cookie", "Hm_lvt_4f65099a014207c7b35752793e197904=1531720093; Hm_lpvt_4f65099a014207c7b35752793e197904=1531725338");

	char body[4096] = {0};
	sprintf_s(body, 4096, "n=%s&a=all", app_id);

	std::string http_body(body);
	request.setContentLength((int)http_body.length());

	try
	{
		http_client_session_->sendRequest(request) << http_body;
	}
#endif

#ifdef USE_OTHER
	Poco::Net::HTTPClientSession *http_client_session_ = new Poco::Net::HTTPClientSession(ROOT_URL, 81);
	Poco::Timespan timeout_span(50, 0);
	http_client_session_->setTimeout(timeout_span);

	char object[4096] = {0};
	sprintf_s(object, 4096, "/mf/%s", app_id);
	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, object, Poco::Net::HTTPRequest::HTTP_1_1);
	request.setVersion("HTTP/1.1");
	request.setKeepAlive(true);
	request.setHost(ROOT_URL, 81);
	request.add("Accept", "text/html, application/xhtml+xml, */*");
	request.add("Content-Type", "application/json");

	try
	{
		http_client_session_->sendRequest(request);
	}
#endif
	catch (Poco::Exception& exception)
	{
		delete http_client_session_;
		sprintf_s(msg, 4096, "���Ͳ�ѯ������Ϣ����ʧ�ܣ������룺%d��������Ϣ��%s", exception.code(), exception.displayText().c_str());
		observer_->StateNotify(msg);
		return exception.code();
	}

	Poco::Net::HTTPResponse response;
	std::istream *ptr_rs = NULL;
	try
	{
		std::istream &rs = http_client_session_->receiveResponse(response);
		ptr_rs = &rs;
	}
	catch (Poco::Exception& exception)
	{
		delete http_client_session_;
		sprintf_s(msg, 4096, "���ղ�ѯ������ϢӦ��ʧ�ܣ������룺%d��������Ϣ��%s", exception.code(), exception.displayText().c_str());
		observer_->StateNotify(msg);
		return exception.code();
	}
	
	Poco::Net::HTTPResponse::HTTPStatus status = response.getStatus();
	if (status != Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK)
	{
		delete http_client_session_;
		sprintf_s(msg, 4096, "��ѯ������ϢӦ��ʧ�ܣ������룺%d", status);
		observer_->StateNotify(msg);
		return status;
	}

	// ȡ��body
	std::ostringstream ostr;
	Poco::StreamCopier::copyStream(*ptr_rs, ostr);
	std::string result = ostr.str();
	if (result.empty())
		return errCode;

	if (result.compare("null") == 0)
		return errCode;

	int pos = result.find("���");
	if (pos >= 0)
		return errCode;

	//OutputDebugStringA(result.c_str());

#ifdef _HANDLE_DOT_
	//// ���ﴦ��һ�¶���Ķ��ŵ����
	//// Ѱ�����һ������
	//pos = result.find_last_of(",");
	//if (pos > 0)
	//{
	//	std::string head = result.substr(0, pos);
	//	std::string tail = result.substr(pos + 1, -1);
	//	result = head + tail;
	//}
#endif

	// תΪJson
	Poco::JSON::Parser parser;
	Poco::Dynamic::Var json = parser.parse(result);
	Poco::JSON::Object::Ptr jsonObject = json.extract<Poco::JSON::Object::Ptr>();

#ifdef USE_QIUMINGSHAN
	Poco::JSON::Array::Ptr urlArray = jsonObject->getArray("url");

	// ����Ľ��Ӧ�ó���һ��xml�ܲ��ܽ��������ﻹҪ����һ���Ƿ���Ҫ������ת��
	Poco::Dynamic::Var pt = jsonObject->get("zb");
	std::string zb_str = pt.toString();

	// ������һ�±���ת��
	int zb_str_unicode_size = zb_str.size() + 1;
	wchar_t *zb_str_unicode = new wchar_t[zb_str_unicode_size];
	ZeroMemory(zb_str_unicode, zb_str_unicode_size * sizeof(wchar_t));

	MultiByteToWideChar(CP_UTF8, 0, zb_str.c_str(), -1, zb_str_unicode, zb_str_unicode_size);

	int zb_str_ansi_size = zb_str_unicode_size;
	char *zb_str_ansi = new char[zb_str_ansi_size];
	ZeroMemory(zb_str_ansi, zb_str_ansi_size * sizeof(char));

	WideCharToMultiByte(CP_ACP, 0, zb_str_unicode, -1, zb_str_ansi, zb_str_ansi_size, NULL, NULL);

	// ʵ�ڲ��У������Ǿ�ֻ���˹�������
	std::string zb_data = zb_str_ansi;

	delete [] zb_str_unicode;
	zb_str_unicode = NULL;
	delete [] zb_str_ansi;
	zb_str_ansi = NULL;

	current_rooms.clear();
	int index = 0;
	while (true)
	{
		// �ҵ�����Ԫ�ؽ�β
		int pos = zb_data.find("</li>");
		if (pos < 0)
			break;

		// ��ȡԪ��
		std::string zb_element = zb_data.substr(0, pos + strlen("</li>"));

		// ��ȡ���ŵ�ַ
		std::string zb_url = urlArray->get(index).toString();

		// ��ȡͼƬurl
		int url_pre_pos = zb_element.find("data-original='");
		int url_post_pos = zb_element.find("'></p>");
		int url_len = url_post_pos - url_pre_pos - strlen("data-original='");
		std::string zb_image_url = zb_element.substr(url_pre_pos + strlen("data-original='"), url_len);

		// ��ȡ����
		int title_pre_pos = zb_element.find("<p class='title'>");
		int title_post_pos = zb_element.find("</p></li>");
		int title_len = title_post_pos - title_pre_pos - strlen("<p class='title'>");
		std::string zb_title = zb_element.substr(title_pre_pos + strlen("<p class='title'>"), title_len);

		struct room_info info;
		info.img_url = zb_image_url;
		info.index = index;
		info.name = zb_title;
		info.live_url = zb_url;
		current_rooms.push_back(info);

		++index;

		zb_data = zb_data.substr(pos + strlen("</li>"));
	}
#endif

#ifdef USE_OTHER
	Poco::JSON::Array::Ptr pt = jsonObject->getArray("zhubo");
	int pt_count = pt->size();
	current_rooms.clear();
	for (int index = 0; index < pt_count; ++index)
	{
		// 
		Poco::Dynamic::Var element = pt->get(index);
		Poco::JSON::Object::Ptr elementObject = element.extract<Poco::JSON::Object::Ptr>();

		struct room_info info;
		info.img_url = elementObject->get("img").toString();
		info.index = index;
		info.live_url = elementObject->get("address").toString();

		// ����Ҫ����ת��һ��
		int pt_str_unicode_size = elementObject->get("title").toString().size() + 1;
		wchar_t *pt_str_unicode = new wchar_t[pt_str_unicode_size];
		ZeroMemory(pt_str_unicode, pt_str_unicode_size * sizeof(wchar_t));

		errCode = MultiByteToWideChar(CP_UTF8, 0, elementObject->get("title").toString().c_str(), -1, pt_str_unicode, pt_str_unicode_size);
		errCode = GetLastError();

		int pt_str_ansi_size = pt_str_unicode_size;
		char *pt_str_ansi = new char[pt_str_ansi_size];
		ZeroMemory(pt_str_ansi, pt_str_ansi_size * sizeof(char));

		errCode = WideCharToMultiByte(CP_ACP, 0, pt_str_unicode, -1, pt_str_ansi, pt_str_ansi_size, NULL, NULL);
		errCode = GetLastError();

		info.name = pt_str_ansi;

		current_rooms.push_back(info);

		delete [] pt_str_unicode;
		pt_str_unicode = NULL;
		delete [] pt_str_ansi;
		pt_str_ansi = NULL;
	}
#endif

	delete http_client_session_;

	observer_->RoomUpdate(app_id, app_name);
	observer_->StateNotify("������Ϣˢ�����...");
	return errCode;
}