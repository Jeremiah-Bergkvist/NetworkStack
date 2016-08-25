#include "Packet.h"

Packet::Packet(void)
{
    this->eth = (ethhdr*)this->payload;
    this->ip4 = NULL;
    this->tcp = NULL;
    this->udp = NULL;
    this->icmp = NULL;
    this->dns = NULL;
    this->ospf = NULL;
    this->arp = NULL;

    memset(this->payload, 0, PCKT_LEN);
}

// Constructor which accepts another packet and copies it.
Packet::Packet( const Packet &pkt )
{
	// Update the length of copied packet
	this->length = pkt.length;
	
	// Copy the packet data
	for(int i = 0; i < pkt.length; i += 1 )
	{
		this->payload[i] = pkt.payload[i];
	}
	// Null the rest of the packet
	for( int i = pkt.length; i < PCKT_LEN; i += 1 )
	{
		this->payload[i] = 0;
	}
	
	// Set the structure pointers
	this->update_structure();
}


Packet::~Packet(void)
{
    this->eth = NULL;
    this->ip4 = NULL;
    this->udp = NULL;
    this->icmp = NULL;
    this->dns = NULL;
    this->ospf = NULL;
    this->arp = NULL;

    memset(this->payload, 0, PCKT_LEN);
}

// Updates the structures and returns the type of packet
std::string Packet::update_structure(void)
{
    // Reset all internal pointers
    this->eth = (ethhdr*)this->payload;
    this->ip4 = NULL;
    this->tcp = NULL;
    this->udp = NULL;
    this->icmp = NULL;
    this->dns = NULL;
    this->ospf = NULL;
    this->arp = NULL;

    // IPv4
    if(ntohs(this->eth->h_proto) == ETH_P_IP ) {
		// Update known machine info
		this->ip4 = (iphdr*)(this->payload + sizeof(ethhdr));

		// ICMP
		if(this->ip4->protocol == IPPROTO_ICMP ) {
			// Update known machine info
			this->icmp = (icmphdr*)(this->payload + sizeof(ethhdr) + this->ip4->ihl * 4);
			this->type = "ICMP";
			return "ICMP";
		}
		// TCP
		else if(this->ip4->protocol == IPPROTO_TCP ) {
			this->tcp = (tcphdr*)(this->ip4 + this->ip4->ihl * 4);
			this->type = "TCP";
			return "TCP";
		}
		// UDP
		else if(this->ip4->protocol == IPPROTO_UDP ) {
			// Update known machine info
			this->udp = (udphdr*)(this->ip4 + this->ip4->ihl * 4);

			// DNS
			if(ntohs(this->udp->source) == 53) {
				// Update known machine info
				this->dns = (dnshdr*)(this->udp + sizeof(udphdr));
				this->type = "DNS";
				return "DNS";
			}
			this->type = "UDP";
			return "UDP";
		}
		// OSPF
		else if(this->ip4->protocol == IPPROTO_OSPF ) {
			this->ospf = (ospfhdr*)(this->ip4 + this->ip4->ihl * 4);
			this->type = "OSPF";
			return "OSPF";
		}
		this->type = "IPv4";
		return "IPv4";
    }
    // ARP
    else if(ntohs(this->eth->h_proto) == ETH_P_ARP ) {
		this->arp = (arpheader*)(this->payload + sizeof(ethhdr));
		this->type = "ARP";
		return "ARP";
    }
	this->type = "ETH";
    return "ETH";
}

void Packet::display(void)
{
    this->update_structure();

	printf("Packet Length %d (%04X)\n", this->length, this->length);

    // Layer 2 Protocols
    if(this->eth != NULL)
	{
		this->show_eth();
    }

    // Layer 3 Protocols
    if(this->ip4 != NULL)
	{
		this->show_ip4();
    }
	else if(this->arp != NULL)
	{
		this->show_arp();
    }

    // Layer 4 Protocols
    if(this->icmp != NULL)
	{
		this->show_icmp();
    }
	else if(this->tcp != NULL)
	{
		this->show_tcp();
    }
	else if(this->udp != NULL)
	{
		this->show_udp();
    }
	else if(this->ospf != NULL)
	{
		this->show_ospf();
    }

    // Layer 5 Protocols

    // Layer 6 Protocols

    // Layer 7 Protocols
    if(this->dns != NULL)
	{
		this->show_dns();
    }
}

void Packet::show_eth(void)
{
    printf("[Ethernet]\n");
    printf("    | Destination MAC: %s\n", GetMacStr( this->eth->h_dest ).c_str() );
    printf("    | Source MAC     : %s\n", GetMacStr( this->eth->h_source ).c_str() );
    printf("    | Protocol       : %u (%04X)\n", (unsigned short)this->eth->h_proto, ntohs(this->eth->h_proto));
}

void Packet::show_ip4(void)
{
    printf("[IPv4]\n");
    printf("    | Version       : %d\n", (unsigned int)this->ip4->version);
    printf("    | Header Length : %d (%d bytes)\n", (unsigned int)this->ip4->ihl, ((unsigned int)(this->ip4->ihl)) * 4);
    printf("    | TOS           : %d\n", (unsigned int)this->ip4->tos);
    printf("    | Total Length  : %d\n", ntohs(this->ip4->tot_len));
    printf("    | Identification: %d (0x%04X)\n", ntohs(this->ip4->id), ntohs(this->ip4->id));
    printf("    | Time to Live  : %d\n", (unsigned int)this->ip4->ttl);
    printf("    | Protocol      : %d\n", (unsigned int)this->ip4->protocol);
    printf("    | Checksum      : 0x%04X\n", ntohs(this->ip4->check));
    printf("    | Source IP     : %s\n", GetIpStr( this->ip4->saddr ).c_str() );
    printf("    | Destination IP: %s\n", GetIpStr( this->ip4->daddr ).c_str() );
}

void Packet::show_udp(void)
{
    printf("[UDP]\n");
    printf("    | Src Port      : %d\n", ntohs(this->udp->source));
    printf("    | Dst Port      : %d\n", ntohs(this->udp->dest));
    printf("    | Length        : %d\n", ntohs(this->udp->len));
    printf("    | Checksum      : %d (0x%04X)\n", ntohs(this->udp->check), ntohs(this->udp->check));
}

void Packet::show_tcp(void)
{
    printf("[TCP]\n");
    printf("    | Src Port      : %d\n", ntohs(this->tcp->source));
    printf("    | Dst Port      : %d\n", ntohs(this->tcp->dest));
    printf("    | Seq Number    : %d (0x%04X)\n", ntohl(this->tcp->seq), ntohl(this->tcp->seq));
    printf("    | Ack Number    : %d (0x%04X)\n", ntohl(this->tcp->ack), ntohl(this->tcp->ack));
    printf("    | Header Length : %d (%d Bytes)\n", (unsigned int)this->tcp->doff, (unsigned int)this->tcp->doff * 4);
    printf("    | Flags         : ");
    if(this->tcp->urg) {
		printf(" URG ");
    }
    if(this->tcp->ack) {
		printf(" ACK ");
    }
    if(this->tcp->psh) {
		printf(" PSH ");
    }
    if(this->tcp->rst) {
		printf(" RST ");
    }
    if(this->tcp->syn) {
		printf(" SYN ");
    }
    if(this->tcp->fin) {
		printf(" FIN ");
    }
    printf("\n");
    printf("    | Window Number : %d (0x%04X)\n", ntohl(this->tcp->window), ntohl(this->tcp->window));
    printf("    | Checksum      : %d (0x%04X)\n", ntohl(this->tcp->check), ntohl(this->tcp->check));
    printf("    | Urgetn Pointer: %d (0x%04X)\n", ntohl(this->tcp->urg_ptr), ntohl(this->tcp->urg_ptr));
}

void Packet::show_icmp(void)
{
    printf("[ICMP]\n");
    printf("    |-Type     : %d (0x%04X) - ", (unsigned int)(this->icmp->type), (unsigned int)(this->icmp->type));
    if((unsigned int)(this->icmp->type) == ICMP_TIME_EXCEEDED)
	{
		printf("  TTL Expired");
    }
	else if((unsigned int)(this->icmp->type) == ICMP_ECHOREPLY)
	{
		printf("  ICMP Echo Reply");
    }
    printf("\n");

    printf("    | Code     : %d (0x%04X)\n", (unsigned int)(this->icmp->code), (unsigned int)(this->icmp->code));
    printf("    | Checksum : %d (0x%04X)\n", ntohs(this->icmp->checksum), ntohs(this->icmp->checksum));
    printf("    | ID (BE)  : %d (0x%04X)\n", ntohs(this->icmp->un.echo.id), ntohs(this->icmp->un.echo.id));
    // printf("    | ID (LE)  : %d (0x%04X)\n", icmph->id, icmph->id );
    printf("    | Seq (BE) : %d (0x%04X)\n", ntohs(this->icmp->un.echo.sequence), ntohs(this->icmp->un.echo.sequence));
    // printf("    | Seq (LE) : %d (0x%04X)\n", icmph->sequence, icmph->sequence );
}

void Packet::show_arp(void)
{
    printf("[ARP]\n");
    printf("    | Hardware Type : %d (0x%04X)\n", ntohs(this->arp->ar_hrd), ntohs(this->arp->ar_hrd));
    printf("    | Protocol Type : %d (0x%04X)\n", ntohs(this->arp->ar_pro), ntohs(this->arp->ar_pro));
    printf("    | Hardware Len  : %d (0x%04X)\n", this->arp->ar_hln, this->arp->ar_hln);
    printf("    | Protocol Len  : %d (0x%04X)\n", this->arp->ar_pln, this->arp->ar_pln);
    printf("    | Operation     : %d (0x%04X)\n", ntohs(this->arp->ar_op), ntohs(this->arp->ar_op));
    printf("    | Source MAC    : %s\n", GetMacStr( this->arp->__ar_sha ).c_str() );
    printf("    | Source IP     : %s\n", GetIpStr( this->arp->__ar_sip ).c_str() );
	printf("    | Target MAC    : %s\n", GetMacStr( this->arp->__ar_tha ).c_str() );
    printf("    | Target IP     : %s\n", GetIpStr( this->arp->__ar_tip ).c_str() );
}

void Packet::show_ospf(void)
{
    printf("[OSPF]\n");
    printf("    | Version       : %d (0x%04X)\n", (this->ospf->version), (this->ospf->version));
    printf("    | Type          : %d (0x%04X)\n", (this->ospf->type), (this->ospf->type));
    printf("    | Length        : %d (0x%04X)\n", ntohs(this->ospf->length), ntohs(this->ospf->length));

    /*
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
    */
}

void Packet::show_dns(void)
{
    printf("[DNS]\n");
    printf("    | Identification: %d (0x%04X)\n", ntohs(this->dns->id), ntohs(this->dns->id));
}
