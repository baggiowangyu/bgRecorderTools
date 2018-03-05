#ifndef _BG_MEDIA_PLAYER_SUB_SCREEN_H_
#define _BG_MEDIA_PLAYER_SUB_SCREEN_H_

#include <windows.h>
#include "..\..\ext\SDL2\include\SDL.h"

class bgMediaPlayerSubScreen
{
public:
	bgMediaPlayerSubScreen()
		: x_(0), y_(0), width_(0), height_(0), current_index_(0)
		, sdl_window_(NULL), sdl_renderer_(NULL), sdl_texture_(NULL)
	{}

	~bgMediaPlayerSubScreen() {}

public:
	int Initialize(int x, int y, int width, int height, SDL_Window *sdl_window, SDL_Renderer *sdl_renderer);
	void Close();

public:
	int ChangeSize(int x, int y, int width, int height);

public:
	int Play(const char *url);
	int Stop();

public:
	static DWORD WINAPI WorkingThread(LPVOID lpParam);
	static DWORD WINAPI EventThread(LPVOID lpParam);

public:
	int x_;
	int y_;
	int width_;
	int height_;

	int current_index_;

	SDL_Window *sdl_window_;
	SDL_Renderer *sdl_renderer_;
	SDL_Texture *sdl_texture_;
	SDL_Rect sdl_rect_;

	std::string url_;
};

#endif//_BG_MEDIA_PLAYER_SUB_SCREEN_H_
