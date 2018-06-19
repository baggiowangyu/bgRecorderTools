#ifndef _Exporter_H_
#define _Exporter_H_

#include "Poco/Data/SQLite/SQLite.h"
#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/Statement.h"
#include "Poco/Exception.h"
#include "Poco/File.h"
#include "Poco/Path.h"

class Exporter
{
public:
	Exporter();
	~Exporter();

public:
	void Export(const char *root_dir);
};
#endif