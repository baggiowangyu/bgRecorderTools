#include "stdafx.h"
#include "bgStreamManager.h"



int bgStreamManager::HandleURL(const char *url, bool isPlay, bool isSave)
{
	// 这个命令是要保存录像
	if (isSave)
	{
		// 首先检查当前url是否已在流管理的
		notifer_->StreamNotifer(StreamSave, url);
	}

	// 这个命令是要播放录像
	if (isPlay)
	{
		notifer_->StreamNotifer(StreamPlay, url);
	}

	return 0;
}

int bgStreamManager::Play(const char *url)
{
	// 扔到流记录模块，将数据回调通知给
	return 0;
}

int bgStreamManager::Stop(const char *url)
{
	return 0;
}

int bgStreamManager::SaveStream(const char *url)
{
	// 扔到流记录模块，开始流记录
}

int bgStreamManager::InsertStreamUrl(const char *url)
{
	int element_count = stream_urls_.size();
	stream_urls_.insert(std::pair<std::string, int>(url, element_count));

	return 0;
}

int bgStreamManager::RemoveStreamUrl(const char *url)
{
	std::map<std::string, int>::iterator iter = stream_urls_.find(url);
	stream_urls_.erase(iter);

	return 0;
}