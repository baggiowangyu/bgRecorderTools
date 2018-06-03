#include "bgSmtpProtocol.h"
#include "..\bgBase\bgBase.h"
#include "..\ext\CURL\include\curl\curl.h"



bgSmtpProtocol::bgSmtpProtocol(const std::string & charset /* = "gb2312" */)
: m_strCharset(charset)
{
	m_vRecvMail.clear();
}

bgSmtpProtocol::~bgSmtpProtocol()
{

}

void bgSmtpProtocol::SetSmtpServer(const std::string &username, const std::string& password, const std::string& servername, const std::string &port /* = "25" */)
{
	m_strUserName = username;
	m_strPassword = password;
	m_strServerName = servername;
	m_strPort = port;
}

void bgSmtpProtocol::SetSendName(const std::string& sendname)
{
	std::string strTemp = "";
	strTemp += "=?";
	strTemp += m_strCharset;
	strTemp += "?B?";
	std::string b64val;
	bgBase::Base64Encode((const unsigned char *)sendname.c_str(), sendname.size(), b64val);
	strTemp += b64val;
	strTemp += "?=";
	m_strSendName = strTemp;
}

void bgSmtpProtocol::SetSendMail(const std::string& sendmail)
{
	m_strSendMail = sendmail;
}

void bgSmtpProtocol::AddRecvMail(const std::string& recvmail)
{
	m_vRecvMail.push_back(recvmail);
}

void bgSmtpProtocol::SetSubject(const std::string &subject)
{
	std::string strTemp = "";
	strTemp = "Subject: ";
	strTemp += "=?";
	strTemp += m_strCharset;
	strTemp += "?B?";
	std::string b64val;
	bgBase::Base64Encode((const unsigned char *)subject.c_str(), subject.size(), b64val);
	strTemp += b64val;
	strTemp += "?=";
	m_strSubject = strTemp;
}

void bgSmtpProtocol::AddAttachment(const std::string &filename)
{
	m_vAttachMent.push_back(filename);
}

bool bgSmtpProtocol::SendMail()
{
	CreateMessage();

	bool bret = true;
	CURL *curl = curl_easy_init();
	CURLcode ret = CURLE_OK;
	struct curl_slist *recipients = NULL;

	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_USERNAME, m_strUserName.c_str());
		curl_easy_setopt(curl, CURLOPT_PASSWORD, m_strPassword.c_str());

		std::string smtp_url = "smtp://";
		smtp_url += m_strServerName;
		curl_easy_setopt(curl, CURLOPT_URL, smtp_url.c_str());

#ifdef SKIP_PEER_VERIFICATION
		// 禁用SSL验证
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERIFICATION
		// 禁用SSL主机验证
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif

		curl_easy_setopt(curl, CURLOPT_MAIL_FROM, m_strSendMail.c_str());

		for (int index = 0; index < m_vRecvMail.size(); ++index)
			recipients = curl_slist_append(recipients, m_vRecvMail[index].c_str());

		curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

		std::stringstream stream;
		stream.str(m_strMessage.c_str());
		stream.flush();

		// 设置回调
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, &bgSmtpProtocol::payload_source);
		curl_easy_setopt(curl, CURLOPT_READDATA, (void *)&stream);
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		int times = 0;

		// 发送邮件
		ret = curl_easy_perform(curl);

		CURLINFO curl_info = CURLINFO_NONE;
		curl_easy_getinfo(curl, curl_info);

		while (ret != CURLE_OK)
		{
			++times;
			if (times > 5)
			{
				bret = false;
				break;
			}

			printf("curl_easy_perform() failed : %s\n\n", curl_easy_strerror(ret));
			

			Sleep(1000);
			ret = curl_easy_perform(curl);
		}

		curl_slist_free_all(recipients);
		curl_easy_cleanup(curl);
	}

	return bret;
}

size_t bgSmtpProtocol::payload_source(void *ptr, size_t size, size_t nmemb, void *stream)
{
	size_t num_bytes = size * nmemb;
	char *data = (char *)ptr;
	std::stringstream *strstream = (std::stringstream *)stream;
	strstream->read(data, num_bytes);
	return strstream->gcount();
}

void bgSmtpProtocol::CreateMessage()
{
	m_strMessage += "From: ";
	m_strMessage += m_strSendMail;
	m_strMessage += "\r\n";

	m_strMessage += "Reply-To: ";
	m_strMessage += m_strSendMail;
	m_strMessage += "\r\n";

	m_strMessage += "To: ";
	for (int index = 0; index < m_vRecvMail.size(); ++index)
	{
		if (index > 0)
			m_strMessage += ",";
		m_strMessage += m_vRecvMail[index];
	}
	m_strMessage += "\r\n";

	// 主题
	m_strMessage += m_strSubject;
	m_strMessage += "\r\n";
	m_strMessage += "X-Mailer: The Bat! (v3.02) Professional";
	m_strMessage += "\r\n";
	m_strMessage += "Mime-Version: 1.0";
	m_strMessage += "\r\n";
	m_strMessage += "Content-Type: multipart/mixed;";
	m_strMessage += "boundary=\"simple boundary\"";
	m_strMessage += "\r\n";
	m_strMessage += "This is a multi-part message in MIME format."
	m_strMessage += "\r\n";
	m_strMessage += "--simple boundary";
	m_strMessage += "\r\n";

	// 正文
	m_strMessage += "\r\n";
	m_strMessage += "Content-Type: text/html;";
	m_strMessage += "charset=";
	m_strMessage += "\"";
	m_strMessage += m_strCharset;
	m_strMessage += "\"";
	m_strMessage += "\r\n";
	m_strMessage += "Content-Transfer-Encoding: 7BIT";
	m_strMessage += "\r\n";
	m_strMessage += "\r\n";
	m_strMessage += m_strContent;

	// 附件
	std::string filename = "";
	std::string filetype = "";
	for (int index = 0; index < m_vAttachMent.size(); ++index)
	{
		m_strMessage += "\r\n";
		m_strMessage += "--simple boundary";
		GetFileName(m_vAttachMent[index], filename);
		GetFileType(m_vAttachMent[index], filetype);
		SetContentType(filetype);
		SetFileName(filename);

		m_strMessage += "\r\n";
		m_strMessage += "Content-Type: ";
		m_strMessage += "\t";
		m_strMessage += "name=";
		m_strMessage += "\"";
		m_strMessage += m_strFileName;
		m_strMessage += "\"";
		m_strMessage += "\r\n";
		m_strMessage += "Content-Disposition:attachment;filename=";
		m_strMessage += "\"";
		m_strMessage += m_strFileName;
		m_strMessage += "\"";
		m_strMessage += "\r\n";
		m_strMessage += "Content-Transfer-Encoding:base64";
		m_strMessage += "\r\n";
		m_strMessage += "\r\n";

		FILE *pt = NULL;    
		if ((pt = fopen(m_vAttachMent[index].c_str(), "rb")) == NULL) {    
			std::cerr << "打开文件失败: " << m_vAttachMent[index] <<std::endl;    
			continue;    
		}    
		fseek(pt, 0, SEEK_END);    
		int len = ftell(pt);    
		fseek(pt, 0, SEEK_SET);    
		int rlen = 0;    
		char buf[55];    
		for (size_t i = 0; i < len / 54 + 1; i++)    
		{    
			memset(buf, 0, 55);    
			rlen = fread(buf, sizeof(char), 54, pt);
			std::string b64val;
			bgBase::Base64Encode((const unsigned char*)buf, rlen, b64val);
			m_strMessage += b64val;    
			m_strMessage += "\r\n";    
		}    
		fclose(pt);    
		pt = NULL;    

	}    

	m_strMessage += "\r\n";
	m_strMessage += "--simple boundary--";
	m_strMessage += "\r\n"; 
}

int bgSmtpProtocol::GetFileType(std::string const& stype)
{

}