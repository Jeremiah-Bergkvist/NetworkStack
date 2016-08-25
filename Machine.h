#ifndef __MACHINE_H__
#define __MACHINE_H__

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdexcept>
#include "netprotocols.h"
#include "Utilities.h"

class Machine
{
private:
	uint32_t ipaddr;
	uint8_t mac[ETH_ALEN];
	std::string name;
	std::vector<std::string> services;
    std::string notes;
	
public:
    Machine(std::string name, std::string ipaddr, std::string mac, std::string service, std::string notes);
	Machine(std::string name, uint32_t ipaddr, uint8_t mac[ETH_ALEN], std::string service, std::string notes);
	Machine(uint32_t ipaddr, uint8_t mac[ETH_ALEN]);
    Machine(std::string recordstring);
    Machine();
    ~Machine();
	
	void display( void );
	void reset( void );
	bool HasService( std::string service );
	
	// Setters and Getters
	u_int32_t GetIp( void );
	u_int8_t* GetMac( void );
	
	std::string GetName( void );
	std::vector<std::string>& GetServices(void);
	std::string GetNotes( void );
	
	void SetIp( std::string ipstr );
    void SetMac( std::string macstr );
	void SetIp( u_int32_t ip );
    void SetMac( u_int8_t mac[ETH_ALEN] );
	
	void SetName( std::string name );
	void SetServices( std::vector<std::string> services );
	void AddService( std::string service );
	void SetNotes( std::string notes );
};

#endif // __MACHINE_H__
