#ifndef _Monitor_H_
#define _Monitor_H_

class Monitor
{
public:
	Monitor();
	~Monitor();

public:
	int StartScreenCaptureMonitor();

public:
	static DWORD WINAPI ScreenCaptureMonitor(LPVOID lpParam);
};

#endif//_Monitor_H_
