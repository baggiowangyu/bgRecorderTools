#ifndef _BG_PLAYER_FRAMEWORK_H_
#define _BG_PLAYER_FRAMEWORK_H_


#include "bgPlayer.h"
#include "SDL.h"


// 最大分屏数，81，即9行9列
#define _max_subscreen_count_	81


/**
 * 播放器框架
 * 向最上层提供播放器框架操作接口，本框架支持多窗口播放
 * 
 */
class bgPlayerFramework
{
public:
	bgPlayerFramework();
	~bgPlayerFramework();

public:

	/**
	 * 设置水平方向上屏幕的个数
	 */
	int SetHorizonScreenCount(int count);

	/**
	 * 设置垂直方向上屏幕的个数
	 */
	int SetVerticalScreenCount(int count);

	/**
	 * 设置播放器框架宽度
	 */
	int SetFrameworkWidth(int width);

	/**
	 * 设置播放器框架高度
	 */
	int SetFrameworkHeight(int height);

	// 初始化子窗口
	int InitializeSubscreens(void* player_hwnd);

	// 重绘子窗口
	int RedrawSubscreens();

public:
	/**
	 * 播放
	 * @media_url			媒体资源地址(可以是网络URL，可以是本地路径)
	 * @sub_screen_index	渲染播放子窗口索引，默认为0
	 */
	int Play(const char *media_url, int sub_screen_index = 0);

	// 暂停
	int Pause(int sub_screen_index = 0);

	// 恢复播放
	int Resume(int sub_screen_index = 0);

	// 逐帧播放
	int PlayPerFrame(int sub_screen_index = 0);

	// 倍速播放
	int SpeedPlay(double speed, int sub_screen_index = 0);

	// 跳跃播放
	int SkipPlay(int seconds_offset, int sub_screen_index = 0);

private:
	int horizon_screen_count_;
	int vertical_screen_count_;

	int framework_width_;
	int framework_height_;

	// 渲染播放子窗口对象集合
	bgPlayer sub_screen_players_[_max_subscreen_count_];
};

#endif//_BG_PLAYER_FRAMEWORK_H_