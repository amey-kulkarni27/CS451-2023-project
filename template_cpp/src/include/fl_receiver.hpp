#pragma once

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>

#include <parser.hpp>
#include <stubborn_receiver.hpp>


class FLReceiver(){

public:
	FLReceiver(Parser::Host Self){
		sock = socket(AF_INET, SOCK_DGRAM, 0);
		if(sock == -1){
        perror("Failed to create socket");
        return 1;
    }

		fp2pReceive();
	}

	void fp2pSend(std::string msg, sockaddr_in clientAddress){
		// Send the ACK to the client
		if(sendto(sock, msg.c_str(), msg.length(), 0, (struct sockaddr*)&clientAddress, sizeof(clientAddress)) == -1)
			perror("Error while sending the message.\n");
	}

	void stopAll(){
		listen = false;
		if (close(sock) == 0) {
        std::cout << "Socket closed successfully." << std::endl;
    }
		else {
        std::cerr << "Failed to close the socket." << std::endl;
    }
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
			StubbornReceiver::sp2pReceive(recvMsg, clientAddress);
		}
	}

}
