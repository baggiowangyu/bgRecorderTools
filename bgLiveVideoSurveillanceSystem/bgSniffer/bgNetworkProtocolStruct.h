#ifndef _BG_NETWORK_PROTOCOL_STRUCT_H_
#define _BG_NETWORK_PROTOCOL_STRUCT_H_

// ��̫��V2֡
// ��̫��802.3֡
// ��������֡
// ��ַ����Э��ARP
// IPЭ��
// IPv6Э��
// ICMPЭ��
// UDPЭ��
// IGMPЭ��
// TCPЭ��
// OSPFЭ��
// ��������Э��DNS
// DHCPЭ��
// ���ı�����Э��HTTP
// FTPЭ��
// ���ʼ�Э��SMTP

//struct bgEthernetII
//{
//	u_char src_mac_[6];		// ԴMAC��ַ
//	u_char des_mac_[6];		// Ŀ��MAC��ַ
//	u_char kind_[6];		// �ϲ�����Э������
//};
//
//typedef bgEthernetII bg802_3;
//
//struct bgPPPoE
//{
//	u_short version_;
//	u_short pppoe_type_;
//	u_short code_;
//	u_short session_id_;
//	u_short len_;
//	std::string next_data_type_;
//};
//
//struct bgARP
//{
//	u_short 
//};

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

#endif//_BG_NETWORK_PROTOCOL_STRUCT_H_
