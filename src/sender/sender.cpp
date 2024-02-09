
#include "sender.hpp"

#include <algorithm>
#include <arpa/inet.h>
#include <asm-generic/errno.h>
#include <condition_variable>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <mutex>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>

bool is_even(const char c) {
    int i = c - '0'; // Convert from ASCII value
    return (i % 2 == 0);
};

int Sender::create_socket() { return socket(AF_INET, SOCK_STREAM, 0); }

sockaddr_in Sender::create_address(in_port_t port) {
    sockaddr_in result;

    result.sin_family = AF_INET;
    result.sin_port = port;
    result.sin_addr.s_addr = INADDR_ANY;

    return result;
}

bool Sender::validate(const std::string& input) {
    return std::all_of(input.begin(), input.end(), ::isdigit) &&
           input.size() <= 64;
}

void Sender::format(std::string& input, const std::string& rep) {
    std::sort(input.rbegin(), input.rend());

    for (size_t pos = 0; pos < input.length(); pos++) {
        if (is_even(input[pos])) {
            input.erase(pos, 1);
            input.insert(pos, rep);
            pos += rep.length() - 1; // Correct for new length
        }
    }
}

void Sender::read() {
    std::string input;
    std::cout << "Input up to 64 digits:" << std::endl;

    while (true) {
        std::cin >> input;

        if (validate(input)) {
            format(input, this->replacement);

            // Push data to processing thread
            std::lock_guard<std::mutex> lock(this->buf_lock);
            this->buffer.push(input);
            this->buf_cv.notify_one();

        } else {
            std::cout << "Incorrect input!" << std::endl;
        }
    }
}

void Sender::process() {
    while (true) {
        std::string data;

        // Wait for data to arrive
        std::unique_lock<std::mutex> lock(this->buf_lock);
        this->buf_cv.wait(lock, [this] { return !buffer.empty(); });

        data = this->buffer.front();
        this->buffer.pop();

        lock.unlock();

        while (!this->connected) {
            request_connection();
        }

        if (send(receiver_fd, data.c_str(), strlen(data.c_str()), 0) < 0) {
            perror("send");
        } else {
            std::cout << "Sending " << data << std::endl;
        }

        ssize_t res;
        if (recv(receiver_fd, &res, sizeof(res), 0) == 0) {
            std::cout << "Disconnected from receiver." << std::endl;
            this->connected = false;
            close(receiver_fd);
            this->receiver_fd = create_socket();
        }
    }
}

void Sender::request_connection() {
    if (connect(receiver_fd, (struct sockaddr*)&this->address,
                sizeof(this->address)) < 0) {
        // perror("connect");
    } else {
        std::cout << "Connected to receiver." << std::endl;
        this->connected = true;
    }
}

void Sender::start() {
    if (this->receiver_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    std::thread reader(&Sender::read, this);
    std::thread processor(&Sender::process, this);

    reader.join();
    processor.join();
}
