#ifndef _BG_SNIFFER_H_
#define _BG_SNIFFER_H_

#include <string>
#include <vector>
#include "bgBasePacketParse.h"



typedef struct _NETWORK_DEVICE_DESC_
{
	std::string name_;		// ����
	std::string descript_;	// ����
	std::string net_ip_;	// ����IP
	unsigned int mask_ip_;	// ��������
	std::string sys_name_;	// ��ϵͳ��չʾ������
} NETWORK_DEVICE_DESC, *PNETWORK_DEVICE_DESC;

class bgSniffer
{
public:
	bgSniffer();
	~bgSniffer();

public:
	// ö�����������豸
	int EnumAllNetworkDevices(int &count, PNETWORK_DEVICE_DESC device_descs);

	// ������
	int OpenNetworkDevice(const char *device_name, unsigned int mask_ip = 0);

	// �ر�����
	void CloseNetworkDevice();

public:
	// ���ؽ�����
	int LoadDataParsePlugins(bgBasePacketParse *plugin);

	int DispatchPacket(const unsigned char *data, int len);

public:
	static DWORD WINAPI WorkingThread(LPVOID lpParam);

public:
	void *adapter_handle_;
	std::vector<bgBasePacketParse*> plugins_;
	bool thread_exit_;
	HANDLE thread_exited_;
};

#endif