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

// ��̫��V2֡
typedef struct bgEthernetII
{
	u_char src_mac_[6];		// ԴMAC��ַ
	u_char des_mac_[6];		// Ŀ��MAC��ַ
	u_short type_;		// �ϲ�����Э������
} bgEthernetII;

// ��̫��802.3֡
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


// IPv4Э��
typedef struct bgIPv4
{
	u_short version_;
	u_short header_len_;	//��ͷ���� *4Ϊͷ���ĳ���  ͷ�����ȿɱ� 
	u_short service_type_;	//ҵ������ 
	u_short len_;			//���ݰ����� 
	u_short sign_;			//��ʶ�� IP������ʱ��ʹ��  ͬһ����ʶ����������
	u_char df_;				//����Ƿ��ֶܷ�   1 ��ʾ���ֶܷΣ����ֶܷ� ���зֶ� �ᶪ������ 
	u_char mf_;				//�ֶΰ��Ľ������� 0 ��ʾ�ֶΰ������һ���� 
	u_short offset_;		//�ֶ�ƫ��
	u_char ttl_;			//���ʱ��
	u_short protocol_;		//�²�Э������
	u_char src_ip_[4];		//Դ��ַ
	u_char des_ip_[4];		//Ŀ�ĵ�ַ
} bgIPv4;

// IPv6Э��
typedef struct bgIPv6
{
	u_short version_;
	u_char traffic_class_;	//ͨ����� 
	u_char flow_label_[3];	//�����
	u_short next_len_;		//���س���  ������չͷ���ϲ��غ�
	u_short protocol_;		//�ϲ�Э�� 
	u_short hop_limit_;		//�������� 
	u_char src_ip_[16];		//ԴMAC
	u_char des_ip_[16];		//Ŀ��MAC
	u_short ext_len_;		//��չ����
	u_short sign_;			//����Ƿ������չͷ�� 
} bgIPv6;

// ICMPЭ��
typedef struct bgICMP
{
	u_short ic_type_;	// ����
	u_short code_;		// ����
} bgICMP;

// UDPЭ��
typedef struct bgUDP
{
	u_short src_port_;	// Դ�˿�
	u_short des_port_;	// Ŀ�Ķ˿�
	u_short len_;		// �ܳ���
} bgUDP;

// IGMPЭ��
typedef struct bgIGMP
{
	u_char version_;	// �汾
	u_char ig_type_;	// ����
	u_char multicast_;	// D�� ���ַ
};

// TCPЭ��
typedef struct bgTCP
{
	u_short src_port_;		// Դ�˿�
	u_short des_port_;		// Ŀ�Ķ˿�
	int seq_;				// ���
	int refirm_seq_;		// ȷ�����
	u_short header_len_;	// ͷ������ * 4
	u_char urg_;			// ����ָ����Ч
	u_char ack_;			// ȷ���ֶ���Ч
	u_char psh_;			// ��������
	u_char rst_;			// ���Ӹ�λ
	u_char syn_;			// ���ӽ���ʱ���ͬ��
	u_char fin_;			// ��ֹ����
	u_short window_size_;	// ���ڴ�С
	u_short imp_point_;		// ����ָ��
} bgTCP;

// OSPFЭ��
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

// ��������
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

// DHCPЭ��
typedef struct bgDHCP
{
	u_char op_;
	u_char hardware_type_;
	u_short hardware_len_;
	u_short hardware_ops_;		// ����������һ��·������һ
	int transaction_id_;		// ����ID
	u_short secs_;				// �ͻ�������ʱ��
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
