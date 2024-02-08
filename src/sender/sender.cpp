
#include "sender.hpp"

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

bool is_even(const char c) {
    int i = c - '0'; // Convert from ASCII value
    return (i % 2 == 0);
};

bool Sender::validate(const std::string& input) {
    printf("\t\t\tReader::validate() called.\n");
    return std::all_of(input.begin(), input.end(), ::isdigit) &&
           input.size() <= 64;
}

void Sender::format(std::string& input, const std::string& rep) {
    printf("\t\t\tReader::format() called.\n");
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
    printf("\t\tReader::read() called.\n");
    std::string input;
    std::cout << "Input up to 64 digits:" << std::endl;

    while (true) {
        std::cin >> input;

        if (validate(input)) {
            format(input, this->replacement);

            // Push data to processing thread
            std::lock_guard<std::mutex> lock(this->buf_lock);
            this->buffer.push(
                std::unique_ptr<std::string>(new std::string(input)));
            this->buf_cv.notify_one();

            printf("\t\tReader::read() sent data.\n");
        } else {
            std::cout << "Incorrect input!" << std::endl;
        }
    }
    printf("\t\tReader::read() exited.\n");
}

void Sender::process() {
    printf("\t\tReader::process() called.\n");
    while (true) {
        std::unique_ptr<std::string> data;

        // Wait for data to arrive
        std::unique_lock<std::mutex> lock(this->buf_lock);
        this->buf_cv.wait(lock, [this] { return !buffer.empty(); });

        printf("\t\tReader::process() received data.\n");

        data = std::move(this->buffer.front());
        this->buffer.pop();

        std::cout << *data.get() << std::endl;

        lock.unlock();

        printf("\t\tReader::process() released lock.\n");

        // Send data to Receiver
        // TODO: Send data
    }
    printf("\t\tReader::process() exited.\n");
}

void Sender::start() {
    printf("\tReader::start() called.\n");

    std::thread reader(&Sender::read, this);
    std::thread processor(&Sender::process, this);

    reader.join();
    processor.join();
}

void Sender::start_detached() {
    printf("\tReader::start_detached() called.\n");

    std::thread reader(&Sender::read, this);
    std::thread processor(&Sender::process, this);

    reader.detach();
    processor.detach();
}
