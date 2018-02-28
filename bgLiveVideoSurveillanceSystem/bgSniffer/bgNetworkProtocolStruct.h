#ifndef _BG_NETWORK_PROTOCOL_STRUCT_H_
#define _BG_NETWORK_PROTOCOL_STRUCT_H_


#define ETHERTYPE_IPV4		0x0800 /* ip protocol */
#define ETHERTYPE_ARP		0x0806 /* ip protocol */
#define ETHERTYPE_RARP		0x8035 /* ip protocol */
#define ETHERTYPE_SNMP		0x814C /* ip protocol */
#define ETHERTYPE_IPX		0x8137 /* ip protocol */
#define ETHERTYPE_IPV6		0x86DD /* ip protocol */
#define ETHERTYPE_PPP		0x880B /* ip protocol */

#define TCP_PROTOCAL		0x0600 /* tcp protocol */
#define BUFFER_MAX_LENGTH	65536 /* buffer max length */

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

// 以太网V2帧
typedef struct bgEthernetII
{
	u_char src_mac_[6];		// 源MAC地址
	u_char des_mac_[6];		// 目的MAC地址
	u_short type_;		// 上层网络协议类型
} bgEthernetII;

// 以太网802.3帧
typedef bgEthernetII bg802_3;

// 网线连接帧
typedef struct bgPPPoE
{
	u_short version_;
	u_short pppoe_type_;
	u_short code_;
	u_short session_id_;
	u_short len_;
	std::string next_data_type_;
} bgPPPoE;

// 地址解析协议ARP
typedef struct bgARP
{
	u_short hardware_type_;		// 硬件类型
	u_short protocol_type_;
	u_short hardware_len_;
	u_short protocol_len_;
	u_short op_code_;
	u_char src_mac_[6];
	u_char des_mac_[6];
	u_char src_ip_[4];
	u_char des_ip_[4];
} bgARP;


// IPv4协议
typedef struct bgIPv4
{
	u_short version_;
	u_short header_len_;	//报头长度 *4为头部的长度  头部长度可变 
	u_short service_type_;	//业务类型 
	u_short len_;			//数据包长度 
	u_short sign_;			//标识符 IP包分组时候使用  同一个标识符进行重组
	u_char df_;				//标记是否能分段   1 表示不能分段（不能分段 进行分段 会丢包处理） 
	u_char mf_;				//分段包的结束操作 0 表示分段包的最后一个包 
	u_short offset_;		//分段偏移
	u_char ttl_;			//存活时间
	u_short protocol_;		//下层协议类型
	u_char src_ip_[4];		//源地址
	u_char des_ip_[4];		//目的地址
} bgIPv4;

// IPv6协议
typedef struct bgIPv6
{
	u_short version_;
	u_char traffic_class_;	//通信类别 
	u_char flow_label_[3];	//流标记
	u_short next_len_;		//负载长度  包括扩展头和上层载荷
	u_short protocol_;		//上层协议 
	u_short hop_limit_;		//跳数限制 
	u_char src_ip_[16];		//源MAC
	u_char des_ip_[16];		//目的MAC
	u_short ext_len_;		//扩展长度
	u_short sign_;			//标记是否存在扩展头部 
} bgIPv6;

// ICMP协议
typedef struct bgICMP
{
	u_short ic_type_;	// 类型
	u_short code_;		// 代码
} bgICMP;

// UDP协议
typedef struct bgUDP
{
	u_short src_port_;	// 源端口
	u_short des_port_;	// 目的端口
	u_short len_;		// 总长度
} bgUDP;

// IGMP协议
typedef struct bgIGMP
{
	u_char version_;	// 版本
	u_char ig_type_;	// 类型
	u_char multicast_;	// D类 组地址
};

// TCP协议
typedef struct bgTCP
{
	u_short src_port_;		// 源端口
	u_short des_port_;		// 目的端口
	int seq_;				// 序号
	int refirm_seq_;		// 确认序号
	u_short header_len_;	// 头部长度 * 4
	u_char urg_;			// 紧急指针有效
	u_char ack_;			// 确认字段有效
	u_char psh_;			// 推送数据
	u_char rst_;			// 连接复位
	u_char syn_;			// 连接建立时序号同步
	u_char fin_;			// 终止连接
	u_short window_size_;	// 窗口大小
	u_short imp_point_;		// 紧急指针
} bgTCP;

// OSPF协议
typedef struct bgOSPF
{
	u_char version_;			// 
	u_short ospf_type_;			// 
	u_short len_;				// 
	u_char route_ip_[4];		// 
	u_char area_ip_[4];			// 
	u_short authen_type_;		// 
	std::string authen_data_;	// 
} bgOSPF;

// 域名解析
typedef struct bgDNS
{
	u_short sign_;				// 
	u_char symbol_[2];			// 
	u_char qr_;					// 
	u_short question_num_;		// 
	u_short answer_num_;		//
	u_short authen_answer_;		// 
	u_short extra_answer_;		// 
	std::string questions_;		// 
};

// DHCP协议
typedef struct bgDHCP
{
	u_char op_;
	u_char hardware_type_;
	u_short hardware_len_;
	u_short hardware_ops_;		// 跳数，经过一个路由器加一
	int transaction_id_;		// 事物ID
	u_short secs_;				// 客户机启动时间
	u_char Ciaddr_[4];
	u_char Yiaddr_[4];
	u_char Siaddr_[4];
	u_char Giaddr_[4];
	u_char Chaddr_[4];
	std::string server_name_;
	std::string file_;
	u_short message_type_;
	u_short message_in_;
};


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
