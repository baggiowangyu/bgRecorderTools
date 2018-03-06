#ifndef _BG_STREAM_HANDLER_H_
#define _BG_STREAM_HANDLER_H_

#include <string>
#include <Windows.h>

#define __STDC_CONSTANT_MACROS

#ifdef __cplusplus
extern "C" {
#endif
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/avutil.h"
#include "libavutil/pixfmt.h"
#include "libswresample/swresample.h"
#include "libswscale/swscale.h"
#ifdef __cplusplus
};
#endif


enum StreamType
{
	StreamTypeLive,
	StreamTypeRecord
};

enum StreamOperator
{
	Op_Record,
	Op_Play
};

enum FrameType
{
	FrameAudio,
	FrameVideo
};

class bgStreamHandlerNotifer
{
public:
	virtual int StreamCodecInfoNotifer(AVCodecContext *video_codec_ctx, AVCodecContext *audio_codec_ctx) = 0;
	virtual int PlayingStreamNotifer(const char *url, AVFrame *frame) = 0;
	virtual int SaveStreamNotifer(const char *url, AVPacket *pkt) = 0;
};

class bgStreamHandler
{
public:
	bgStreamHandler(bgStreamHandlerNotifer *notifer)
		: notifer_(notifer)
		, is_record_(false)
		, is_play_(false)
		, is_thread_working_(false)
	{}
	~bgStreamHandler() {}

public:
	int Initialize();
	void Close();

public:
	int Start(const char *url, enum StreamType type, enum StreamOperator op);
	void Stop();

public:
	static DWORD WINAPI WorkingThread(LPVOID lpParam);
	static DWORD WINAPI EventThread(LPVOID lpParam);
	
private:
	bgStreamHandlerNotifer *notifer_;
	enum StreamType type_;
	std::string url_;

	bool is_record_;
	bool is_play_;

	bool is_thread_working_;
};


#endif//_BG_STREAM_HANDLER_H_