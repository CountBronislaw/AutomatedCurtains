/*
* udp_send_receive.cpp
* 12/30/13
* Travis Carney
*
* This program is used to send and receive strings via UDP. I used it to test communication with an Arduino using an Ethernet shield.
* It uses the boost asio library to handle UDP communication.
*/

#include "udp_send_receive.h"

using boost::asio::ip::udp;

/*
* Simply requests an IP from the user, and places the value entered into the supplied string reference.
*/
void getIP(std::string &ip) {
	do {
		std::cout << "Please enter the arduino's IP address:" << std::endl;
		std::cin >> ip;
		std::cout << std::endl;
	} while (!validateIP(ip));

}

/*
* Make sure the ip is valid.
* Returns false when invalid, and true when valid.
*/
bool validateIP(const std::string &ip) {
	boost::system::error_code ec;					// Error code
	boost::asio::ip::address::from_string(ip, ec);	// Convert IP from string

	try {
		// Check if invalid
		if (ec) {
			std::cerr << ec.message() << std::endl;
			return false;
		}
	}
	catch (int err) {
		std::cout << "Exception caught" << std::endl;
		std::cout << err;
	}

	return true;
}

int main() {
	boost::shared_ptr<std::string> message(new std::string(""));	// Holds the message to send
	boost::asio::io_service io_service;								// IO service needed for communication
	udp::resolver resolver(io_service);								// Create a resolver
	std::string ipAddress = "";										// Holds the IP address entered by the user

	getIP(ipAddress);

	udp::resolver::query query(udp::v4(), ipAddress, "8888");		// Query the arduino to get the endpoint
	udp::endpoint receiver_endpoint = *resolver.resolve(query);
	udp::socket socket(io_service);
	socket.open(udp::v4());											// Open the socket

	// Prompt the user for messages and send them
	while (true) {
		std::cout << "Please enter a message:" << std::endl;
		std::cin >> *message;
		
		socket.send_to(boost::asio::buffer(*message), receiver_endpoint);

		// Get the response
		boost::array<char, 128> recv_buff;
		udp::endpoint sender_endpoint;
		size_t len = socket.receive_from(
			boost::asio::buffer(recv_buff), sender_endpoint);

		// Print out the response
		std::cout.write(recv_buff.data(), len) << std::endl;
		recv_buff.empty();
	}
	
	// Close the socket and reset the pointer
	socket.close();
	message.reset();

	return 0;
}