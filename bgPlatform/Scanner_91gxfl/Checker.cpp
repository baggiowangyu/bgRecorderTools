#include "Checker.h"

Checker::Checker()
{
	// 初始化数据库
	Poco::Data::SQLite::Connector::registerConnector();
}

Checker::~Checker()
{
	Poco::Data::SQLite::Connector::unregisterConnector();
}

int Checker::Start(const char *root_dir)
{
	int errCode = 0;

	// 从数据库逐条查，拼接文件名，尝试打开
	int count = 0;

	//////////////////////////////////////////////////////////////////////////
	//
	// 连接数据库，取出最近10个已下载的记录
	// SELECT id, name, url FROM 'data' WHERE is_download = 1 LIMIT 0,10

	// 初始化数据库会话
	Poco::Data::Session *session = new Poco::Data::Session("SQLite", "data.db3");
	Poco::Data::Statement select_(*session);

	std::vector<std::string> ids;
	std::vector<std::string> names;
	std::vector<std::string> urls;

	select_ << "SELECT id, name, url FROM 'data'"
		, Poco::Data::Keywords::into(ids)
		, Poco::Data::Keywords::into(names)
		, Poco::Data::Keywords::into(urls);

	select_.execute();
	
	for (int index = 0; index < ids.size(); ++index)
	{
		std::string file_name_;
		file_name_ = root_dir;
		file_name_ += ids[index];
		file_name_ += "_";

		// 编码转换，UTF-8 转 UTF-16
		setlocale(LC_ALL, "chs");
		Poco::UTF16String str16;
		Poco::UnicodeConverter::convert(names[index], str16);
		Poco::UTF16Char * char16 = (Poco::UTF16Char*)str16.c_str();
		size_t _Dsize = (str16.size() + 1) * 2;
		char * char8 = (char*)malloc(_Dsize);
		memset(char8, 0, _Dsize);
		wcstombs(char8, char16, _Dsize);

		std::string str = char8;
		delete[]char8;

		setlocale(LC_ALL, "C");

		file_name_ += /*str*/names[index];
		file_name_ += ".mkv";
	
		try
		{
			Poco::Path path_(file_name_);
			Poco::File file(path_);
			Poco::File::FileSize size = file.getSize();
			if (size == 0)
			{
				// 不存在，则将数据库已下载的标记改为未下载
				Poco::Data::Statement update_(*session);
				char sql[4096] = {0};
				sprintf_s(sql, 4096, "UPDATE 'data' SET is_download = 0 WHERE id = %s", ids[index].c_str());
				update_ << sql, Poco::Data::Keywords::now;
				update_.reset(*session);
			}
		}
		catch(Poco::Exception e)
		{
			std::string err = e.displayText();
			errCode = e.code();
			if (errCode == 2)
			{
				// 意思就是文件不存在
				Poco::Data::Statement update_(*session);
				char sql[4096] = {0};
				sprintf_s(sql, 4096, "UPDATE 'data' SET is_download = 0 WHERE id = %s", ids[index].c_str());
				update_ << sql, Poco::Data::Keywords::now;
				update_.reset(*session);
			}
			std::cout<<errCode<<"  "<<err.c_str()<<std::endl;
		}
		

		++count;
	}

	ids.clear();
	names.clear();
	urls.clear();
	select_.reset(*session);

	return errCode;
}