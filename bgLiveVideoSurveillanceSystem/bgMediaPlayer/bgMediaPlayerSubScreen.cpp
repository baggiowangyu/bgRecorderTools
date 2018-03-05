#include "stdafx.h"
#include "bgMediaPlayerSubScreen.h"
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


int bgMediaPlayerSubScreen::Initialize(int x, int y, int width, int height, SDL_Window *sdl_window, SDL_Renderer *sdl_renderer)
{
	int errCode = 0;

	// ��ʼ��
	sdl_window_ = sdl_window;
	sdl_renderer_ = sdl_renderer;

	x_ = x;
	y_ = y;
	width_ = width;
	height_ = height;

	// ����������Ϣ
	sdl_texture_ = SDL_CreateTexture(sdl_renderer_, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, width_, height_);

	// �����ǲ��ǿ���Ҫ����ͼ��

	return errCode;
}

void bgMediaPlayerSubScreen::Close()
{

}

int bgMediaPlayerSubScreen::ChangeSize(int x, int y, int width, int height)
{
	int errCode = 0;

	return errCode;
}

int bgMediaPlayerSubScreen::Play(const char *url)
{
	int errCode = 0;

	// ��ֹͣ��ǰ���Ź���
	Stop();

	url_ = url;

	// �����µĽ��벥���߳�
	CreateThread(NULL, 0, bgMediaPlayerSubScreen::WorkingThread, this, 0, NULL);
	CreateThread(NULL, 0, bgMediaPlayerSubScreen::EventThread, this, 0, NULL);

	return errCode;
}

int bgMediaPlayerSubScreen::Stop()
{
	int errCode = 0;

	// �޸��߳����б�־���ȴ��߳���Ȼ�˳�������ȴ���ʱ�ˣ���ǿ���˳�

	return errCode;
}

DWORD WINAPI bgMediaPlayerSubScreen::WorkingThread(LPVOID lpParam)
{
	bgMediaPlayerSubScreen *subscreen = (bgMediaPlayerSubScreen *)lpParam;

	return 0;
}

DWORD WINAPI bgMediaPlayerSubScreen::EventThread(LPVOID lpParam)
{
	return 0;
}