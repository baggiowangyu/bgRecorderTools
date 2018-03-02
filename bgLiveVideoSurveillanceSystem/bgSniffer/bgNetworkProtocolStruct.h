#ifndef _BG_NETWORK_PROTOCOL_STRUCT_H_
#define _BG_NETWORK_PROTOCOL_STRUCT_H_

#include <string>
#include <WinSock2.h>
#include "..\bgBase\amf.h"


#define ETHERTYPE_IPV4		0x0800 /* ip protocol */
#define ETHERTYPE_ARP		0x0806 /* ip protocol */
#define ETHERTYPE_RARP		0x8035 /* ip protocol */
#define ETHERTYPE_SNMP		0x814C /* ip protocol */
#define ETHERTYPE_IPX		0x8137 /* ip protocol */
#define ETHERTYPE_IPV6		0x86DD /* ip protocol */
#define ETHERTYPE_PPP		0x880B /* ip protocol */

#define TCP_PROTOCAL		0x0600 /* tcp protocol */
#define BUFFER_MAX_LENGTH	65536 /* buffer max length */


#pragma pack(1)

//
// ��̫��V2֡����������
typedef struct bgEthernetII
{
	u_char src_mac_[6];		// ԴMAC��ַ
	u_char des_mac_[6];		// Ŀ��MAC��ַ
	u_short type_;			// �ϲ�����Э������
	// u_char *eth_data_;
} bgEthernetII;

// ��̫��802.3֡����������
typedef bgEthernetII bg802_3;

// ��������֡
typedef struct bgPPPoE
{
	u_short version_;
	u_short pppoe_type_;
	u_short code_;
	u_short session_id_;
	u_short len_;
	std::string next_data_type_;
} bgPPPoE;

// ��ַ����Э��ARP
typedef struct bgARP
{
	u_short hardware_type_;		// Ӳ������
	u_short protocol_type_;
	u_short hardware_len_;
	u_short protocol_len_;
	u_short op_code_;
	u_char src_mac_[6];
	u_char des_mac_[6];
	u_char src_ip_[4];
	u_char des_ip_[4];
} bgARP;


// IPv4Э�顾��������
typedef struct bgIPv4
{
	u_char header_len_	:4;		// IPv4ͷ������
	u_char version		:4;		// �汾
	u_char service_type_;		// �������ͣ�TOS��
	u_short ipv4_len_;			// IPv4���ݰ��ܳ��ȣ��ֽ���������IPv4��ͷ��
	u_short identity_;			// ��ʶ
	u_short flag_		:3;		// ��־
	u_short frag_offset_:13;	// Ƭƫ��
	u_char ttl_;				// ����ʱ��
	u_char protocl_;			// Э��	0x01-ICMPЭ�飻0x06-TCPЭ�飻0x11-UDPЭ�飻
	u_short checksum_;			// IPv4ͷ��У���
	u_char src_ip_[4];			// ԴIP��ַ
	u_char des_ip_[4];			// Ŀ��IP��ַ
	// u_char *options_;		// IPv4ͷ��ѡ���ѡ��
	// u_char *ip_data_;		// IPv4���ݱ���
} bgIPv4;

// IPv6Э��
typedef struct bgIPv6
{
	//u_short version_;
	//u_char traffic_class_;	//ͨ����� 
	//u_char flow_label_[3];	//�����
	//u_short next_len_;		//���س���  ������չͷ���ϲ��غ�
	//u_short protocol_;		//�ϲ�Э�� 
	//u_short hop_limit_;		//�������� 
	//u_char src_ip_[16];		//ԴMAC
	//u_char des_ip_[16];		//Ŀ��MAC
	//u_short ext_len_;		//��չ����
	//u_short sign_;			//����Ƿ������չͷ�� 
} bgIPv6;

// ICMPЭ��
typedef struct bgICMP
{
	//u_short ic_type_;	// ����
	//u_short code_;		// ����
} bgICMP;


//
// UDPЭ�顾��������
typedef struct bgUDP
{
	u_short src_port_;	// Դ�˿�
	u_short des_port_;	// Ŀ�Ķ˿�
	u_short len_;		// �ܳ���
	u_short checksum_;	// У���
	// u_char *udp_data_;
} bgUDP;


// IGMPЭ��
typedef struct bgIGMP
{
	//u_char version_;	// �汾
	//u_char ig_type_;	// ����
	//u_char multicast_;	// D�� ���ַ
} bgIGMP;


//
// TCPЭ�顾��������
typedef struct bgTCP
{
	u_short src_port_;		// Դ�˿�
	u_short des_port_;		// Ŀ�Ķ˿�
	u_int seq_;				// ���
	u_int refirm_seq_;		// ȷ�����
	u_short header_len_	:4;	// ͷ������ * 4
	u_short reserve_	:6;
	u_short urg_		:1;	// ����ָ����Ч
	u_short ack_		:1;	// ȷ���ֶ���Ч
	u_short psh_		:1;	// ��������
	u_short rst_		:1;	// ���Ӹ�λ
	u_short syn_		:1;	// ���ӽ���ʱ���ͬ��
	u_short fin_		:1;	// ��ֹ����
	u_short window_size_;	// ���ڴ�С
	u_short check_sum_;		// У���
	u_short imp_point_;		// ����ָ��
	// u_char *options_;
	// u_char *ipv6_data_;
} bgTCP;



// OSPFЭ��
typedef struct bgOSPF
{
	//u_char version_;			// 
	//u_short ospf_type_;			// 
	//u_short len_;				// 
	//u_char route_ip_[4];		// 
	//u_char area_ip_[4];			// 
	//u_short authen_type_;		// 
	//std::string authen_data_;	// 
} bgOSPF;

// ��������
typedef struct bgDNS
{
	//u_short sign_;				// 
	//u_char symbol_[2];			// 
	//u_char qr_;					// 
	//u_short question_num_;		// 
	//u_short answer_num_;		//
	//u_short authen_answer_;		// 
	//u_short extra_answer_;		// 
	//std::string questions_;		// 
} bgDNS;

// DHCPЭ��
typedef struct bgDHCP
{
	//u_char op_;
	//u_char hardware_type_;
	//u_short hardware_len_;
	//u_short hardware_ops_;		// ����������һ��·������һ
	//int transaction_id_;		// ����ID
	//u_short secs_;				// �ͻ�������ʱ��
	//u_char Ciaddr_[4];
	//u_char Yiaddr_[4];
	//u_char Siaddr_[4];
	//u_char Giaddr_[4];
	//u_char Chaddr_[4];
	//std::string server_name_;
	//std::string file_;
	//u_short message_type_;
	//u_short message_in_;
} bgDHCP;


//
// RTMPЭ��
// ����ֶν������ο�http://www.360doc.com/content/12/0110/16/11192_178547459.shtml
typedef struct bgRTMP
{
	struct bgRtmpHeader
	{
		u_char header_type_;	// ǰ2�ֽ���0xC0����&����ɵõõ���ͷ�ĳ��ȣ���6�ֽں�stream_id_������channel_id��stream_id_ = (channel_id - 4) / 5 + 1
		u_char timestamp_[3];	// ʱ���
		u_char body_size_[3];	// AMFSize ���ݴ�С
		u_char type_id_;		// AMFType ��������
		u_int stream_id_;		// ��ID
	} header_;

	u_char body_[1];
} bgRTMP;


// �û�����4�ֽڵ�IP��ַ  
typedef struct ip_address {  
	u_char byte1;  
	u_char byte2;  
	u_char byte3;  
	u_char byte4;  
}ip_address;  


// ���ڱ���IPV4���ײ�  
typedef struct ip_header {  
	u_char ver_ihl;  
	u_char tos;  
	u_short tlen;  
	u_short identification;  
	u_short flags_fo;  
	u_char ttl;  
	u_char proto;  
	u_short crc;  
	ip_address saddr;  
	ip_address daddr;  
	u_int op_pad;  
}ip_header;  

#pragma pack()



class bgSnifferNotifer
{
public:
	virtual int SnifferResultReport(const char *protocol, const char *value) = 0;
};


#endif//_BG_NETWORK_PROTOCOL_STRUCT_H_
