#ifndef _bgProtector_H_
#define _bgProtector_H_

#include <string>

/**
 * 校验原理
 * 校验码组成部分：[20字节随机数] + [到期时间] + [cpu信息] + [磁盘信息] + [内存总数]
 *
 * 用户第一次启动的时候，使用自己的账号，将本机的物理信息加密后，用我的公钥
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
	 * 计算机器码
	 */
	int GetComputerCode();

	/**
	 * 写入校验码，存在注册表中
	 */
	int WriteCheckCode(const char *check_code);

	/**
	 * 启动校验，拿保存在注册表的校验码，解密，比较机器信息，通过后比较有效期，有效期不正确的，要重新输入有效期
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
