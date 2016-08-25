#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <vector>
#include <string>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iomanip>
#include <sstream>
#include <iostream>

std::string GetMacStr( uint8_t mac[ETH_ALEN] );
std::string GetIpStr( uint32_t ipaddr );

void hexdump(uint8_t* buffer, size_t size);
bool is_hex(const std::string &str);

std::vector<std::string> Split( std::string str_to_split, std::string tok );
std::string& ltrim(std::string& s, const char* t = " \t\n\r\f\v");
std::string& rtrim(std::string& s, const char* t = " \t\n\r\f\v");

template< typename T > std::string int_to_hex( T i );


#endif // __UTILITIES_H__
