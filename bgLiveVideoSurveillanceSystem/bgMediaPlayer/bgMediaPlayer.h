#ifndef _BG_MEDIA_PLAYER_H_
#define _BG_MEDIA_PLAYER_H_

#include "..\..\ext\SDL2\include\SDL.h"
#include "bgMediaPlayerSubScreen.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "libavformat\avformat.h"
#include "libavcodec\avcodec.h"
#ifdef __cplusplus
};
#endif


#define MAX_SUBSCREEN_COUNT 128

enum bgMediaPlayerEvent
{
	Player
};

class bgMediaPlayerNotifer
{
public:
	virtual int PlayNotifer(const char *url, enum bgMediaPlayerEvent event, const unsigned char *data, int data_len) = 0;
};

class bgMediaPlayer
{
public:
	bgMediaPlayer(bgMediaPlayerNotifer *notifer)
		: notifer_(notifer)
		, sdl_window_(NULL)
		, sdl_renderer_(NULL)
		, sub_screen_count_(0)
	{
		
	}

	~bgMediaPlayer() { notifer_ = NULL; }

public:
	int Initialize(int width, int height, int subscreen_count = 1, void *hwnd = NULL);
	void Close();

public:
	int ChangeSubscreenCount(int count);
	int ChangeScreenSize(int width, int height);

public:
	int Play(const char *url, int subscreen_index = 0);
	int Stop(int subscreen_index = 0);
	int StopAll();

private:
	bgMediaPlayerNotifer *notifer_;

private:
	int width_;
	int height_;

	SDL_Window *sdl_window_;
	SDL_Renderer *sdl_renderer_;

	int sub_screen_count_;
	bgMediaPlayerSubScreen sub_screens_[MAX_SUBSCREEN_COUNT];	// 这里简单粗暴一点吧，直接支持128个子屏幕
};

#endif//_BG_MEDIA_PLAYER_H_
