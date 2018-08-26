#include "bgProtector.h"
#include <Windows.h>

bgProtector::bgProtector()
{

}

bgProtector::~bgProtector()
{

}

int bgProtector::GetComputerCode()
{
	int errCode = 0;

	// 获取CPU型号信息
	std::string processor_name = GetProcessorName();
	std::string processor_identifier = GetProcessorIdentifier();
	std::string processor_vendor_identifier = GetProcessorVendorIdentifier();

	std::string bios_vendor = GetBIOSVendor();
	std::string bios_version = GetBIOSVersion();

	return errCode;
}

int bgProtector::WriteCheckCode(const char *check_code)
{
	int errCode = 0;

	return errCode;
}

int bgProtector::DoCheck()
{
	int errCode = 0;

	return errCode;
}

std::string bgProtector::GetProcessorName()
{
	HKEY hKey = NULL;
	LONG errCode = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey);
	if (errCode != ERROR_SUCCESS)
	{
		return "";
	}

	DWORD dwType = 0;
	DWORD dwDataLen = 4096;
	char data[4096] = {0};
	errCode = RegQueryValueExA(hKey, "ProcessorNameString", NULL, &dwType, (LPBYTE)data, &dwDataLen);
	RegCloseKey(hKey);
	if (errCode != ERROR_SUCCESS)
	{
		return "";
	}

	return data;
}

std::string bgProtector::GetProcessorIdentifier()
{
	HKEY hKey = NULL;
	LONG errCode = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey);
	if (errCode != ERROR_SUCCESS)
	{
		return "";
	}

	DWORD dwType = 0;
	DWORD dwDataLen = 4096;
	char data[4096] = {0};
	errCode = RegQueryValueExA(hKey, "Identifier", NULL, &dwType, (LPBYTE)data, &dwDataLen);
	RegCloseKey(hKey);
	if (errCode != ERROR_SUCCESS)
	{
		return "";
	}

	return data;
}

std::string bgProtector::GetProcessorVendorIdentifier()
{
	HKEY hKey = NULL;
	LONG errCode = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey);
	if (errCode != ERROR_SUCCESS)
	{
		return "";
	}

	DWORD dwType = 0;
	DWORD dwDataLen = 4096;
	char data[4096] = {0};
	errCode = RegQueryValueExA(hKey, "VendorIdentifier", NULL, &dwType, (LPBYTE)data, &dwDataLen);
	RegCloseKey(hKey);
	if (errCode != ERROR_SUCCESS)
	{
		return "";
	}

	return data;
}

std::string bgProtector::GetBIOSVendor()
{
	HKEY hKey = NULL;
	LONG errCode = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\BIOS", 0, KEY_READ, &hKey);
	if (errCode != ERROR_SUCCESS)
	{
		return "";
	}

	DWORD dwType = 0;
	DWORD dwDataLen = 4096;
	char data[4096] = {0};
	errCode = RegQueryValueExA(hKey, "BIOSVendor", NULL, &dwType, (LPBYTE)data, &dwDataLen);
	RegCloseKey(hKey);
	if (errCode != ERROR_SUCCESS)
	{
		return "";
	}

	return data;
}

std::string bgProtector::GetBIOSVersion()
{
	HKEY hKey = NULL;
	LONG errCode = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\BIOS", 0, KEY_READ, &hKey);
	if (errCode != ERROR_SUCCESS)
	{
		return "";
	}

	DWORD dwType = 0;
	DWORD dwDataLen = 4096;
	char data[4096] = {0};
	errCode = RegQueryValueExA(hKey, "BIOSVersion", NULL, &dwType, (LPBYTE)data, &dwDataLen);
	RegCloseKey(hKey);
	if (errCode != ERROR_SUCCESS)
	{
		return "";
	}

	return data;
}

int bgProtector::GetDiskInfo(int &count, struct DiskInfo *info)
{
	int errCode = 0;

	return errCode;
}