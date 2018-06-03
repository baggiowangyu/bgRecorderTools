#ifndef _Downloader_H_
#define _Downloader_H_

#include "Poco/Data/SQLite/SQLite.h"
#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/Statement.h"
#include "Poco/Exception.h"

#include "Poco/ThreadPool.h"
#include "Poco/Runnable.h"
#include "Poco/ThreadTarget.h"
#include "Poco/Event.h"
#include "Poco/Timestamp.h"
#include "Poco/Timespan.h"

#include <string>

class Downloader
{
public:
	Downloader();
	~Downloader();

public:
	int Init();
	int Start();

private:
	int DoDownload(Poco::Data::Session *session, const char *id, const char *output_file, const char *url);

private:
	Poco::ThreadPool *threadpool_;

private:
	std::string file_name_;
	std::string url_;
};

#endif//_Downloader_H_
