#ifndef _BG_NETWORK_PROTOCOL_STRUCT_H_
#define _BG_NETWORK_PROTOCOL_STRUCT_H_

// 以太网V2帧
// 以太网802.3帧
// 网线连接帧
// 地址解析协议ARP
// IP协议
// IPv6协议
// ICMP协议
// UDP协议
// IGMP协议
// TCP协议
// OSPF协议
// 域名解析协议DNS
// DHCP协议
// 超文本传输协议HTTP
// FTP协议
// 简单邮件协议SMTP

//struct bgEthernetII
//{
//	u_char src_mac_[6];		// 源MAC地址
//	u_char des_mac_[6];		// 目的MAC地址
//	u_char kind_[6];		// 上层网络协议类型
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

// 用户保存4字节的IP地址  
typedef struct ip_address {  
	u_char byte1;  
	u_char byte2;  
	u_char byte3;  
	u_char byte4;  
}ip_address;  


// 用于保存IPV4的首部  
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
