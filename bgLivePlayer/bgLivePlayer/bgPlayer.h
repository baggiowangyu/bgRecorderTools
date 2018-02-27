#ifndef _BG_PLAYER_H_
#define _BG_PLAYER_H_

class bgPlayer
{
public:
	bgPlayer();
	~bgPlayer();

public:
	/**
	 * ���ò�������ܿ��
	 */
	int SetFrameworkWidth(int width);

	/**
	 * ���ò�������ܸ߶�
	 */
	int SetFrameworkHeight(int height);

public:
	/**
	 * ����
	 * @media_url			ý����Դ��ַ(����������URL�������Ǳ���·��)
	 * @sub_screen_index	��Ⱦ�����Ӵ���������Ĭ��Ϊ0
	 */
	int Play(const char *media_url);

	// ��ͣ
	int Pause();

	// �ָ�����
	int Resume();

	// ��֡����
	int PlayPerFrame();

	// ���ٲ���
	int SpeedPlay(double speed);

	// ��Ծ����
	int SkipPlay(int seconds_offset);
};

#endif//_BG_PLAYER_H_
