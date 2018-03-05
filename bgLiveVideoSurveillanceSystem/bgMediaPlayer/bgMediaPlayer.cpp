#include "stdafx.h"
#include "bgMediaPlayer.h"

int bgMediaPlayer::Initialize(int width, int height, int subscreen_count /*= 1*/, void *hwnd /*= NULL*/)
{
	int errCode = 0;

	width_ = width;
	height_ = height;
	sub_screen_count_ = subscreen_count;

	// ��ʼ��SDL���
	errCode = SDL_Init(SDL_INIT_TIMER|SDL_INIT_AUDIO|SDL_INIT_VIDEO|SDL_INIT_EVENTS);
	if (errCode != 0)
		return errCode;

	// ����SDL����
	sdl_window_ = SDL_CreateWindowFrom(hwnd);
	if (!sdl_window_)
		return -1;

	// ����SDL��Ⱦ��
	sdl_renderer_ = SDL_CreateRenderer(sdl_window_, -1, 0);
	if (!sdl_renderer_)
		return -2;

	// ���������ݷ����������Ӵ���
	if (subscreen_count == 1)
	{
		// ֻ��һ���Ӵ��壬�����Ӵ����ԭ������Ϳ�߶�
		errCode = sub_screens_[0].Initialize(0, 0, width, height, sdl_window_, sdl_renderer_);
	}
	else 
	{
		// �ݲ�����
	}

	return errCode;
}

void bgMediaPlayer::Close()
{
	for (int index = 0; index < MAX_SUBSCREEN_COUNT; ++index)
	{
		sub_screens_[index].Stop();
		sub_screens_[index].Close();
	}

	SDL_Quit();
}

int bgMediaPlayer::ChangeSubscreenCount(int count)
{
	int errCode = 0;

	return errCode;
}

int bgMediaPlayer::ChangeScreenSize(int width, int height)
{
	int errCode = 0;

	return errCode;
}

int bgMediaPlayer::Play(const char *url, int subscreen_index /*= 0*/)
{
	int errCode = sub_screens_[subscreen_index].Play(url);
	return errCode;
}

int bgMediaPlayer::Stop(int subscreen_index /* = 0 */)
{
	int errCode = sub_screens_[subscreen_index].Stop();
	return errCode;
}

int bgMediaPlayer::StopAll()
{
	for (int index = 0; index < MAX_SUBSCREEN_COUNT; ++index)
		sub_screens_[index].Stop();
	
	return 0;
}