#ifndef _BG_STREAM_HANDLER_H_
#define _BG_STREAM_HANDLER_H_

#include <Windows.h>

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

enum FrameType
{
	FrameAudio,
	FrameVideo
};

class bgStreamHandlerNotifer
{
public:
	virtual int PlayingStreamNotifer(const char *url, AVFrame *frame, ) = 0;
	virtual int SaveStreamNotifer(const char *url, AVPacket *pkt) = 0;
};

class bgStreamHandler
{
public:
	bgStreamHandler(bgStreamHandlerNotifer *notifer)
		: notifer_(notifer)
	{}
	~bgStreamHandler() {}

public:
	int Initialize();
	void Close();

public:
	int Start(const char *url, enum StreamType type);
	void Stop();

public:
// 	int Pause();
// 	int Resume();

public:
	static DWORD WINAPI WorkingThread(LPVOID lpParam);
	static DWORD WINAPI EventThread(LPVOID lpParam);
	
private:
	bgStreamHandlerNotifer *notifer_;
	enum StreamType type_;
	std::string url_;
};


#endif//_BG_STREAM_HANDLER_H_