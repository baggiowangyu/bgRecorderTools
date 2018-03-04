#include "stdafx.h"
#include "bgStreamRecorder.h"


int bgStreamRecorder::SetRootPath(const char *rootpath)
{
	root_path_ = root_path_;
	return 0;
}

int bgStreamRecorder::StartRecord(const char *url)
{
	int errCode = 0;

	errCode = notifer_->RecoderNotifer(url, StreamRecordStart, NULL);

	return errCode;
}

int bgStreamRecorder::StopRecord(const char *url)
{
	int errCode = 0;

	errCode = notifer_->RecoderNotifer(url, StreamRecordStop, NULL);

	return errCode;
}

DWORD WINAPI bgStreamRecorder::RecordThread(LPVOID lpParam)
{
	return 0;
}