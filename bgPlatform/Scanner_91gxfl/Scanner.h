#ifndef _Scanner_H_
#define _Scanner_H_

#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"

#include "Poco/Data/SQLite/SQLite.h"

#define ROOT_URL	"www.2018gxfl.com"

class Scanner
{
public:
	Scanner();
	~Scanner();

public:
	int Connect();
	int Login(const char *username, const char *password);

	int DoScan();

private:
	Poco::Net::HTTPClientSession *http_client_session_;
	std::vector<Poco::Net::HTTPCookie> http_cookies_;
};

#endif//_Scanner_H_
