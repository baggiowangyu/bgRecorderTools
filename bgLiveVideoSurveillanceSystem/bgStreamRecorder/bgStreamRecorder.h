#ifndef _BG_STREAM_RECORDER_H_
#define _BG_STREAM_RECORDER_H_

#include <string>
#include <map>
#include <Windows.h>


enum StreamRecordEvent
{
	StreamRecordUpdateRootpath,
	StreamRecordUpdateRecordInfo,
	StreamRecordStart,
	StreamRecordPause,
	StreamRecordStop
};

// 录像信息
struct bgRecordInfo
{
	std::string save_path_;		// 保存路径
	__int64 record_data_len_;	// 已录制长度
};


class bgStreamRecorderNotifer
{
public:
	virtual int RecoderNotifer(const char *url, enum StreamRecordEvent event, unsigned char *info) = 0;
};

class bgStreamRecorder
{
public:
	bgStreamRecorder(bgStreamRecorderNotifer *notifer)
		: notifer_(notifer)
	{}

	~bgStreamRecorder() { notifer_ = NULL; }

public:
	int SetRootPath(const char *rootpath);

public:
	int StartRecord(const char *url);
	int StopRecord(const char *url);

public:
	static DWORD WINAPI RecordThread(LPVOID lpParam);

public:
	std::string root_path_;
	std::map<std::string, struct bgRecordInfo> records_;

private:
	bgStreamRecorderNotifer *notifer_;
};

#endif//_BG_STREAM_RECORDER_H_