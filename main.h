#ifndef MAIN_H
#define MAIN_H

/// C includes
#include <stdio.h>

// Multithreading
#include <thread>
#include <mutex>

/// C++ Includes
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <vector>

/// Globals
//extern std::mutex printlock;

// Packet Type Includes
#include "Interface.h"
#include "Packet.h"

#define ARP_REFRESH_RATE 40 // Seconds between re-transmit of spoofed machines

#endif