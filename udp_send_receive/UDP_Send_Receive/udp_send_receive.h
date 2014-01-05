/*
* udp_send_receive.h
* 12/31/13
* Travis Carney
*
* This header contains forward declarations for functions used in udp_send_receive.cpp.
*/

#ifndef UPD_SEND_RECEIVE_H
#define UDP_SEND_RECEIVE_H

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

void getIP(std::string &);				// Prompts the user to enter an IP
bool validateIP(const std::string &);	// Validates an IP address

#endif /* UDP_SEND_RECEIVE_H */