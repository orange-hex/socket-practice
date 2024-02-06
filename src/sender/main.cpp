// Sender
// Takes user input data, processes it and sends it to the Receiver.

#include <string>
#include <iostream>
#include <algorithm>

#include "sender.hpp"

// Process user input
void process(std::string& input) {
    
}

// Validate user input - string only has digits and is under 65 characters
bool validate(const std::string& input) {
    return std::all_of(input.begin(), input.end(), ::isdigit) &&
        input.size() <= 64;
}

// Continuously read user input
void read() {
    std::string input = "";

    std::cout << "Input up to 64 digits: ";

    while (true) {
        std::cin >> input;
        
        if (validate(input)) {
            process(input);
            // Push to buffer
        } else {
            std::cout << "Incorrect input!" << std::endl;
        }
    }
}

int main() {
    return 0;
}
