#include "stdafx.h"
#include "bgStreamHandler.h"



int bgStreamHandler::Initialize()
{
	int errCode = 0;

	av_register_all();
	avformat_network_init();
	avcodec_register_all();

	return errCode;
}

void bgStreamHandler::Close()
{

}

int bgStreamHandler::Start(const char *url, enum StreamType type, enum StreamOperator op)
{
	int errCode = 0;

	// �Ƚ������ɵĹ���
	Close();

	type_ = type;
	url_ = url;

	// ����߳��Ƿ��������У���û�У��������̣߳����������У�������
	CreateThread(NULL, 0, bgStreamHandler::WorkingThread, this, 0, NULL);
	CreateThread(NULL, 0, bgStreamHandler::EventThread, this, 0, NULL);

	return errCode;
}

void bgStreamHandler::Stop()
{

}

DWORD WINAPI bgStreamHandler::WorkingThread(LPVOID lpParam)
{
	bgStreamHandler *handler = (bgStreamHandler *)lpParam;

	// ���ȿ�ʼ�⸴��
	AVFormatContext *input_fmtctx = NULL;
	int errCode = avformat_open_input(&input_fmtctx, handler->url_.c_str(), NULL, NULL);
	if (errCode != 0)
	{
		// ������Ҫ�ϱ��ص�
		return errCode;
	}

	avformat_find_stream_info(input_fmtctx, NULL);

	// �õ�����Ƶ������Ϣ
	int video_stream_index = -1;
	int audio_stream_index = -1;

	AVStream *video_stream = NULL;
	AVStream *audio_stream = NULL;

	AVCodecContext *video_codec_ctx = NULL;
	AVCodecContext *audio_codec_ctx = NULL;

	AVCodec *video_codec = NULL;
	AVCodec *audio_codec = NULL;

	for (int index = 0; index < input_fmtctx->nb_streams; ++index)
	{
		if (input_fmtctx->streams[index]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			video_stream_index = index;
			video_stream = input_fmtctx->streams[index];
			video_codec_ctx = input_fmtctx->streams[index]->codec;

			video_codec = avcodec_find_decoder(video_codec_ctx->codec_id);
			if (!video_codec)
			{
				// �����ˣ�û�ҵ���Ƶ������
				break;
			}

			errCode = avcodec_open2(video_codec_ctx, video_codec, NULL);
			if (errCode != 0)
			{
				// �����ˣ�����Ƶ������ʧ��
				break;
			}
		}
		else if (input_fmtctx->streams[index]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			audio_stream_index = index;
			audio_stream = input_fmtctx->streams[index];
			audio_codec_ctx = input_fmtctx->streams[index]->codec;

			audio_codec = avcodec_find_decoder(audio_codec_ctx->codec_id);
			if (!audio_codec)
			{
				// �����ˣ�û�ҵ���Ƶ������
				break;
			}

			errCode = avcodec_open2(audio_codec_ctx, audio_codec, NULL);
			if (errCode != 0)
			{
				// �����ˣ�����Ƶ������ʧ��
				break;
			}
		}
	}

	// ��������ȡ��Ƶ�����ҵ���Ƶ֡����֡���ݻص����͸��ϲ�
	// ����
	AVPacket av_packet;
	while (true)
	{
		errCode = av_read_frame(input_fmtctx, &av_packet);
		if (errCode != 0)
			break;

		if (av_packet.stream_index == video_stream_index)
		{
			// ������Ƶ֡���Ƚ�֡���ݻص�������ص�
			handler->notifer_->SaveStreamNotifer(handler->url_.c_str(), &av_packet);
			
			// �����ص������Żص�
			AVFrame *frame = av_frame_alloc();
			int got_pic = 0;
			avcodec_decode_video2(video_codec_ctx, frame, &got_pic, &av_packet);
			
			if (got_pic)
			{
				// ���Ȼص�������Ƶ��Ϣ

				// Ȼ����
			}
		}
		else if (av_packet.stream_index == audio_stream_index)
		{
			// ������Ƶ֡
		}
	}

	return 0;
}

DWORD WINAPI bgStreamHandler::EventThread(LPVOID lpParam)
{
	bgStreamHandler *handler = (bgStreamHandler *)lpParam;

	return 0;
}