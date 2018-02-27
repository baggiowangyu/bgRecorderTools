#ifndef _BG_PLAYER_FRAMEWORK_H_
#define _BG_PLAYER_FRAMEWORK_H_


#include "bgPlayer.h"
#include "SDL.h"


// ����������81����9��9��
#define _max_subscreen_count_	81


/**
 * ���������
 * �����ϲ��ṩ��������ܲ����ӿڣ������֧�ֶര�ڲ���
 * 
 */
class bgPlayerFramework
{
public:
	bgPlayerFramework();
	~bgPlayerFramework();

public:

	/**
	 * ����ˮƽ��������Ļ�ĸ���
	 */
	int SetHorizonScreenCount(int count);

	/**
	 * ���ô�ֱ��������Ļ�ĸ���
	 */
	int SetVerticalScreenCount(int count);

	/**
	 * ���ò�������ܿ��
	 */
	int SetFrameworkWidth(int width);

	/**
	 * ���ò�������ܸ߶�
	 */
	int SetFrameworkHeight(int height);

	// ��ʼ���Ӵ���
	int InitializeSubscreens(void* player_hwnd);

	// �ػ��Ӵ���
	int RedrawSubscreens();

public:
	/**
	 * ����
	 * @media_url			ý����Դ��ַ(����������URL�������Ǳ���·��)
	 * @sub_screen_index	��Ⱦ�����Ӵ���������Ĭ��Ϊ0
	 */
	int Play(const char *media_url, int sub_screen_index = 0);

	// ��ͣ
	int Pause(int sub_screen_index = 0);

	// �ָ�����
	int Resume(int sub_screen_index = 0);

	// ��֡����
	int PlayPerFrame(int sub_screen_index = 0);

	// ���ٲ���
	int SpeedPlay(double speed, int sub_screen_index = 0);

	// ��Ծ����
	int SkipPlay(int seconds_offset, int sub_screen_index = 0);

private:
	int horizon_screen_count_;
	int vertical_screen_count_;

	int framework_width_;
	int framework_height_;

	// ��Ⱦ�����Ӵ��ڶ��󼯺�
	bgPlayer sub_screen_players_[_max_subscreen_count_];
};

#endif//_BG_PLAYER_FRAMEWORK_H_