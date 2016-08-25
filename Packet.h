#ifndef __PACKET_H__
#define __PACKET_H__

#include "netprotocols.h"
#include "Machine.h"
#include "Utilities.h"

#define PCKT_LEN 8192

//++++++++++++++++++++++++++++++++++
/// Packet Structure
//++++++++++++++++++++++++++++++++++
class Packet{
public:
	// Variables
	u_char payload[PCKT_LEN];
	int length;
	std::string type;	// Final calculated packet type
	
	// Structure pointers
	ethhdr *eth;
	iphdr *ip4;
	tcphdr *tcp;
	udphdr *udp;
	icmphdr *icmp;
	dnshdr *dns;
	ospfhdr *ospf;
	arpheader *arp;
	
	// Display methods
	// Layer 2
	void show_eth( void );
	// Layer 3
	void show_ip4( void );	
	void show_arp( void );
	// Layer 4
	void show_udp( void );
	void show_tcp( void );
	void show_icmp( void );
	void show_ospf( void );
	// Layer 5
	// Layer 6
	// Layer 7
	void show_dns( void );
	
	// Constructor/Destructor
	Packet( void );
	Packet( const Packet &pkt );
	~Packet( void );
	
	// Updates the structure pointers correctly
	std::string update_structure( void );
	void display( void );
};



#endif // __PACKET_H__
