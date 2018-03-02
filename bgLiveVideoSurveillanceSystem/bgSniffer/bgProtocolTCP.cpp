#include "stdafx.h"
#include "bgProtocolTCP.h"
#include "bgNetworkProtocolStruct.h"


int bgProtocolTCP::Parse(unsigned char *header, const unsigned char *data, int size)
{
	int errCode = -2;

	// �õ�TCPͷ��
	struct bgTCP *tcp_header_ = (struct bgTCP *)data;

	// ���TCP��Ϣ
	char trace_msg[4096] = {0};
	sprintf_s(trace_msg, 4096, "Դ�˿ڣ�%d\tĿ��˿ڣ�%d\n",
		ntohs(tcp_header_->src_port_), ntohs(tcp_header_->des_port_));
	//OutputDebugStringA(trace_msg);

	// ����TCP������
	int tcp_header_len = 0;
	if (tcp_header_->header_len_ == 0)
		tcp_header_len = 20;
	else
		tcp_header_len = tcp_header_->header_len_ * 4;

	int tcp_data_len_ = size - tcp_header_len;
	unsigned char *tcp_data_ = (unsigned char *)tcp_header_ + tcp_header_len;
	

	// �ӽ�����TCP��Э��ջ�У����Դ���
	std::vector<bgBasePacketParse *>::iterator iter;
	for (iter = parses_.begin(); iter != parses_.end(); ++iter)
	{
		bgBasePacketParse *parse_ = (bgBasePacketParse *)*iter;
		errCode = parse_->Parse(header, tcp_data_, tcp_data_len_);
		if (errCode != -6002)
		{
			// �Ѿ�������ˡ�����
			break;
		}
	}

	return errCode;
}