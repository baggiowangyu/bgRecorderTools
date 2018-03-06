#include "stdafx.h"
#include "bgStreamManager.h"
#include "bgStreamHandler.h"



int bgStreamManager::SetSaveRootPath(const char *rootpath)
{
	int errCode = 0;

	root_path_ = rootpath;

	return errCode;
}

int bgStreamManager::HandleURL(const char *url, bool isPlay, bool isSave)
{
	int errCode = 0;

	// ���Ȳ��ҵ�ǰ�����б������Ƿ�������������������ھͲ�������
	bgStreamHandler *handler = NULL;
	std::map<std::string, bgStreamHandler*>::iterator iter = stream_urls_.find(url);
	if (iter != stream_urls_.end())
		handler = iter->second;
	else
	{
		handler = new bgStreamHandler((bgStreamHandlerNotifer*)notifer_);

		errCode = handler->Initialize();
		if (errCode != 0)
		{
			delete handler;
			return errCode;
		}
	}

	// ���������Ҫ����¼��
	if (isSave)
	{
		errCode = handler->Start(url, StreamTypeLive, Op_Record);
		if (errCode != 0)
		{
			delete handler;
			return errCode;
		}
	}

	// ���������Ҫ����¼��
	if (isPlay)
	{
		errCode = handler->Start(url, StreamTypeLive, Op_Play);
		if (errCode != 0)
		{
			delete handler;
			return errCode;
		}
	}

	stream_urls_.insert(std::pair<std::string, bgStreamHandler*>(url, handler));

	return errCode;
}


