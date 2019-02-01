#include "Downloader.h"
#include "StringExchange.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/avutil.h"
#ifdef __cplusplus
	};
#endif

#include <shellapi.h>
#include <WinInet.h>

#include "Poco/StreamCopier.h"
#include "Poco/UTF8Encoding.h"
#include "Poco/ASCIIEncoding.h"
#include "Poco/TextConverter.h"
#include "Poco/Net/MediaType.h"
#include "Poco/JSON/JSON.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/Statement.h"
#include "Poco/Exception.h"
#include <istream>
#include <ostream>
#include <sstream>

#include "curl/curl.h"

//////////////////////////////////////////////////////////////////////////
//
// 首先实现一个我自己的线程执行类
class MyRunnable : public Poco::Runnable
{
public:
	MyRunnable()
	{
		Poco::Data::SQLite::Connector::registerConnector();
	};

	~MyRunnable()
	{
		Poco::Data::SQLite::Connector::unregisterConnector();
	};

public:
	void SetParam(Poco::Data::Session *session, const char *id, const char *name, const char *url, const char *path) {
		session_ = session;
		id_ = id;
		name_ = name;
		url_ = url;
		path_ = path;
	}

	void SetParam(Poco::Data::Session *session, const char *id, const char *name, const char *url, const char *path, const char *ffmpeg) {
		session_ = session;
		id_ = id;
		name_ = name;
		url_ = url;
		path_ = path;
		ffmpeg_ = ffmpeg;
	}

public:
	static size_t GetFirstM3u8Content(void *ptr, size_t size, size_t nmemb, void *stream)
	{
		MyRunnable *object = (MyRunnable *)stream;
		object->first_m3u8_content_ += (char *)ptr;

		return size * nmemb;
	}

	static size_t GetSecondM3u8Content(void *ptr, size_t size, size_t nmemb, void *stream)
	{
		MyRunnable *object = (MyRunnable *)stream;
		object->second_m3u8_content_ += (char *)ptr;

		return size * nmemb;
	}

	static size_t DownloadVideoFile(void *ptr, size_t size, size_t nmemb, void *stream)
	{
		MyRunnable *object = (MyRunnable *)stream;
		object->second_m3u8_content_ += (char *)ptr;

		DWORD written = 0;
		BOOL bret = WriteFile(object->video_file_, ptr, size * nmemb, &written, NULL);
		if (!bret)
		{
			int errCode = GetLastError();
			std::cout<<"写入文件失败！错误码："<<errCode<<std::endl;
		}

		return size * nmemb;
	}

	virtual void run() {
		Poco::Thread *ptr_thread = Poco::Thread::current();

		//////////////////////////////////////////////////////////////////////////
		//
		// 转存参数信息
		Poco::Data::Session *session = session_;
		std::string id = id_.c_str();
		std::string name = name_.c_str();
		std::string url = url_.c_str();
		std::string path = path_.c_str();
		std::string ffmpeg = ffmpeg_.c_str();

		int errCode = 0;

		//if (ffmpeg.empty())
		{
			// 分析URL，找到HOST和PORT，考虑了以下，为了方便，这里还是用CURL吧
			//////////////////////////////////////////////////////////////////////////
			//
			int pos = url.find("2100/");
			std::string object = url.substr(pos + 4);

			CURLcode curl_code;
			CURL *curl = curl_easy_init();
			if (!curl)
			{
				std::cout<<"初始化CURL失败！"<<std::endl;
				return ;
			}

			// 设置参数
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https  
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false  
			curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);  
			curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);  
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, GetFirstM3u8Content);  
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)this);  
			curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);  
			curl_easy_setopt(curl, CURLOPT_HEADER, 0);  
			curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3); // set transport and time out time  
			curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);  

			first_m3u8_content_ = "";
			// start req 
			curl_code = curl_easy_perform(curl);

			// 这里应该是可以得到一个完整的结果了
			//std::cout<<first_m3u8_content_.c_str()<<std::endl;

			// 逐行分析M3U8文件内容，或者....直接读取换行符试试，一般读取第二个换行符到第三个换行符之间就可以了
			int first_pos = first_m3u8_content_.find("\n");
			std::string tmp_result = first_m3u8_content_.substr(first_pos + 1, -1);
			int second_pos = tmp_result.find("\n");
			tmp_result = tmp_result.substr(second_pos + 1, -1);
			int third_pos = tmp_result.find("\n");
			std::string second_m3u8_object = tmp_result.substr(0, third_pos);

			// 重新构建URL
			std::string real_m3u8_url;
			if (memcmp("http", second_m3u8_object.c_str(), 4) == 0)
			{
				// 这是一个带有http头部的链接
				real_m3u8_url = second_m3u8_object;
			}
			else
			{
				// 不带头部，则添加头部
				real_m3u8_url = "http://www.91gxflvip.com:2100";
				real_m3u8_url += second_m3u8_object;
			}

			// 设置参数
			curl_easy_setopt(curl, CURLOPT_URL, real_m3u8_url.c_str());
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https  
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false  
			curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);  
			curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);  
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, GetSecondM3u8Content);  
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)this);  
			curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);  
			curl_easy_setopt(curl, CURLOPT_HEADER, 0);  
			curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3); // set transport and time out time  
			curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);  

			second_m3u8_content_ = "";
			curl_code = curl_easy_perform(curl);

			//std::cout<<second_m3u8_content_.c_str()<<std::endl;
			if (second_m3u8_content_.empty())
				return;

			// 打开文件
			video_file_ = CreateFileA(path.c_str(), GENERIC_ALL, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (video_file_ == INVALID_HANDLE_VALUE)
			{
				errCode = GetLastError();
				std::cout<<"打开文件"<<path.c_str()<<"失败！错误码："<<errCode<<std::endl;
				return ;
			}

			std::cout<<"打开文件"<<path.c_str()<<"成功！错误码："<<errCode<<std::endl;

			// 开始循环分析,结束符为
			bool succeed = true;
			std::string content = second_m3u8_content_;
			while (true)
			{
				int next_line_flag_pos = content.find("\n");
				if (next_line_flag_pos < 0)
				{
					break;
				}

				std::string line_content = content.substr(0, next_line_flag_pos);
				int ts_flag = line_content.find(".ts");
				if (ts_flag < 0)
				{
					content = content.substr(next_line_flag_pos + 1, -1);
					continue;
				}

				std::string ts_url;
				if (memcmp("http", line_content.c_str(), 4) == 0)
				{
					// 这是一个带有http头部的链接
					ts_url = line_content;
				}
				else
				{
					// 不带头部，则添加头部
					ts_url = "http://www.91gxflvip.com:2100";
					ts_url += line_content;
				}

				// 下载这个文件
				curl_easy_setopt(curl, CURLOPT_URL, ts_url.c_str());
				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https  
				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false  
				curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);  
				curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);  
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, DownloadVideoFile);  
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)this);  
				curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);  
				curl_easy_setopt(curl, CURLOPT_HEADER, 0);  
				curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30); // set transport and time out time  
				curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);

				curl_code = curl_easy_perform(curl);
				if (CURLE_OK != curl_code)
				{
					std::cout<<"=====下载文件"<<path.c_str()<<"失败！"<<std::endl;
					succeed = false;
					break;
				}

				content = content.substr(next_line_flag_pos + 1, -1);
			}

			FlushFileBuffers(video_file_);
			CloseHandle(video_file_);

			curl_easy_cleanup(curl);
			if (succeed)
			{
				// 这里更新数据库信息
				std::cout<<"下载："<<path.c_str()<<"成功！"<<std::endl;
				Poco::Data::Statement update_(*session);
				char sql[4096] = {0};
				sprintf_s(sql, 4096, "UPDATE 'data' SET is_download = 1 WHERE id = %s", id.c_str());
				update_ << sql, Poco::Data::Keywords::now;
				update_.reset(*session);
			}
			

			//delete http_client_session_;
		}
		//else
		//{
	//		// 使用ffmpeg下载
	//		char param[4096] = {0};
	//		sprintf_s(param, 4096, "-i \"%s\" -codec copy \"%s\"", url.c_str(), path.c_str());

	//		SHELLEXECUTEINFOA exeinfo;
	//		ZeroMemory(&exeinfo, sizeof(SHELLEXECUTEINFOA));
	//		exeinfo.cbSize = sizeof(SHELLEXECUTEINFOA);
	//		exeinfo.lpVerb = "open";
	//		exeinfo.lpFile = ffmpeg.c_str();
	//		exeinfo.lpParameters = param;
	//		exeinfo.fMask = SEE_MASK_FLAG_NO_UI | SEE_MASK_NOCLOSEPROCESS;
	//		exeinfo.nShow = SW_MINIMIZE;
	//		BOOL bret = ShellExecuteExA(&exeinfo);
	//		if (!bret)
	//		{
	//			std::cout<<"下载："<<path.c_str()<<"失败！错误码："<<GetLastError()<<std::endl;
	//			return ;
	//		}

	//		WaitForSingleObject(exeinfo.hProcess, INFINITE);
	//	}

	//	// 这里更新数据库信息
	//	std::cout<<"下载："<<path.c_str()<<"成功！"<<std::endl;
	//	Poco::Data::Statement update_(*session);
	//	char sql[4096] = {0};
	//	sprintf_s(sql, 4096, "UPDATE 'data' SET is_download = 1 WHERE id = %s", id.c_str());
	//	update_ << sql, Poco::Data::Keywords::now;
	//	update_.reset(*session);
	}

private:
	std::string id_;
	std::string name_;
	std::string url_;
	std::string path_;
	std::string ffmpeg_;
	Poco::Data::Session *session_;

	std::string first_m3u8_content_;
	std::string second_m3u8_content_;

	HANDLE video_file_;
};

Downloader::Downloader()
//: threadpool_(new Poco::ThreadPool(1, 1))
: threadpool_(new Poco::ThreadPool())
{
	// 初始化数据库
	Poco::Data::SQLite::Connector::registerConnector();

	av_register_all();
	avcodec_register_all();
	avformat_network_init();
}

Downloader::~Downloader()
{
	avformat_network_deinit();
	Poco::Data::SQLite::Connector::unregisterConnector();
}

int Downloader::Init()
{
	int errCode = 0;

	return errCode;
}

int Downloader::Start()
{
	int errCode = 0;

	// 取得进程文件当前所在目录
	char current_process_path[4096] = {0};
	GetModuleFileNameA(NULL, current_process_path, 4096);
	std::string current_process_path_stl = current_process_path;
	int pos = current_process_path_stl.find_last_of("\\");
	std::string current_dir = current_process_path_stl.substr(0, pos + 1);

	std::string record_dir = current_dir;
	record_dir += "downloads\\";

	CreateDirectoryA(record_dir.c_str(), NULL);

	//////////////////////////////////////////////////////////////////////////
	//
	// 连接数据库，取出最近10个未下载的记录
	// SELECT id, name, url FROM 'data' WHERE is_download = 0 LIMIT 0,10

	// 初始化数据库会话
	Poco::Data::Session *session = new Poco::Data::Session("SQLite", "data.db3");
	Poco::Data::Statement select_(*session);

	std::vector<std::string> ids;
	std::vector<std::string> names;
	std::vector<std::string> urls;
	select_ << "SELECT id, name, url FROM 'data' WHERE is_download = 0"
		, Poco::Data::Keywords::into(ids)
		, Poco::Data::Keywords::into(names)
		, Poco::Data::Keywords::into(urls)
		, Poco::Data::Keywords::range(0, 1);

	while (!select_.done())
	{
		select_.execute();

		ffmpeg_ = current_dir;
		ffmpeg_ += "\\ffmpeg.exe";

		file_name_ = record_dir;
		file_name_ += ids[0];
		file_name_ += "_";

		// 编码转换，UTF-8 转 UTF-16
		setlocale(LC_ALL, "chs");
		Poco::UTF16String str16;
		Poco::UnicodeConverter::convert(names[0], str16);
		Poco::UTF16Char * char16 = (Poco::UTF16Char*)str16.c_str();
		size_t _Dsize = (str16.size() + 1) * 2;
		char * char8 = (char*)malloc(_Dsize);
		memset(char8, 0, _Dsize);
		wcstombs(char8, char16, _Dsize);

		std::string str = char8;
		delete[]char8;

		setlocale(LC_ALL, "C");

		// 检查文件名是否有非法字符，有的话替换成其他编码
		// 算了，还是简单点，直接将不合法字符移除掉
		str = StringExchange::ReplayceString(str);

		file_name_ += str;
		file_name_ += ".mkv";

		// 在这里，先检查剩余线程数量
		// 
		while (true)
		{
			int available_thread = threadpool_->available();
			if (available_thread > 0)
				break;

			Poco::Thread::sleep(1000);
		}
		

		MyRunnable *my_runnable = new MyRunnable();
		//my_runnable->SetParam(session, ids[0].c_str(), names[0].c_str(), urls[0].c_str(), file_name_.c_str());
		my_runnable->SetParam(session, ids[0].c_str(), names[0].c_str(), urls[0].c_str(), file_name_.c_str(), ffmpeg_.c_str());
		threadpool_->start(*my_runnable);
		Poco::Thread::sleep(1000);

		//// 执行下载
		//errCode = DoDownload(session, ids[0].c_str(), file_name_.c_str(), urls[0].c_str());
		//if (errCode != 0)
		//{
		//	std::cout<<"下载："<<str.c_str()<<"失败！错误码："<<errCode<<std::endl;
		//}
		//else
		//{
		//	std::cout<<"下载："<<str.c_str()<<"成功！"<<std::endl;
		//	Poco::Data::Statement update_(*session);
		//	char sql[4096] = {0};
		//	sprintf_s(sql, 4096, "UPDATE 'data' SET is_download = 1 WHERE id = %s", ids[0].c_str());
		//	update_ << sql, Poco::Data::Keywords::now;
		//	//update_.execute();
		//	update_.reset(*session);
		//}

		ids.clear();
		names.clear();
		urls.clear();
	}

	// 这里可能需要等待线程池清空
	threadpool_->joinAll();

	return errCode;
}

int Downloader::DoDownload(Poco::Data::Session *session, const char *id, const char *output_file, const char *url)
{
	//////////////////////////////////////////////////////////////////////////
	//
	// 预处理输入文件

	AVFormatContext *input_fmtctx_ = NULL;
	int errCode = avformat_open_input(&input_fmtctx_, url, NULL, NULL);
	if (errCode < 0)
	{
		errCode = -2;
		goto end;
	}
	
	errCode = avformat_find_stream_info(input_fmtctx_, NULL);
	if (errCode < 0)
	{
		errCode = -3;
		goto end;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	// 预处理输出文件

	AVFormatContext *output_fmtctx_ = NULL;
	errCode = avformat_alloc_output_context2(&output_fmtctx_, NULL, NULL, output_file);
	if (!output_fmtctx_)
	{
		errCode = -4;
		goto end;
	}

	AVOutputFormat *ofmt = output_fmtctx_->oformat;

	//////////////////////////////////////////////////////////////////////////
	AVStream *video_stream = NULL;
	AVStream *audio_stream = NULL;
	int video_index = -1;
	int audio_index = -1;

	AVStream *video_output_stream = NULL;
	AVStream *audio_output_stream = NULL;

	AVCodecContext *audio_output_codecctx_ = NULL;
	AVCodec *audio_encoder_ = NULL;

	AVCodecContext *audio_input_codecctx_ = NULL;
	AVCodec *audio_decoder_ = NULL;

	AVFrame *audio_frame_ = NULL;
	int buffer_size = 0;
	uint8_t *frame_buffer = NULL;

	for (int index = 0; index < input_fmtctx_->nb_streams; ++index)
	{
		AVStream *in_stream = input_fmtctx_->streams[index];
		AVCodecParameters *in_codecpar = in_stream->codecpar;

		if (in_codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			video_index = index;
			video_stream = in_stream;

			// 这里就是copy了原来的流信息，包括codec这一类的数据
			video_output_stream = avformat_new_stream(output_fmtctx_, NULL);
			if (video_output_stream == NULL)
			{
				printf("创建视频输出流失败！\n");
				errCode = AVERROR_UNKNOWN;
				goto end;
			}

			// 复制参数
			errCode = avcodec_parameters_copy(video_output_stream->codecpar, in_codecpar);
			if (errCode < 0)
			{
				printf("复制流参数失败！\n");
				errCode = AVERROR_UNKNOWN;
				goto end;
			}

			video_output_stream->codecpar->codec_tag = 0;
		}
		else if (in_codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			audio_index = index;
			audio_stream = in_stream;

			audio_input_codecctx_ = audio_stream->codec;

			//////////////////////////////////////////////////////////////////////////
			// 创建输出流
			audio_output_stream = avformat_new_stream(output_fmtctx_, NULL);
			if (audio_output_stream == NULL)
			{
				printf("创建音频输出流失败！\n");
				errCode = AVERROR_UNKNOWN;
				goto end;
			}

			// 复制参数
			errCode = avcodec_parameters_copy(audio_output_stream->codecpar, in_codecpar);
			if (errCode < 0)
			{
				printf("复制流参数失败！\n");
				errCode = AVERROR_UNKNOWN;
				goto end;
			}

			audio_output_stream->codecpar->codec_tag = 0;

		}
	}

	//////////////////////////////////////////////////////////////////////////
	if (!(output_fmtctx_->flags & AVFMT_NOFILE))
	{
		errCode = avio_open(&output_fmtctx_->pb, output_file, AVIO_FLAG_WRITE);
		if (errCode < 0)
		{
			errCode = -5;
			goto end;
		}
	}

	errCode = avformat_write_header(output_fmtctx_, NULL);
	if (errCode < 0)
	{
		errCode = -6;
		goto end;
	}

	int total = 0;
	while (true)
	{
		AVPacket pkt;
		errCode = av_read_frame(input_fmtctx_, &pkt);
		if (errCode < 0)
			break;

		AVStream *in_stream = input_fmtctx_->streams[pkt.stream_index];
		AVStream *out_stream = output_fmtctx_->streams[pkt.stream_index];

		//
		pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
		pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
		pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
		pkt.pos = -1;

		errCode = av_interleaved_write_frame(output_fmtctx_, &pkt);
		if (errCode < 0)
		{
			printf("写入帧失败！\n");
			goto end;
		}
		else
		{
			total += pkt.size;
			//printf("正在下载：%s号文件，已下载：%d 字节数据...\n", id, total);
		}

		av_packet_unref(&pkt);
	}

	av_write_trailer(output_fmtctx_);

end:

	avformat_close_input(&input_fmtctx_);

	if (output_fmtctx_ && !(output_fmtctx_->flags & AVFMT_NOFILE))
		avio_closep(&output_fmtctx_->pb);
	avformat_free_context(output_fmtctx_);

	if (errCode < 0 && errCode != AVERROR_EOF)
	{
		//printf("发生了错误！%s\n", (av_err2str(errCode)));
		return 1;
	}

	return 0;
}