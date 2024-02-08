// Sender
// Takes user input data, processes it and sends it to the Receiver.

#include "sender.hpp"

int main() {
    printf("main() called.\n");

    Sender s("KB");
    s.start();

    printf("main() exited.\n");

    return 0;
}
