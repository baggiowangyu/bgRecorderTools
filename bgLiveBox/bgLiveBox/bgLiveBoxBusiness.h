#ifndef _bgLiveBoxBusiness_H_
#define _bgLiveBoxBusiness_H_

#include "Poco/StreamCopier.h"
#include "Poco/UTF8Encoding.h"
#include "Poco/ASCIIEncoding.h"
#include "Poco/TextConverter.h"
#include "Poco/Net/MediaType.h"
#include "Poco/JSON/JSON.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Exception.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"

#include "Poco/DOM/DOMParser.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/Node.h"
#include "Poco/DOM/NodeIterator.h"
#include "Poco/DOM/NodeFilter.h"
#include "Poco/DOM/NamedNodeMap.h"
#include "Poco/AutoPtr.h"
#include "Poco/XML/XMLString.h"

#include <istream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

struct app_info
{
	std::string app_name;
	std::string app_id;
	std::string app_img_url;
};

struct room_info
{
	int index;
	std::string img_url;
	std::string name;
	std::string live_url;
};

#define ROOT_URL	"zb.qm3.top"

class bgLiveBoxBusinessObserver
{
public:
	virtual int AppUpdated() = 0;
	virtual int RoomUpdate(const char *app_id, const char *app_name) = 0;
	virtual void StateNotify(const char *state) = 0;
};

class bgLiveBoxBusiness
{
public:
	bgLiveBoxBusiness(bgLiveBoxBusinessObserver *observer);
	~bgLiveBoxBusiness();

public:
	int UpdateApps();
	int UpdateRooms(const char *app_id, const char *app_name);

public:
	std::vector<app_info> apps;
	std::vector<room_info> current_rooms;

private:
	Poco::Net::HTTPClientSession *http_client_session_;
	bgLiveBoxBusinessObserver *observer_;
};

#endif//_bgLiveBoxBusiness_H_