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

	// 初始化
	sdl_window_ = sdl_window;
	sdl_renderer_ = sdl_renderer;

	x_ = x;
	y_ = y;
	width_ = width;
	height_ = height;

	// 创建纹理信息
	sdl_texture_ = SDL_CreateTexture(sdl_renderer_, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, width_, height_);

	// 这里是不是考虑要贴个图？

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

	// 先停止当前播放工作
	Stop();

	url_ = url;

	// 启动新的解码播放线程
	CreateThread(NULL, 0, bgMediaPlayerSubScreen::WorkingThread, this, 0, NULL);
	CreateThread(NULL, 0, bgMediaPlayerSubScreen::EventThread, this, 0, NULL);

	return errCode;
}

int bgMediaPlayerSubScreen::Stop()
{
	int errCode = 0;

	// 修改线程运行标志，等待线程自然退出，如果等待超时了，就强制退出

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