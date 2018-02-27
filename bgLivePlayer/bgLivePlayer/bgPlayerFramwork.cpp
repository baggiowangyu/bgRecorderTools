#include "stdafx.h"
#include "bgPlayerFramwork.h"
#include "bgPlayer.h"




bgPlayerFramework::bgPlayerFramework()
{
	
}

bgPlayerFramework::~bgPlayerFramework()
{
	SDL_Quit();
}


int bgPlayerFramework::SetHorizonScreenCount(int count)
{
	// 重绘子窗口
	horizon_screen_count_ = count;
	return 0;
}

int bgPlayerFramework::SetVerticalScreenCount(int count)
{
	// 重绘子窗口
	vertical_screen_count_ = count;
	return 0;
}

int bgPlayerFramework::SetFrameworkWidth(int width)
{
	// 重绘子窗口
	framework_width_ = width;
	return RedrawSubscreens();
}

int bgPlayerFramework::SetFrameworkHeight(int height)
{
	// 重绘子窗口
	framework_height_ = height;
	return RedrawSubscreens();
}

int bgPlayerFramework::InitializeSubscreens(void* player_hwnd)
{
	int errCode = SDL_Init(SDL_INIT_TIMER|SDL_INIT_AUDIO|SDL_INIT_VIDEO|SDL_INIT_EVENTS);
	return 0;
}

int bgPlayerFramework::RedrawSubscreens()
{
	return 0;
}

int bgPlayerFramework::Play(const char *media_url, int sub_screen_index /* = 0 */)
{
	return sub_screen_players_[sub_screen_index].Play(media_url);
}

int bgPlayerFramework::Pause(int sub_screen_index /* = 0 */)
{
	return sub_screen_players_[sub_screen_index].Pause();
}

int bgPlayerFramework::Resume(int sub_screen_index /* = 0 */)
{
	return sub_screen_players_[sub_screen_index].Resume();
}

int bgPlayerFramework::PlayPerFrame(int sub_screen_index /* = 0 */)
{
	return sub_screen_players_[sub_screen_index].PlayPerFrame();
}

int bgPlayerFramework::SpeedPlay(double speed, int sub_screen_index /* = 0 */)
{
	return sub_screen_players_[sub_screen_index].SpeedPlay(speed);
}

int bgPlayerFramework::SkipPlay(int seconds_offset, int sub_screen_index /* = 0 */)
{
	return sub_screen_players_[sub_screen_index].SkipPlay(seconds_offset);
}
