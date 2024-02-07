
#include "sender.hpp"

#include <algorithm>
#include <mutex>
#include <string>
#include <thread>

bool is_even(const char c) {
    int i = c - '0'; // Convert from ASCII value
    return !(i % 2);
};

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
    //
}

void Sender::process() {
    //
}

void Sender::start() {
    //
}

void Sender::stop() {
    //
}
