#ifndef _BG_PLAYER_H_
#define _BG_PLAYER_H_

class bgPlayer
{
public:
	bgPlayer();
	~bgPlayer();

public:
	/**
	 * 设置播放器框架宽度
	 */
	int SetFrameworkWidth(int width);

	/**
	 * 设置播放器框架高度
	 */
	int SetFrameworkHeight(int height);

public:
	/**
	 * 播放
	 * @media_url			媒体资源地址(可以是网络URL，可以是本地路径)
	 * @sub_screen_index	渲染播放子窗口索引，默认为0
	 */
	int Play(const char *media_url);

	// 暂停
	int Pause();

	// 恢复播放
	int Resume();

	// 逐帧播放
	int PlayPerFrame();

	// 倍速播放
	int SpeedPlay(double speed);

	// 跳跃播放
	int SkipPlay(int seconds_offset);
};

#endif//_BG_PLAYER_H_
