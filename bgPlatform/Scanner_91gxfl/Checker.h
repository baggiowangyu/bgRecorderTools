#ifndef _Checker_H_
#define _Checker_H_

#include "Poco/Data/SQLite/SQLite.h"
#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/Statement.h"
#include "Poco/Exception.h"
#include "Poco/File.h"
#include "Poco/Path.h"

class Checker
{
public:
	Checker();
	~Checker();

public:
	//int Init(const char *root_dir);
	int Start(const char *root_dir);
};

#endif//_Checker_H_
