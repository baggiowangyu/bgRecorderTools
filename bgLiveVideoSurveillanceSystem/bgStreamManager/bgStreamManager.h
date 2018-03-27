#ifndef _BG_STREAM_MANAGER_H_
#define _BG_STREAM_MANAGER_H_

#include <string>
#include <map>
#include "bgStreamHandler.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavfilter/avfilter.h"
#include "libavutil/avutil.h"
#include "libswresample/swresample.h"
#include "libswscale/swscale.h"
#ifdef __cplusplus
};
#endif

class bgStreamHandler;

enum STREAM_NOTIFY_TYPE
{
	StreamPlay,
	StreamSave
};

class bgStreamNotifer
{
public:
	virtual int StreamNotifer(enum STREAM_NOTIFY_TYPE msg_type, const char *url) = 0;
};

class bgStreamManager : public bgStreamHandlerNotifer
{
public:
	bgStreamManager(bgStreamNotifer *notifer)
		: notifer_(notifer)
	{}

	~bgStreamManager()
	{
		notifer_ = NULL;
	}

public:
	int SetSaveRootPath(const char *rootpath);
	int HandleURL(const char *url, bool isPlay, bool isSave);

public:
	virtual int StreamCodecInfoNotifer(AVCodecContext *video_codec_ctx, AVCodecContext *audio_codec_ctx);
	virtual int PlayingStreamNotifer(const char *url, AVFrame *frame);
	virtual int SaveStreamNotifer(const char *url, AVPacket *pkt);

public:
	std::string root_path_;
	std::map<std::string, bgStreamHandler*> stream_urls_;

private:
	bgStreamNotifer *notifer_;
	
};

#endif//_BG_STREAM_MANAGER_H_