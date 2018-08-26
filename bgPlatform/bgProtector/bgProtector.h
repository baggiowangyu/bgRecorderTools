#ifndef _bgProtector_H_
#define _bgProtector_H_

#include <string>

/**
 * У��ԭ��
 * У������ɲ��֣�[20�ֽ������] + [����ʱ��] + [cpu��Ϣ] + [������Ϣ] + [�ڴ�����]
 *
 * �û���һ��������ʱ��ʹ���Լ����˺ţ���������������Ϣ���ܺ����ҵĹ�Կ
 */

struct DiskInfo
{
	std::string serial_number_;
	std::string model_number_;
	std::string revision_number_;
};

class bgProtector
{
public:
	bgProtector();
	~bgProtector();

public:
	/**
	 * ���������
	 */
	int GetComputerCode();

	/**
	 * д��У���룬����ע�����
	 */
	int WriteCheckCode(const char *check_code);

	/**
	 * ����У�飬�ñ�����ע����У���룬���ܣ��Ƚϻ�����Ϣ��ͨ����Ƚ���Ч�ڣ���Ч�ڲ���ȷ�ģ�Ҫ����������Ч��
	 */
	int DoCheck();

private:
	std::string GetProcessorName();
	std::string GetProcessorIdentifier();
	std::string GetProcessorVendorIdentifier();

	std::string GetBIOSVendor();
	std::string GetBIOSVersion();

	int GetDiskInfo(int &count, struct DiskInfo *info);

private:
	
};

#endif
