#include "Interface.h"

// IP and ICMP Checksum function
uint16_t checksum( const uint16_t *buf, uint32_t count )
{
	register unsigned long sum = 0;
	
	while( count > 1 )
	{
		sum += *buf++;
		count -= sizeof(uint16_t) ;
	}
	
	if( count )
	{
		sum += *(uint8_t*) buf;
	}
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	
	return (uint16_t)(~sum);
}

// TCP checksum function


// Constructor
Interface::Interface(std::string if_name)
{
    // Ensure the passed string is correct length
    if_name.resize(IFNAMSIZ - 1);

    // Open socket
    this->sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if(this->sock == -1)
	{
		perror("Interface(): Couldn't create socket.");
		exit(EXIT_FAILURE);
    }
	
	// Store nic interface information
	struct ifreq ifopts;
	memset(&ifopts, 0, sizeof(ifreq) );
    strncpy( ifopts.ifr_name, if_name.c_str(), IFNAMSIZ-1 );
	
	// Get the current state of the card flags
	if( ioctl( this->sock, SIOCSIFFLAGS, &ifopts ) == -1 )
	{
		perror("Interface(): Couldn't get interface flags.");
		close(this->sock);
		exit(EXIT_FAILURE);
	}
	
	// Set the interface to up
    ifopts.ifr_flags |= IFF_UP;
	if( ioctl( this->sock, SIOCSIFFLAGS, &ifopts ) == -1 )
	{
		perror("Interface(): Couldn't set interface flags.");
		close(this->sock);
		exit(EXIT_FAILURE);
	}
	
	// Set the interface to promiscuous mode
	ifopts.ifr_flags |= IFF_PROMISC;
	if( ioctl( this->sock, SIOCSIFFLAGS, &ifopts ) == -1 )
	{
		perror("Interface(): Couldn't set interface flags.");
		close(this->sock);
		exit(EXIT_FAILURE);
	}
	
	// Get interface mac address
	if( ioctl(this->sock, SIOCGIFHWADDR, &ifopts) == -1 )
	{
		perror("Interface(): Couldn't get interface MAC address.");
		close(this->sock);
		exit(EXIT_FAILURE);
	}
	memcpy( &this->mac, &ifopts.ifr_hwaddr.sa_data, ETH_ALEN);
	
	// Get interface ip address
	if( ioctl(this->sock, SIOCGIFADDR, &ifopts) == -1 )
	{
		perror("Interface(): Couldn't get interface IP address.");
		close(this->sock);
		exit(EXIT_FAILURE);
	}
	this->ipv4 = ((sockaddr_in*)(&ifopts.ifr_addr))->sin_addr.s_addr;
	
	// Get interface subnet mask
	if( ioctl(this->sock, SIOCGIFNETMASK, &ifopts) == -1 )
	{
		perror("Interface(): Couldn't get interface subnet mask.");
		close(this->sock);
		exit(EXIT_FAILURE);
	}
	this->mask = ((sockaddr_in*)(&ifopts.ifr_addr))->sin_addr.s_addr;
	
	// Get interface number used by the kernel
	if( ioctl(this->sock, SIOCGIFINDEX, &ifopts) == -1 )
	{
		perror("Interface(): Couldn't get interface number.");
		close(this->sock);
		exit(EXIT_FAILURE);
	}
	
    // Bind socket to specified interface
    memset(&this->interfaceAddress, 0, sizeof(this->interfaceAddress));
    this->interfaceAddress.sll_family = AF_PACKET;
    this->interfaceAddress.sll_ifindex = ifopts.ifr_ifindex;
    // interfaceAddress.sll_protocol = htons(ETH_P_IP);
	this->address_length = sizeof(this->interfaceAddress);
    if(bind(this->sock, (struct sockaddr*)&this->interfaceAddress, this->address_length) == -1)
	{
		perror("Interface(): Couldn't bind to interface");
		close(this->sock);
		exit(EXIT_FAILURE);
    }

	/*
    // Allow the socket to be reused if connection is lost
    int socket_options;
    if( setsockopt(this->socket_descriptor, SOL_SOCKET, SO_REUSEADDR, &this->socket_options,
    sizeof(this->socket_options))== -1 )
    {
            perror("Interface(): Couldn't set socket options.");
            close( this->socket_descriptor );
            exit( EXIT_FAILURE );
    }
	*/
    //perror("Interface()");
}

// Destructor
Interface::~Interface()
{
    close(this->sock);
}

// Monitor Interface
// Default of no timeout
 bool Interface::get_packet( uint32_t timeout )
{
	FD_ZERO( &this->select_fds );
	FD_SET( this->sock, &this->select_fds );
	
	this->select_timeout.tv_sec = timeout;
	this->select_timeout.tv_usec = 0;
	
	// -1 error, 0 timeout, else read data. timeout modified after call
	int select_result = select(this->sock+1, &this->select_fds, NULL, NULL, &this->select_timeout);
	if( select_result == -1 )
	{
		perror("Interface(): Couldn't read data from socket.");
		close(this->sock);
		exit( EXIT_FAILURE );
	}
	// Data available
	else if( select_result > 0 )
	{
		this->packet.length = recvfrom( this->sock, &this->packet.payload, PCKT_LEN, 0, (sockaddr*)&this->interfaceAddress, &this->address_length);
		//this->pkt.payload_length = recv( this->sock, &this->pkt.payload, PCKT_LEN, 0);
		if( this->packet.length < 1 )
		{
			perror( "Interface(): -1 from recvfrom() ");
			exit( EXIT_FAILURE );
		}
	}
	// Timeout triggered
	else
	{
		return false;
	}
	
	/*
	// Clear packet before using
	//this->packet.reset();
	this->pkt.payload_length = recvfrom( this->sock, &this->pkt.payload, PCKT_LEN, 0, (sockaddr*)&this->interfaceAddress, &this->address_length);
	if( this->pkt.payload_length == -1 && errno != EAGAIN)
	{
		perror("Interface(): Couldn't read data from socket.");
		close(this->socket_descriptor);
		exit( EXIT_FAILURE );
	}
	// TODO: May need to add if getting a lot packets originating from the machine itself.
	// Outbound packet
	else if(this->interfaceAddress.sll_pkttype == PACKET_OUTGOING)
	 {
		// Skip any outbound packets
		continue;
	}
	// Inbound packet
	else
	{
	} // end outbound packet
	*/
	this->packet.update_structure();
    return true;
}

// Replies to the remote machine based on packet contents
void Interface::respond( void )
{
    this->packet.update_structure();

    // Layer 2 Protocols
    if(this->packet.eth != NULL)
	{
		// Layer 3 Protocols
		if(this->packet.ip4 != NULL)
		{
			if(this->packet.icmp != NULL)
			{
				// Process ICMP response
				this->process_icmp( );
			}
			else if(this->packet.udp != NULL)
			{
				if(this->packet.dns != NULL)
				{
					// Process DNS
				}
			}
			else if( this->packet.ospf != NULL )
			{
				// Process OSPF
			}
		}
		else if(this->packet.arp != NULL)
		{
			// Process ARP response
			this->process_arp();
		}
    }
}

void Interface::process_icmp( void )
{
	// Copy old packet to new packet
	Packet outpacket( this->packet );
	
	// Ethernet
	memcpy( outpacket.eth->h_dest, this->packet.eth->h_source, ETHER_ADDR_LEN );
	memcpy( outpacket.eth->h_source, this->packet.eth->h_dest , ETHER_ADDR_LEN );
	
	// IPv4 - Swap ip addresses
	outpacket.ip4->daddr = this->packet.ip4->saddr;
	outpacket.ip4->saddr = this->packet.ip4->daddr;
	
	// Flags, frag, ttl, and checksum
	outpacket.ip4->ttl -= IPTTLDEC;
	outpacket.ip4->frag_off = 0;
	outpacket.ip4->check = 0;
	outpacket.ip4->check = checksum( (uint16_t*)outpacket.ip4, ntohs(outpacket.ip4->tot_len) );
	
	// ICMP - set type, code and checksum
	outpacket.icmp->type = 0;
	outpacket.icmp->code = 0;
	outpacket.icmp->checksum = 0;
	outpacket.icmp->checksum = checksum( (uint16_t*)outpacket.icmp, ntohs(outpacket.ip4->tot_len) - outpacket.ip4->ihl * 4 );
	
	// Send the response
	outpacket.display();
	send(this->sock, outpacket.payload, outpacket.length, 0 );
	perror("[ARP] Send()");
}

void Interface::process_arp( void )
{
	// Only process if there is an ARP request
	if( this->packet.arp->ar_op != htons( ARPOP_REQUEST ) )
	{
		return;
	}
	
	// Packet to be sent
	Packet outpacket( this->packet );
	
	// Ethernet data
	// Source as destination
	memcpy( outpacket.eth->h_dest, &this->packet.arp->__ar_sha, ETH_ALEN );
	
	// Destination as machine to imitate
	memcpy( outpacket.eth->h_source, &this->mac, ETH_ALEN );
	
	// Ethernet next protocol (ARP)
	outpacket.eth->h_proto = htons( ETH_P_ARP );
	
	// ARP data
	outpacket.arp->ar_hrd = htons( ARPHRD_ETHER );
	outpacket.arp->ar_pro = htons( ETH_P_IP );
	outpacket.arp->ar_hln = ETH_ALEN;
	outpacket.arp->ar_pln = INET_ADDRLEN;
	
	// ARP response
	outpacket.arp->ar_op = htons( ARPOP_REPLY );
	
	// ARP Sender MAC (target machine to spoof)
	memcpy( &outpacket.arp->__ar_sha, &this->mac, ETH_ALEN );
	
	// APR Sender IP (machine we are spoofing)
	outpacket.arp->__ar_sip = this->packet.arp->__ar_tip;
	
	// ARP Target MAC
	memcpy( &outpacket.arp->__ar_tha, &this->packet.arp->__ar_sha, ETH_ALEN );
	
	// APR Target IP
	outpacket.arp->__ar_tip = this->packet.arp->__ar_sip;
	
	// Send the response
	outpacket.display();	
	send(this->sock, outpacket.payload, outpacket.length, 0 );
	//write(this->sock, &outpacket.payload, outpacket.length );
}

void Interface::send_arp_request( Machine &machine )
{
	/*
	Packet arp_notice;
	
	// Ethernet data
	memcpy( &arp_notice.eth->h_dest, &gateway_mac, ETH_ALEN );
	memcpy( &arp_notice.eth->h_source, machine.GetMac(), ETH_ALEN );
	arp_notice.eth->h_proto = htons( ETH_P_ARP ); 
	
	// ARP data
	arp_notice.arp->ar_hrd = htons( ARPHRD_ETHER );
	arp_notice.arp->ar_pro = htons( ETH_P_IP );
	arp_notice.arp->ar_hln = ETH_ALEN;
	arp_notice.arp->ar_pln = INET_ADDRLEN;
	arp_notice.arp->ar_op = htons( ARPOP_REQUEST );
	
	// Zero target mac, since it is not known yet
	memset( &arp_notice.arp->__ar_tha, 0, ETH_ALEN );
	
	// Destination and Source MAC Addresses
	//memcpy( &arp_notice.arp->__ar_tha, 0xFF, ETH_ALEN );
	memcpy( &arp_notice.arp->__ar_sha, machine.GetMac(), ETH_ALEN );
	//memset( &outpacket.arp->__ar_tha, 0xFF,  ETH_ALEN );
	//memset( &outpacket.arp->__ar_sip, 0, INET_ADDRLEN );
	 * 
	// Update the total length of data to be sent
	arp_notice.payload_length = sizeof( ethhdr ) + sizeof( arpheader );
	
	arp_notice.display();
	send(this->sock, arp_notice.payload, arp_notice.payload_length, 0 );
	 */
}


void Interface::send_arp_reply( Machine &machine )
{
	/*
	Packet arp_notice;
	
	// Ethernet data
	memcpy( &arp_notice.eth->h_dest, &gateway_mac, ETH_ALEN );
	memcpy( &arp_notice.eth->h_source, machine.GetMac(), ETH_ALEN );
	arp_notice.eth->h_proto = htons( ETH_P_ARP ); 
	
	// ARP data
	arp_notice.arp->ar_hrd = htons( 1 );
	arp_notice.arp->ar_pro = htons( ETH_P_IP );
	arp_notice.arp->ar_hln = ETH_ALEN;
	arp_notice.arp->ar_pln = INET_ADDRLEN;
	arp_notice.arp->ar_op = htons( ARPOP_REQUEST );
	
	// Zero target mac, since it is not known yet
	memset( &arp_notice.arp->__ar_tha, 0, ETH_ALEN );
	
	// Destination and Source MAC Addresses
	//memcpy( &arp_notice.arp->__ar_tha, 0xFF, ETH_ALEN );
	memcpy( &arp_notice.arp->__ar_sha, machine.GetMac(), ETH_ALEN );
	
	// Update the total length of data to be sent
	arp_notice.payload_length = sizeof( ethhdr ) + sizeof( arpheader );
	
	arp_notice.display();
	send(this->sock, arp_notice.payload, arp_notice.payload_length, 0 );
	 */
}
