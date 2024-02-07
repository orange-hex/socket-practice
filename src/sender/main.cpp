// Sender
// Takes user input data, processes it and sends it to the Receiver.

#include <string>
#include <iostream>
#include <algorithm>

#include "sender.hpp"

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
