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

	// 首先查找当前进程列表里面是否存在流处理对象，如果存在就不处理了
	bgStreamHandler *handler = NULL;
	std::map<std::string, bgStreamHandler*>::iterator iter = stream_urls_.find(url);
	if (iter != stream_urls_.end())
		handler = iter->second;
	else
	{
		handler = new bgStreamHandler(this);

		errCode = handler->Initialize();
		if (errCode != 0)
		{
			delete handler;
			return errCode;
		}
	}

	// 这个命令是要保存录像
	if (isSave)
	{
		errCode = handler->Start(url, StreamTypeLive, Op_Record);
		if (errCode != 0)
		{
			delete handler;
			return errCode;
		}
	}

	// 这个命令是要播放录像
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


int bgStreamManager::StreamCodecInfoNotifer(AVCodecContext *video_codec_ctx, AVCodecContext *audio_codec_ctx)
{
	int errCode = 0;

	return errCode;
}

int bgStreamManager::PlayingStreamNotifer(const char *url, AVFrame *frame)
{
	int errCode = 0;

	return errCode;
}

int bgStreamManager::SaveStreamNotifer(const char *url, AVPacket *pkt)
{
	int errCode = 0;

	return errCode;
}
