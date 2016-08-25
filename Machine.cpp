#include "Machine.h"

Machine::Machine()
{
	
}

Machine::Machine(std::string recordstring)
{
	std::vector<std::string> tokens = Split( recordstring, "," );
	
	this->name = tokens.at(0);
	this->SetIp( tokens.at(1) );
	this->SetMac( tokens.at(2) );
	this->services = Split( tokens.at(3), ";" );
	
	try
	{
		this->notes = tokens.at(4);
	}
	catch( const std::out_of_range& e )
	{
	}
	
}

Machine::Machine(std::string name, std::string ipaddr, std::string mac, std::string services, std::string notes)
{
	this->name = name;
	this->SetIp( ipaddr );
	this->SetMac( mac );
	this->services = Split( services, ";" );
	this->notes = notes;
}

Machine::~Machine()
{
}

void Machine::display( void )
{
	std::cout << "Name    : " << this->name << "\n";
	std::cout << "IP Addr : " << GetIpStr(this->ipaddr) << "\n";
	std::cout << "MAC Addr: " << GetMacStr(this->mac) << "\n";
	std::cout << "Services: ";
	for(uint32_t i = 0; i < this->services.size(); i += 1 )
	{
		std::cout << this->services.at(i) << " ";
	}
	std::cout << "\n";
	std::cout << "Notes   : " << this->notes << "\n";
}

void Machine::reset( void )
{
	this->name = "";
	this->ipaddr = 0;
	memset( this->mac, 0, ETH_ALEN );
	this->services.clear();
	this->notes = "";
}

void Machine::SetIp( u_int32_t ip )
{
	this->ipaddr = ip;
}

void Machine::SetMac( u_int8_t mac[ETH_ALEN] )
{
	memcpy( this->mac, mac, ETH_ALEN );
}

void Machine::SetIp( std::string ipstr )
{
	this->ipaddr = inet_addr( ipstr.c_str() );
}

void Machine::SetMac( std::string macstr )
{
	memset( this->mac, 0, ETH_ALEN );
	std::string octet;
	uint32_t counter = 0;
	
	for( u_int32_t i=0; i < macstr.size(); i+= 1 )
	{
		if( macstr[i] != ':' )
		{
			octet += macstr[i];
		}
		if( octet.size() == 2 )
		{
			if( is_hex(octet ) )
			{
				this->mac[counter] = atoi( octet.c_str() );
				counter += 1;
				octet = "";
			}
		}
	}
	
	// Not all octets were converted, zero out the mac
	if( counter != ETH_ALEN )
	{
		memset( this->mac, 0, ETH_ALEN );
	}
}

u_int32_t Machine::GetIp(void)
{
	return this->ipaddr;
}

u_int8_t* Machine::GetMac(void)
{
	return this->mac;
}

std::string Machine::GetName(void)
{
	return this->name;
}

std::string Machine::GetNotes(void)
{
	return this->notes;
}

std::vector<std::string>& Machine::GetServices(void)
{
	return this->services;
}

void Machine::SetName(std::string name)
{
	this->name = name;
}

void Machine::SetNotes(std::string notes)
{
	this->notes = notes;
}

void Machine::SetServices( std::vector<std::string> services )
{
	this->services = services;
}
void Machine::AddService( std::string service )
{
	this->services.push_back( service );
}

bool Machine::HasService( std::string service )
{
	for( uint32_t i = 0; i < this->services.size(); i += 1 )
	{
		if( this->services.at(i) == service )
		{
			return true;
		}
	}

	return false;
}