#include "Exporter.h"

Exporter::Exporter()
{
	// ��ʼ�����ݿ�
	Poco::Data::SQLite::Connector::registerConnector();
}

Exporter::~Exporter()
{
	Poco::Data::SQLite::Connector::unregisterConnector();
}

void Exporter::Export(const char *root_dir)
{
	// ���ȴ������Ŀ¼
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

	// ��ѯ���м�¼
	//////////////////////////////////////////////////////////////////////////
	//
	// �������ݿ⣬ȡ�����10�������صļ�¼
	// SELECT id, name, url FROM 'data' WHERE is_download = 1 LIMIT 0,10

	// ��ʼ�����ݿ�Ự
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

		// ����ת����UTF-8 ת UTF-16
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

		// ����д���ļ�����д�ļ�������д���ص�ַ



		//++count;
	}

	ids.clear();
	names.clear();
	urls.clear();
	select_.reset(*session);
}