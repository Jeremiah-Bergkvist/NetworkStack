#ifndef __NETPROTOCOLS_H__
#define __NETPROTOCOLS_H__

//++++++++++++++++++++++++++++++++++
/// Includes
//++++++++++++++++++++++++++++++++++
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <net/ethernet.h> 
#include <netinet/in.h>
#include <netinet/if_ether.h>  //For ETH_P_ALL
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <netinet/ip_icmp.h>
#include <netpacket/packet.h> // sockaddr_ll

//++++++++++++++++++++++++++++++++++
/// ARP header
//++++++++++++++++++++++++++++++++++
#define IPPROTO_OSPF 89
#define INET_ADDRLEN 4
struct arpheader {
  u_short ar_hrd;						// Hardware type
  u_short ar_pro;						// Protocol type
  u_char  ar_hln;						// Hardware address length
  u_char  ar_pln;						// Protocol address length
  u_short ar_op;						// Operation type				
  u_char  __ar_sha[ETHER_ADDR_LEN];		// Sender hw address
  uint32_t __ar_sip;					// Sender protocol address
  //u_char  __ar_sip[INET_ADDRLEN];		// Sender protocol address
  u_char  __ar_tha[ETHER_ADDR_LEN];		// Target hw address
  uint32_t __ar_tip;					// Target protocol address
  //u_char   __ar_tip[INET_ADDRLEN];		// Target protocol address
  u_char  padding[18];					// Header padding
}__attribute__ ((packed));
/**/

//++++++++++++++++++++++++++++++++++
/// IPv6 header
//++++++++++++++++++++++++++++++++++
struct ipv6_header
{
    unsigned int
        version : 4,
        traffic_class : 8,
        flow_label : 20;
    uint16_t length;
    uint8_t  next_header;
    uint8_t  hop_limit;
    struct in6_addr src;
    struct in6_addr dst;
}__attribute__ ((packed));

//++++++++++++++++++++++++++++++++++
/// IPv4 pseudo header structure use
/// to compute the tcp checksum
//++++++++++++++++++++++++++++++++++
struct pseudo_tcphdr
{
   struct in_addr saddr; 	// source ip
   struct in_addr daddr; 	// destination ip
   uint8_t zeroes; 			// = 0
   uint8_t protocol; 		// = 6
   uint16_t len; 			// length of TCPHeader
} __attribute__ ((packed));

//++++++++++++++++++++++++++++++++++
/// DNS Header
//++++++++++++++++++++++++++++++++++
 struct dnshdr
{
	uint16_t id; // identification number
	uint8_t rd :1; // recursion desired
	uint8_t tc :1; // truncated message
	uint8_t aa :1; // authoritive answer
	uint8_t opcode :4; // purpose of message
	uint8_t qr :1; // query/response flag
	  
	uint8_t rcode :4; // response code
	uint8_t cd :1; // checking disabled
	uint8_t ad :1; // authenticated data
	uint8_t z :1; // its z! reserved
	uint8_t ra :1; // recursion available
	  
	uint16_t q_count; // number of question entries
	uint16_t ans_count; // number of answer entries
	uint16_t auth_count; // number of authority entries
	uint16_t add_count; // number of resource entries
} __attribute__ ((packed));

//Constant sized fields of query structure
struct dns_question
{
	uint16_t qtype;
	uint16_t qclass;
} __attribute__ ((packed));
      
//Constant sized fields of the resource record structure
struct dns_record_data
{
	uint16_t type;
	uint16_t _class;
	uint32_t ttl;
	uint16_t data_len;
} __attribute__ ((packed));
      
//Pointers to resource record contents
struct dns_resource_redord
{
	uint8_t *name;
	dns_record_data *resource;
	uint8_t *rdata;
} __attribute__ ((packed));
      
//Structure of a Query
struct dns_query
{
	uint8_t *name;
	dns_question *queston;
}__attribute__ ((packed));

//++++++++++++++++++++++++++++++++++
/// OSPF Header
//++++++++++++++++++++++++++++++++++
#define OSPF_HEADER_SIZE         24U
#define OSPF_AUTH_SIMPLE_SIZE     8U
#define OSPF_AUTH_MD5_SIZE       16U

#define OSPF_MAX_PACKET_SIZE  65535U	// includes IP Header size.
#define OSPF_HELLO_MIN_SIZE      20U 	// not including neighbors
#define OSPF_DB_DESC_MIN_SIZE     8U
#define OSPF_LS_REQ_MIN_SIZE      0U
#define OSPF_LS_UPD_MIN_SIZE      4U
#define OSPF_LS_ACK_MIN_SIZE      0U

#define OSPF_MSG_HELLO         1  		// OSPF Hello Message.
#define OSPF_MSG_DB_DESC       2  		// OSPF Database Descriptoin Message.
#define OSPF_MSG_LS_REQ        3  		// OSPF Link State Request Message.
#define OSPF_MSG_LS_UPD        4  		// OSPF Link State Update Message.
#define OSPF_MSG_LS_ACK        5  		// OSPF Link State Acknoledgement Message.

#define OSPF_SEND_PACKET_DIRECT         1
#define OSPF_SEND_PACKET_INDIRECT       2
#define OSPF_SEND_PACKET_LOOP           3

#define OSPF_HELLO_REPLY_DELAY          1

#define OSPF_IF_PARAM(O, P) \
        (OSPF_IF_PARAM_CONFIGURED ((O)->params, P)?\
                        (O)->params->P:IF_DEF_PARAMS((O)->ifp)->P)


struct ospf_packet
{
  struct ospf_packet *next;

  // Pointer to data stream.
  struct stream *s;

  // IP destination address.
  struct in_addr dst;

  // OSPF packet length.
  u_int16_t length;
}__attribute__ ((packed));

// OSPF packet queue structure.
struct ospf_fifo
{
  unsigned long count;

  struct ospf_packet *head;
  struct ospf_packet *tail;
}__attribute__ ((packed));

// OSPF packet header structure.
struct ospfhdr
{
  u_char version;                       // OSPF Version.
  u_char type;                          // Packet Type.
  u_int16_t length;                     // Packet Length.
  struct in_addr router_id;             // Router ID.
  struct in_addr area_id;               // Area ID.
  u_int16_t checksum;                   // Check Sum.
  u_int16_t auth_type;                  // Authentication Type.
  // Authentication Data.
  union
  {
    // Simple Authentication.
    u_char auth_data [OSPF_AUTH_SIMPLE_SIZE];
    // Cryptographic Authentication.
    struct
    {
      u_int16_t zero;                   // Should be 0.
      u_char key_id;                    // Key ID.
      u_char auth_data_len;             // Auth Data Length.
      u_int32_t crypt_seqnum;           // Cryptographic Sequence Number.
    } crypt;
  } u;
}__attribute__ ((packed));

// OSPF Hello body format.
struct ospf_hello
{
  struct in_addr network_mask;
  u_int16_t hello_interval;
  u_char options;
  u_char priority;
  u_int32_t dead_interval;
  struct in_addr d_router;
  struct in_addr bd_router;
  struct in_addr neighbors[1];
}__attribute__ ((packed));

// OSPF Database Description body format.
struct ospf_db_desc
{
  u_int16_t mtu;
  u_char options;
  u_char flags;
  u_int32_t dd_seqnum;
}__attribute__ ((packed));


#endif	//  __NETPROTOCOLS_H__