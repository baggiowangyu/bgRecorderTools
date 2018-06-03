#include "Downloader.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/avutil.h"
#ifdef __cplusplus
	};
#endif

//////////////////////////////////////////////////////////////////////////
//
// ����ʵ��һ�����Լ����߳�ִ����
class MyRunnable : public Poco::Runnable
{
public:
	MyRunnable() {};
	~MyRunnable() {};

public:
	void SetParam(Poco::Data::Session *session, const char *id, const char *name, const char *url, const char *path) {
		session_ = session;
		id_ = id;
		name_ = name;
		url_ = url;
		path_ = path;
	}

public:
	virtual void run() {
		Poco::Thread *ptr_thread = Poco::Thread::current();

		//////////////////////////////////////////////////////////////////////////
		//
		// ת�������Ϣ
		Poco::Data::Session *session = session_;
		std::string id = id_.c_str();
		std::string name = name_.c_str();
		std::string url = url_.c_str();
		std::string path = path_.c_str();

		//////////////////////////////////////////////////////////////////////////
		//
		// Ԥ���������ļ�

		AVFormatContext *input_fmtctx_ = NULL;
		int errCode = avformat_open_input(&input_fmtctx_, url.c_str(), NULL, NULL);
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
		// Ԥ��������ļ�

		AVFormatContext *output_fmtctx_ = NULL;
		errCode = avformat_alloc_output_context2(&output_fmtctx_, NULL, NULL, path.c_str());
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

				// �������copy��ԭ��������Ϣ������codec��һ�������
				video_output_stream = avformat_new_stream(output_fmtctx_, NULL);
				if (video_output_stream == NULL)
				{
					printf("������Ƶ�����ʧ�ܣ�\n");
					errCode = AVERROR_UNKNOWN;
					goto end;
				}

				// ���Ʋ���
				errCode = avcodec_parameters_copy(video_output_stream->codecpar, in_codecpar);
				if (errCode < 0)
				{
					printf("����������ʧ�ܣ�\n");
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
				// ���������
				audio_output_stream = avformat_new_stream(output_fmtctx_, NULL);
				if (audio_output_stream == NULL)
				{
					printf("������Ƶ�����ʧ�ܣ�\n");
					errCode = AVERROR_UNKNOWN;
					goto end;
				}

				// ���Ʋ���
				errCode = avcodec_parameters_copy(audio_output_stream->codecpar, in_codecpar);
				if (errCode < 0)
				{
					printf("����������ʧ�ܣ�\n");
					errCode = AVERROR_UNKNOWN;
					goto end;
				}

				audio_output_stream->codecpar->codec_tag = 0;

			}
		}

		//////////////////////////////////////////////////////////////////////////
		if (!(output_fmtctx_->flags & AVFMT_NOFILE))
		{
			errCode = avio_open(&output_fmtctx_->pb, path.c_str(), AVIO_FLAG_WRITE);
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
				printf("д��֡ʧ�ܣ�\n");
				goto end;
			}
			else
			{
				total += pkt.size;
				//printf("�������أ�%s���ļ��������أ�%d �ֽ�����...\n", id, total);
			}

			av_packet_unref(&pkt);
		}

		av_write_trailer(output_fmtctx_);

end:

		avformat_close_input(&input_fmtctx_);

		if (output_fmtctx_)
		{
			if (output_fmtctx_->flags & AVFMT_NOFILE)
				avio_closep(&output_fmtctx_->pb);

			avformat_free_context(output_fmtctx_);
		}

		if (errCode < 0 && errCode != AVERROR_EOF)
		{
			//printf("�����˴���%s\n", (av_err2str(errCode)));
			return ;
		}

		// ����������ݿ���Ϣ
		std::cout<<"���أ�"<<path.c_str()<<"�ɹ���"<<std::endl;
		Poco::Data::Statement update_(*session);
		char sql[4096] = {0};
		sprintf_s(sql, 4096, "UPDATE 'data' SET is_download = 1 WHERE id = %s", id.c_str());
		update_ << sql, Poco::Data::Keywords::now;
		update_.reset(*session);
	}

private:
	std::string id_;
	std::string name_;
	std::string url_;
	std::string path_;
	Poco::Data::Session *session_;
};

Downloader::Downloader()
: threadpool_(new Poco::ThreadPool())
{
	// ��ʼ�����ݿ�
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

	// ȡ�ý����ļ���ǰ����Ŀ¼
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
	// �������ݿ⣬ȡ�����10��δ���صļ�¼
	// SELECT id, name, url FROM 'data' WHERE is_download = 0 LIMIT 0,10

	// ��ʼ�����ݿ�Ự
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

		file_name_ = record_dir;
		file_name_ += ids[0];
		file_name_ += "_";

		// ����ת����UTF-8 ת UTF-16
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

		file_name_ += str;
		file_name_ += ".mkv";

		// ������ȼ��ʣ���߳�����
		// 
		while (true)
		{
			int available_thread = threadpool_->available();
			if (available_thread > 0)
				break;

			Poco::Thread::sleep(1000);
		}
		

		MyRunnable *my_runnable = new MyRunnable();
		my_runnable->SetParam(session, ids[0].c_str(), names[0].c_str(), urls[0].c_str(), file_name_.c_str());
		threadpool_->start(*my_runnable);
		Poco::Thread::sleep(1000);

		//// ִ������
		//errCode = DoDownload(session, ids[0].c_str(), file_name_.c_str(), urls[0].c_str());
		//if (errCode != 0)
		//{
		//	std::cout<<"���أ�"<<str.c_str()<<"ʧ�ܣ������룺"<<errCode<<std::endl;
		//}
		//else
		//{
		//	std::cout<<"���أ�"<<str.c_str()<<"�ɹ���"<<std::endl;
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

	return errCode;
}

int Downloader::DoDownload(Poco::Data::Session *session, const char *id, const char *output_file, const char *url)
{
	//////////////////////////////////////////////////////////////////////////
	//
	// Ԥ���������ļ�

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
	// Ԥ��������ļ�

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

			// �������copy��ԭ��������Ϣ������codec��һ�������
			video_output_stream = avformat_new_stream(output_fmtctx_, NULL);
			if (video_output_stream == NULL)
			{
				printf("������Ƶ�����ʧ�ܣ�\n");
				errCode = AVERROR_UNKNOWN;
				goto end;
			}

			// ���Ʋ���
			errCode = avcodec_parameters_copy(video_output_stream->codecpar, in_codecpar);
			if (errCode < 0)
			{
				printf("����������ʧ�ܣ�\n");
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
			// ���������
			audio_output_stream = avformat_new_stream(output_fmtctx_, NULL);
			if (audio_output_stream == NULL)
			{
				printf("������Ƶ�����ʧ�ܣ�\n");
				errCode = AVERROR_UNKNOWN;
				goto end;
			}

			// ���Ʋ���
			errCode = avcodec_parameters_copy(audio_output_stream->codecpar, in_codecpar);
			if (errCode < 0)
			{
				printf("����������ʧ�ܣ�\n");
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
			printf("д��֡ʧ�ܣ�\n");
			goto end;
		}
		else
		{
			total += pkt.size;
			//printf("�������أ�%s���ļ��������أ�%d �ֽ�����...\n", id, total);
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
		//printf("�����˴���%s\n", (av_err2str(errCode)));
		return 1;
	}

	return 0;
}