#pragma once

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>

#include <parser.hpp>
#include <PLReceiverReceive.hpp>


class FLReceiverReceive(){

public:
	FLReceiverReceive(int sock_){
		sock = sock_;

		PLReceiverReceive::PLReceiverReceive();
		fp2pReceive();
	}

	void stopAll(){
		PLReceiverReceive::stopAll();
		listen = false;
	}

private:
	bool listen = true;
	int sock;

	void fp2pReceive(){
		char buffer[1024];
		sockaddr_in clientAddress;
		socklen_t cAddrLen = sizeof(clientAddress);
		while(listen){
			ssize_t readLen = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddress, &cAddrLen);
			if(readLen == -1){
				perror("Could not read the contents of the datagram(ACK) sent by the receiver.\n");
				return;
			}
			assert(readLen < 1024);
			buffer[readLen] = '\0';

			std::string recvMsg(buffer);
			PLReceiverReceive::pp2pReceive(recvMsg, clientAddress);
		}
		if (close(sock) == 0) {
				std::cout << "Socket closed successfully." << std::endl;
		}
		else {
				std::cerr << "Failed to close the socket." << std::endl;
		}
	}

};
