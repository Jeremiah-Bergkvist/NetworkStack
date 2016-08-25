#ifndef __INTERFACE_H__
#define __INTERFACE_H__


#include "netprotocols.h"
#include "Packet.h"
#include "Machine.h"
#include "Utilities.h"
#include <cstring>
#include <iostream>
#include <vector>
#include <fstream>
#include <fcntl.h>
#include <sys/poll.h>

class Interface
{
private:
	int sock;
	fd_set select_fds;
	timeval select_timeout;
	uint8_t mac[ETH_ALEN];
	uint32_t ipv4;
	uint32_t mask;
	
	// TODO: Replace readfrom() with read()?
	struct sockaddr_ll interfaceAddress;
	socklen_t address_length;
	
public:
	Packet packet;

	Interface( std::string if_name );
	~Interface();
	
	// Populates a packet and returns a reference to it
	bool get_packet( uint32_t timeout=0 );
	void respond( void );
	
	// Protocol generators
	void send_arp_request( Machine &machine );
	void send_arp_reply( Machine &machine );
	
	// Protocol processors
	void process_icmp( void );
	void process_arp( void );
};

#endif // __INTERFACE_H__
