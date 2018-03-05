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

	// 先结束掉旧的工作
	Close();

	type_ = type;
	url_ = url;

	// 检测线程是否正在运行，若没有，则启动线程，若正在运行，则跳过
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

	// 首先开始解复用
	AVFormatContext *input_fmtctx = NULL;
	int errCode = avformat_open_input(&input_fmtctx, handler->url_.c_str(), NULL, NULL);
	if (errCode != 0)
	{
		// 这里需要上报回调
		return errCode;
	}

	avformat_find_stream_info(input_fmtctx, NULL);

	// 拿到视音频流的信息
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
				// 出错了，没找到视频解码器
				break;
			}

			errCode = avcodec_open2(video_codec_ctx, video_codec, NULL);
			if (errCode != 0)
			{
				// 出错了，打开视频解码器失败
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
				// 出错了，没找到音频解码器
				break;
			}

			errCode = avcodec_open2(audio_codec_ctx, audio_codec, NULL);
			if (errCode != 0)
			{
				// 出错了，打开音频解码器失败
				break;
			}
		}
	}

	// 接下来读取视频流，找到视频帧，将帧数据回调发送给上层
	// 本层
	AVPacket av_packet;
	while (true)
	{
		errCode = av_read_frame(input_fmtctx, &av_packet);
		if (errCode != 0)
			break;

		if (av_packet.stream_index == video_stream_index)
		{
			// 这是视频帧，先将帧数据回调给保存回调
			handler->notifer_->SaveStreamNotifer(handler->url_.c_str(), &av_packet);
			
			// 解码后回调给播放回调
			AVFrame *frame = av_frame_alloc();
			int got_pic = 0;
			avcodec_decode_video2(video_codec_ctx, frame, &got_pic, &av_packet);
			
			if (got_pic)
			{
				// 首先回调发送视频信息

				// 然后发送
			}
		}
		else if (av_packet.stream_index == audio_stream_index)
		{
			// 这是音频帧
		}
	}

	return 0;
}

DWORD WINAPI bgStreamHandler::EventThread(LPVOID lpParam)
{
	bgStreamHandler *handler = (bgStreamHandler *)lpParam;

	return 0;
}