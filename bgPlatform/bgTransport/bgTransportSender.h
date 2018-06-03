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
	 * 初始化
	 * 参数：
	 *	@proto		传输协议
	 *	@param		初始化参数
	 * 返回值：
	 *	@0			成功
	 *	@-1			失败
	 */
	int Initialize(enum TransportProtocol proto, const char *param);

	/**
	 * 发送数据
	 * 参数：
	 *	@data		待发送的数据（必须以Base64编码）
	 *	@data_len	待发送的数据长度
	 * 返回值：
	 *	@0			发送成功
	 *	@-1			发送失败
	 */
	int SendData(const char *data, int data_len);
};

#endif//_bgTransportSender_H_
