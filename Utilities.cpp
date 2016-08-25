#include "Utilities.h"

// Convert IP address into string
std::string GetIpStr( uint32_t ipaddr )
{
	in_addr ip_address;
	ip_address.s_addr = ipaddr;
	std::string ip_str = inet_ntoa( ip_address );
	return ip_str;
}

// Convert MAC address into a string
std::string GetMacStr( uint8_t mac[ETH_ALEN] )
{
	//std::to_string( mac[i] );
	std::string mac_str;
	for( uint32_t i = 0; i < ETH_ALEN-1; i += 1 )
	{
		mac_str += int_to_hex( mac[i] );
		mac_str += ":";
		
		//printf("%s\n", int_to_hex( mac[i] ).c_str() );
	}
	mac_str += int_to_hex( mac[ETH_ALEN-1] );
	return mac_str;
}

template< typename T >
std::string int_to_hex( T i )
{
	std::stringstream stream;
	stream << std::setfill('0') << std::setw( sizeof(T) * 2 ) << std::hex << static_cast<unsigned>(i);
	return stream.str();
}

// Display the entire packet in hex
void hexdump(unsigned char* buffer, size_t size)
{
    for(size_t i = 0; i < size; i += 1)
	{
		if(i % 16 == 0 && i != 0)
		{
			printf("\n");
		}
		else if(i % 8 == 0 && i != 0)
		{
			printf(" ");
		}
		printf("%02x ", buffer[i]);
    }
    printf("\n");
}

// Token can be a string of all tokens to split on.
std::vector<std::string> Split( std::string str_to_split, std::string tok )
{
	std::vector<std::string> tokenized;
	std::string split_str;
	
	for( uint32_t i = 0; i < str_to_split.size(); i += 1 )
	{
		// Check to see if any of the tokens are at "i"
		// position in the string
		bool token_found = false;
		for( uint32_t j = 0; j < tok.size(); j += 1)
		{
			// Token found, add eveything up to
			// this point to the return vector
			if( str_to_split.at(i) == tok.at(j) )
			{
				if( split_str.size() > 0 )
				{
					tokenized.push_back( split_str );
					split_str = "";
				}
				token_found = true;
				break;
			}
		}
		
		if( token_found == false )
		{
			// No tokens found, keep building string
			split_str += str_to_split.at(i);
		}
	}
	
	// Done parsing string, return the vector
	return tokenized;
}

// Checks to see if a given string is made of only hex numbers
bool is_hex(const std::string &str)
{
    return str.find_first_not_of("0123456789abcdefABCDEF") == std::string::npos;
}

// trim from left
std::string& ltrim(std::string& s, const char* t /*= " \t\n\r\f\v"*/)
{
    s.erase(0, s.find_first_not_of(t));
    return s;
}

// trim from right
std::string& rtrim(std::string& s, const char* t /*= " \t\n\r\f\v"*/)
{
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}
