// Scanner_91gxfl.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>

#include "Scanner.h"
#include "Downloader.h"

#include "Poco\\Data\\SQLite\\Connector.h" 
#include "Poco\\Data\\SessionFactory.h"
#include "Poco\\Data\\Session.h"

// �����û�������
#define USERNAME	"baggiopz"
#define PASSWORD	"871511"

int _tmain(int argc, _TCHAR* argv[])
{
	
	if (argc < 2)
	{
		//int errCode = scan->Connect();
		//errCode = scan->Login(USERNAME, PASSWORD);

		while (true)
		{
			Scanner scan;
			if (0 == scan.DoScan())
				break;
		}
	}
	else
	{
		if (_tcscmp(_T("-d"), argv[1]) == 0)
		{
			Downloader downloader;
			downloader.Start();
		}
	}

	return 0;
}

