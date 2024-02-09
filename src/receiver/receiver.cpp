
#include "receiver.hpp"
#include <asm-generic/socket.h>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>

int Receiver::create_socket() { return socket(AF_INET, SOCK_STREAM, 0); }

sockaddr_in Receiver::create_address(in_port_t port) {
    sockaddr_in result;

    result.sin_family = AF_INET;
    result.sin_port = port;
    result.sin_addr.s_addr = INADDR_ANY;

    return result;
}

bool Receiver::analyze(const std::string& data) {
    size_t data_len = data.length();
    return (data_len > 2 && data_len % 32 == 0);
}

void Receiver::accept_connection() {
    socklen_t addr_size = sizeof(this->address);

    this->sender_fd =
        accept(this->receiver_fd, (struct sockaddr*)&this->address, &addr_size);

    if (this->sender_fd < 0) {
        perror("accept");
    } else {
        std::cout << "Connected to Sender." << std::endl;
        this->connected = true;
    }
}

void Receiver::receive_data() {
    char buf[Receiver::buffer_size] = {0};
    ssize_t count = recv(this->sender_fd, &buf, sizeof(buf), 0);

    std::cout << "Got message of length: " << count << std::endl;

    switch (count) {
        case -1: {
            perror("recv");
            break;
        }
        case 0: {
            std::cout << "Sender disconnected." << std::endl;
            this->connected = false;
            break;
        }
        default: {
            std::cout << "Message contents: " << (std::string)buf << std::endl;

            if (analyze((std::string)buf)) {
                std::cout << "Message analysis successful!" << std::endl;
            } else {
                std::cout << "Message analysis failed." << std::endl;
            }

            if (send(sender_fd, &count, sizeof(count), 0) < 0) {
                perror("send");
            }

            break;
        }
    }
}

void Receiver::await_data() {
    while (true) {
        if (!this->connected) {
            accept_connection();
        } else {
            receive_data();
        }
    }
}

void Receiver::start() {
    if (this->receiver_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(this->receiver_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    if (bind(this->receiver_fd, (struct sockaddr*)&this->address,
             sizeof(this->address)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(this->receiver_fd, 2) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    std::thread connector(&Receiver::await_data, this);
    connector.join();
}
