#ifndef _bgLiveKeeper_H_
#define _bgLiveKeeper_H_

#include <string>
#include "Poco/Thread.h"
#include "Poco/Data/SQLite/SQLite.h"
#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/Statement.h"
#include "Poco/Exception.h"

struct StoredRoomInfo
{
	std::string app_name_;
	std::string room_name_;
};

struct LivingRoomInfo
{
	struct StoredRoomInfo room_info;
	std::string history_url_;
};

class bgLiveKeeper
{
public:
	bgLiveKeeper();
	~bgLiveKeeper();

public:
	/**
	 * 初始化守护模块，连接数据库
	 */
	int Init();
	void Close();

public:
	/**
	 * 获取已经保存的守护信息
	 */
	int GetStoredLivingInfo();
	int AddLivingInfo(const char *app_name, const char *room_name);
	int RemoveLivingInfo(const char *app_name, const char *room_name);

public:
	/**
	 * 开启守护线程
	 */
	int Start();
	void Stop();

private:
	Poco::Data::Session *session_;

};

#endif//_bgLiveKeeper_H_
