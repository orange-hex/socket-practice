// Receiver
// Receives data from Sender, analyzes it and displays results.

#include "receiver.hpp"

int main() {
    Receiver r(8080);
    r.start();

    return 0;
}
