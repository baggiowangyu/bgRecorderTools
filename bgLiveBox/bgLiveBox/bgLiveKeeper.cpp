#include "bgLiveKeeper.h"

bgLiveKeeper::bgLiveKeeper()
: session_(NULL)
{
	// ��ʼ�����ݿ�
	Poco::Data::SQLite::Connector::registerConnector();
}

bgLiveKeeper::~bgLiveKeeper()
{
	Poco::Data::SQLite::Connector::unregisterConnector();
}

int bgLiveKeeper::Init()
{
	int errCode = 0;

	try
	{
		session_ = new Poco::Data::Session("SQLite", "bgKeep.dll");
		if (session_ == NULL)
		{
			errCode = -1;
		}
	} catch (Poco::Exception e) {
		std::string errmsg = e.displayText();
		errCode = e.code();
	}
	
	return errCode;
}

void bgLiveKeeper::Close()
{
	if (session_ != NULL)
		delete session_;
	session_ = NULL;
}

int bgLiveKeeper::GetStoredLivingInfo()
{
	int errCode = 0;

	// �����ݿ��ȡ

	return errCode;
}