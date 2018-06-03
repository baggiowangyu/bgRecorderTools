#ifndef _bgTransportSender_H_
#define _bgTransportSender_H_

#include "bgTransportProtocol.h"

class bgTransportSender
{
public:
	bgTransportSender();
	~bgTransportSender();

public:
	/**
	 * ��ʼ��
	 * ������
	 *	@proto		����Э��
	 *	@param		��ʼ������
	 * ����ֵ��
	 *	@0			�ɹ�
	 *	@-1			ʧ��
	 */
	int Initialize(enum TransportProtocol proto, const char *param);

	/**
	 * ��������
	 * ������
	 *	@data		�����͵����ݣ�������Base64���룩
	 *	@data_len	�����͵����ݳ���
	 * ����ֵ��
	 *	@0			���ͳɹ�
	 *	@-1			����ʧ��
	 */
	int SendData(const char *data, int data_len);
};

#endif//_bgTransportSender_H_
