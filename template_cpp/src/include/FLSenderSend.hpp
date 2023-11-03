#pragma once

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cassert>

#include "parser.hpp"


class FLSenderSend{

public:
	FLSenderSend(Parser::Host Receiver){
		sock = socket(AF_INET, SOCK_DGRAM, 0);
		if(sock == -1){
        perror("Failed to create socket");
    }

		serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(Receiver.portReadable());
    serverAddress.sin_addr.s_addr = inet_addr(Receiver.ipReadable().c_str());

	}

	int getSocket(){
		return sock;
	}

	void fp2pSend(std::string msg){
		if(sendto(sock, msg.c_str(), msg.length(), 0, reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress)) == -1)
			perror("Error while sending the message.\n");
	}

	void stopAll(){
		if (close(sock) == 0) {
        std::cout << "Socket closed successfully." << std::endl;
    }
	 	else {
        std::cerr << "Failed to close the socket." << std::endl;
    }
	}

private:
	int sock;
	sockaddr_in serverAddress;

};
