#include "stdafx.h"
#include "bgStreamManager.h"



int bgStreamManager::HandleURL(const char *url, bool isPlay, bool isSave)
{
	// ���������Ҫ����¼��
	if (isSave)
	{
		// ���ȼ�鵱ǰurl�Ƿ������������
		notifer_->StreamNotifer(StreamSave, url);
	}

	// ���������Ҫ����¼��
	if (isPlay)
	{
		notifer_->StreamNotifer(StreamPlay, url);
	}

	return 0;
}

int bgStreamManager::Play(const char *url)
{
	// �ӵ�����¼ģ�飬�����ݻص�֪ͨ��
	return 0;
}

int bgStreamManager::Stop(const char *url)
{
	return 0;
}

int bgStreamManager::SaveStream(const char *url)
{
	// �ӵ�����¼ģ�飬��ʼ����¼
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