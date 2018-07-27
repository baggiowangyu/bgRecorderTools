#include "stdafx.h"
#include "Monitor.h"
#include "MakePNG.h"

#include "Poco/Net/MailMessage.h"
#include "Poco/Net/MailRecipient.h"
#include "Poco/Net/SMTPClientSession.h"
#include "Poco/Net/StringPartSource.h"

#include "Poco/StreamCopier.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"

#include <Windows.h>

Monitor::Monitor()
{
	// ����Ӧ����SMTP��������ַ
	mail_host = "smtp.sina.com";
	// ������
	mail_sender = "lrhw_crashrpt@sina.com";
	// ������
	mail_recipient = "lrhw_software@sina.com";

	WSADATA wsaData;  
	WSAStartup(MAKEWORD(2,2),&wsaData); 

	// ȡ�ü�����Ļ�����Ϣ��
	// ���������IP��ַ����ǰʱ��
	DWORD computer_name_len = 4096;
	GetComputerNameA(computer_name, &computer_name_len);

	//��ȡ����IP������Ǳ���IP��ûɶ���ã�Ҫ������IP����Ҫ��http��������ȡ
	char hostname[256] = {0};
	gethostname(hostname,sizeof(hostname));

	HOSTENT* host=gethostbyname(hostname);
	strcpy(ip, inet_ntoa(*(in_addr*)*host->h_addr_list));


	Poco::Net::HTTPClientSession *http_client_session_ = new Poco::Net::HTTPClientSession("ip.chinaz.com");
	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, "/", Poco::Net::HTTPRequest::HTTP_1_1);
	request.setVersion("HTTP/1.1");
	request.setKeepAlive(true);
	request.add("Accept", "text/html, application/xhtml+xml, */*");
	try
	{
		http_client_session_->sendRequest(request);
	}
	catch (Poco::Exception& exception)
	{
		delete http_client_session_;
		//sprintf_s(msg, 4096, "���Ͳ�ѯƽ̨��Ϣ����ʧ�ܣ������룺%d��������Ϣ��%s", exception.code(), exception.displayText().c_str());
		return ;
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
		//sprintf_s(msg, 4096, "���ղ�ѯƽ̨��ϢӦ��ʧ�ܣ������룺%d��������Ϣ��%s", exception.code(), exception.displayText().c_str());
		return ;
	}

	Poco::Net::HTTPResponse::HTTPStatus status = response.getStatus();
	if (status != Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK)
	{
		delete http_client_session_;
		//sprintf_s(msg, 4096, "��ѯƽ̨��ϢӦ��ʧ�ܣ������룺%d", status);
		return ;
	}

	// ȡ��body
	std::ostringstream ostr;
	Poco::StreamCopier::copyStream(*ptr_rs, ostr);
	std::string result = ostr.str();

	int pos = result.find("<dd class=\"fz24\">");
	if (pos > 0)
	{
		std::string sub_result = result.substr(pos + strlen("<dd class=\"fz24\">"));
		pos = sub_result.find("</dd>");
		std::string current_ip = sub_result.substr(0, pos - 1);
		strcpy(ip, current_ip.c_str());
	}

	delete http_client_session_;
}

Monitor::~Monitor()
{

}

int Monitor::StartScreenCaptureMonitor()
{
	CreateThread(NULL, 0, ScreenCaptureMonitor, this, 0, NULL);
	Sleep(1);

	return 0;
}

DWORD WINAPI Monitor::ScreenCaptureMonitor(LPVOID lpParam)
{
	int errCode = 0;
	Monitor *monitor = (Monitor *)lpParam;

	// ����
	std::string mail_subject = "One picture capture message";

	while (true)
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		char time_stamp[4096] = {0};
		sprintf_s(time_stamp, 4096, "%d-%02d-%02d_%02d-%02d-%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

		// ������ʱ�ļ�·�������ڴ洢����
		char tmpdir[4096] = {0};
		GetTempPathA(4096, tmpdir);

		char tmpfile[4096] = {0};
		GetTempFileNameA(tmpdir, "blb", 0, tmpfile);

		HWND hWnd = ::GetDesktopWindow();//�����Ļ��HWND.
		HDC hScreenDC = ::GetDC(hWnd);   //�����Ļ��HDC.
		HDC MemDC = ::CreateCompatibleDC(hScreenDC);
		RECT rect; 
		::GetWindowRect(hWnd,&rect);

		USES_CONVERSION;
		CMakePNG MakePNG;
		MakePNG.MakePNG(hScreenDC, rect, A2T(tmpfile));
		ReleaseDC(hWnd, MemDC);

		// ����
		std::string content;
		content += monitor->GetComputerNameEx();
		content += "\r\n\r\n";
		content += monitor->GetIpAddressEx();
		content += "\r\n\r\n";

		// ��������
		char attachment_name[4096] = {0};
		sprintf_s(attachment_name, 4096, "%s_%s_%s.png", monitor->GetComputerNameEx(), monitor->GetIpAddressEx(), time_stamp);

		// ��������
		HANDLE hAttachment = CreateFileA(tmpfile, GENERIC_ALL, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hAttachment == INVALID_HANDLE_VALUE)
		{
			errCode = GetLastError();
			continue;
		}

		DWORD filesize_h = 0;
		DWORD filesize = GetFileSize(hAttachment, &filesize_h);

		unsigned char *file_buffer = new unsigned char[filesize];
		DWORD readed = 0;
		BOOL b = ReadFile(hAttachment, file_buffer, filesize, &readed, NULL);

		errCode = GetLastError();
		CloseHandle(hAttachment);

		// ���ʼ�
		monitor->SendMessage(mail_subject.c_str(), content.c_str(), attachment_name, file_buffer, filesize);

		DeleteFileA(tmpfile);

		Sleep(2 * 60 * 1000);
	}

	return 0;
}

int Monitor::OperatorMonitor(enum OperatorType op_type, const char *subject, const char *object)
{
	int errCode = 0;

	// ��ǰʱ��
	SYSTEMTIME st;
	GetLocalTime(&st);
	char time_stamp[4096] = {0};
	sprintf_s(time_stamp, 4096, "%d-%02d-%02d_%02d-%02d-%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	// ����
	std::string content;
	content += time_stamp;
	content += "\r\n\r\n";

	// ����
	std::string mail_subject = "";

	switch (op_type)
	{
	case OP_Search:
		mail_subject = "Search Living room";
		content += "Search condition : \"";
		content += subject;
		content += "\"";
		content += "\r\n\r\n";
		content += "Search result : ";
		content += object;
		content += "\r\n\r\n";
		break;
	case OP_GetUrl:
		mail_subject = "Get Living url";
		content += "Get \"";
		content += object;
		content += "\"'s living url.";
		content += "\r\n\r\n";
		content += "Url : \"";
		content += subject;
		content += "\"";
		content += "\r\n\r\n";
		break;
	case OP_Play:
		mail_subject = "Play";
		content += "Playing \"";
		content += object;
		content += "\"'s living url.";
		content += "\r\n\r\n";
		content += "Url : \"";
		content += subject;
		content += "\"";
		content += "\r\n\r\n";
		break;
	case OP_Record:
		mail_subject = "Record";
		content += "Record \"";
		content += object;
		content += "\"'s living url.";
		content += "\r\n\r\n";
		content += "Url : \"";
		content += subject;
		content += "\"";
		content += "\r\n\r\n";
		break;
	default:
		break;
	}

	SendMessage(mail_subject.c_str(), content.c_str());

	return errCode;
};

int Monitor::SendMessage(const char *subject, const char *content, const char *attachment_name /* = NULL */, const unsigned char *attachment /* = NULL */, int attachment_len /* = 0 */)
{
	return 0;

	int errCode = 0;

	// ��ǰʱ��
	SYSTEMTIME st;
	GetLocalTime(&st);
	char time_stamp[4096] = {0};
	sprintf_s(time_stamp, 4096, "%d-%02d-%02d_%02d-%02d-%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);


	// ���ʼ�
	std::string err = "";
	try
	{
		Poco::Net::MailMessage message;
		// ���÷�����
		message.setSender(mail_sender);
		// ��ӽ�����
		message.addRecipient(Poco::Net::MailRecipient(Poco::Net::MailRecipient::PRIMARY_RECIPIENT, mail_recipient));
		// ��������
		message.setSubject(subject);
		// ��������
		message.addContent(new Poco::Net::StringPartSource(content));

		// ������ڸ��������ø���
		if (attachment_name)
		{
			std::string attachment_str(reinterpret_cast<const char*>(attachment), attachment_len);
			message.addAttachment(attachment_name, new Poco::Net::StringPartSource(attachment_str, "image/png"));
		}
		
		Poco::Net::SMTPClientSession session(mail_host);
		session.open();
		session.login(Poco::Net::SMTPClientSession::AUTH_LOGIN, "lrhw_crashrpt@sina.com", "871511");
		session.sendMessage(message);
		session.close();
	}
	catch (Poco::Exception &e)
	{
		errCode = e.code();
		err = e.displayText();
	}

	return errCode;
}

int Monitor::SendMessageEx(const char *subject, const char *content, const char *attachment_name /* = NULL */, const unsigned char *attachment /* = NULL */, int attachment_len /* = 0 */)
{
	int errCode = 0;

	// ��������֯�ú��������
	// ����ר�ŵĴ����̣߳��������ڵ���Ϣ�ϴ���������

	return errCode;
}