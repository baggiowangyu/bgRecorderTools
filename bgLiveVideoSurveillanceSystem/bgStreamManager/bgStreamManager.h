#ifndef _BG_STREAM_MANAGER_H_
#define _BG_STREAM_MANAGER_H_

#include <string>
#include <map>


enum STREAM_NOTIFY_TYPE
{
	StreamPlay,
	StreamSave
};

class bgStreamNotifer
{
public:
	virtual int StreamNotifer(enum STREAM_NOTIFY_TYPE msg_type, const char *url) = 0;
};

class bgStreamManager
{
public:
	bgStreamManager(bgStreamNotifer *notifer)
		: notifer_(notifer)
	{}

	~bgStreamManager()
	{
		notifer_ = NULL;
	}

public:
	int SetSaveRootPath(const char *rootpath);
	int HandleURL(const char *url, bool isPlay, bool isSave);

public:
	int Play(const char *url);
	int Stop(const char *url);

public:
	int SaveStream(const char *url);

public:
	int InsertStreamUrl(const char *url);
	int RemoveStreamUrl(const char *url);

public:
	std::map<std::string, int> stream_urls_;

private:
	bgStreamNotifer *notifer_;
	
};

#endif//_BG_STREAM_MANAGER_H_