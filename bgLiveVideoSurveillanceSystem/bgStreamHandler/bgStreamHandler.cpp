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
	is_play_ = false;
	is_record_ = false;
}

int bgStreamHandler::Start(const char *url, enum StreamType type, enum StreamOperator op)
{
	int errCode = 0;

	type_ = type;
	url_ = url;

	switch (op)
	{
	case Op_Play:
		is_play_ = true;
		break;
	case Op_Record:
		is_record_ = true;
		break;
	}

	if (!is_thread_working_)
	{
		// 检测线程是否正在运行，若没有，则启动线程，若正在运行，则跳过
		CreateThread(NULL, 0, bgStreamHandler::WorkingThread, this, 0, NULL);
		CreateThread(NULL, 0, bgStreamHandler::EventThread, this, 0, NULL);
	}
	

	return errCode;
}

void bgStreamHandler::Stop()
{
	url_ = "";
}

DWORD WINAPI bgStreamHandler::WorkingThread(LPVOID lpParam)
{
	bgStreamHandler *handler = (bgStreamHandler *)lpParam;

	while (true)
	{
		// 直到外部出发停止标记后，我们停止这个流处理线程

		// 首先开始解复用
		AVFormatContext *input_fmtctx = NULL;
		int errCode = avformat_open_input(&input_fmtctx, handler->url_.c_str(), NULL, NULL);
		if (errCode != 0)
		{
			// 这里需要上报回调
			break;
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

		// 这里应当通报上层，新拉了一个流，流标记为一个GUID，将对应的上下文信息传上去，创建对应的视频文件

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
				if (handler->is_record_)
					handler->notifer_->SaveStreamNotifer(handler->url_.c_str(), &av_packet);

				if (handler->is_play_)
				{
					// 解码后回调给播放回调
					AVFrame *frame = av_frame_alloc();
					int got_pic = 0;
					avcodec_decode_video2(video_codec_ctx, frame, &got_pic, &av_packet);

					if (got_pic)
					{
						// 首先回调发送视频帧信息
						handler->notifer_->PlayingStreamNotifer(handler->url_.c_str(), frame);
					}

					av_frame_free(&frame);
				}

			}
			else if (av_packet.stream_index == audio_stream_index)
			{
				if (handler->is_record_)
					handler->notifer_->SaveStreamNotifer(handler->url_.c_str(), &av_packet);

				if (handler->is_play_)
				{
					// 解码后回调给播放器
					AVFrame *frame = av_frame_alloc();
					int got_sound = 0;
					avcodec_decode_audio4(audio_codec_ctx, frame, &got_sound, &av_packet);

					if (got_sound)
					{
						// 回调音频信息
						handler->notifer_->PlayingStreamNotifer(handler->url_.c_str(), frame);
					}

					av_frame_free(&frame);
				}

			}
		}

		// 如果调到循环外了，应该通知上面发生异常了
		// 然后清理资源，准备就这个源重新进行视频拉取
		avcodec_close(video_codec_ctx);
		avcodec_close(audio_codec_ctx);
		avformat_close_input(&input_fmtctx);
	}

	return 0;
}

DWORD WINAPI bgStreamHandler::EventThread(LPVOID lpParam)
{
	bgStreamHandler *handler = (bgStreamHandler *)lpParam;

	return 0;
}