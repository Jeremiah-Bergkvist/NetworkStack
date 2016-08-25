// Must be run by root
#include "main.h"

// Get list of machines to be spoofed
bool load_machineList( std::string filename, std::vector<Machine> &machines )
{
	std::ifstream machinefile(filename, std::ios::in);
	
	if( machinefile.is_open() )
	{
		std::string record;
		while( getline( machinefile, record) )
		{
			// Skip comments
			if( ltrim(record)[0] != '#' )
			{
				machines.push_back( Machine(record) );
			}
		}
		machinefile.close();
		return true;
	}
	else
	{
		perror("Main(): Could not load machine list file");
		return false;
	}
}

bool check_machineList( Packet packet, std::vector<Machine> &machines )
{
	// Check each machine in the spoof list for a match
	for( u_int32_t i = 0; i < machines.size(); i+=1 )
	{
		// Packet is ICMP, check dst IP
		if( packet.type == "ICMP" )
		{
			if( machines.at(i).GetIp() == packet.ip4->daddr )
			{
				packet.display();
				return true;
			}
		}
		
		// Type is ARP, check dst IP
		else if( packet.type == "ARP")
		{
			if( machines.at(i).GetIp() == packet.arp->__ar_tip )
			{
				return true;
			}
		}
	}
	return false;
}

/// Main function
int main(int argc, char *argv[] )
{
	// TODO: Add as CLI arguments
	std::string if_name = "enp0s3";
	std::string machineList_file = "spooflist.csv";
	uint32_t packet_timeout = ARP_REFRESH_RATE;
	
	// Vector to store loaded machines
	std::vector<Machine> machines;
	
	// ARP spoof timeout timer
	time_t start = time(0);
	double delta = 0;
	
	// Load the CSV
	if( load_machineList( machineList_file, machines ) == false )
	{
		printf("Cannot load list of machines to spoof. Exiting...\n");
		exit( EXIT_FAILURE );
	}
	
	// Start the interface with the name to sniff on and list of machines to react to.
	Interface interface( if_name );
	
	// Start main sniffing loop
	printf("Network Stack started.\n");
	while( true )
	{
		// ARP gratuitous keepalive (approximate time)
		delta = difftime( time(0), start );
		if( delta >= packet_timeout )
		{
			// refresh each machine in the machineList
			for( u_int32_t i = 0; i < machines.size(); i += 1)
			{
				interface.send_arp_request( machines.at(i) );
				interface.send_arp_reply( machines.at(i) );
			}
			
			// Reset for the next interval
			start = time(0);
		}
		
		// Check to see if we got a packet
		if( interface.get_packet( packet_timeout ) )
		{
			// See if the packet is something we care about
			if( check_machineList( interface.packet, machines ) )
			{
				// Show packet that is about to be processed
				//interface.packet.display( );
				
				// Respond with a packet sent out over the sniffed interface
				interface.respond( );
			}
		}

	} // End while loop
    return 0;
}

/*
// The different protocol threads
std::thread ospf_thread( handle_ospf );
std::thread arp_thread( handle_arp );
std::thread tcp_thread( handle_tcp );

// Wait until each thread has finished.
ospf_thread.join();
arp_thread.join();
tcp_thread.join();


/// Functions
// Handle OSPF Packet
void handle_ospf(  )
{
    //std::lock_guard<std::mutex> lock(printlock);
    std::cout << "[+] OSPF Packet Handler Started..." << std::endl;
}

// Handle APR Packet
void handle_arp(  )
{
    //std::lock_guard<std::mutex> lock(printlock);
    std::cout << "[+] APR Packet Handler Started..." << std::endl;
}

// Handle TCP Packet
void handle_tcp(  )
{
    //std::lock_guard<std::mutex> lock(printlock);
    std::cout << "[+] TCP Packet Handler Started..." << std::endl;
}

// Handle ICMP Packet
void handle_icmp( Packet &packet, std::vector<Machine> &machines )
{
	
}
*/