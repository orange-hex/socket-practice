// Sender
// Takes user input data, processes it and sends it to the Receiver.

#include "sender.hpp"

int main() {
    Sender s("KB", 8080);
    s.start();

    return 0;
}
