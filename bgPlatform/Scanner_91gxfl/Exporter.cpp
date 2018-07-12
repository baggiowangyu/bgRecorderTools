#include "Exporter.h"

Exporter::Exporter()
{
	// 初始化数据库
	Poco::Data::SQLite::Connector::registerConnector();
}

Exporter::~Exporter()
{
	Poco::Data::SQLite::Connector::unregisterConnector();
}

void Exporter::Export(const char *root_dir)
{
	// 首先创建输出目录
	Poco::Path export_dir_path(root_dir);
	Poco::File export_dir(export_dir_path);
	export_dir.createDirectories();

	Poco::Path txt_file_path = export_dir_path.append("export.txt");
	Poco::File txt_file(export_dir_path);
	txt_file.remove();
	if (!txt_file.createFile())
	{
		return ;
	}

	// 查询所有记录
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

		// 这里写入文件，先写文件名，再写下载地址



		//++count;
	}

	ids.clear();
	names.clear();
	urls.clear();
	select_.reset(*session);
}