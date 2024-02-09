
#ifndef SENDER_HPP
#define SENDER_HPP

#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <netinet/in.h>
#include <queue>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

/**
 * Validates user input data, formats it and sends it over a socket.
 */
class Sender {
  public:
    Sender(const std::string rep, uint16_t port)
        : replacement(rep), connected(false), receiver_fd(create_socket()),
          address(create_address(htons(port))) {}
    ~Sender() {
        close(receiver_fd);
    }

    /**
     * Begin Sender operations. This is a blocking function.
     */
    void start();

    /**
     * Validate that a string consists only of digits and is under 65
     * characters.
     *
     * @param input String to be validated.
     */
    static bool validate(const std::string& input);

    /**
     * Sort user input and replace all even digits with some string.
     *
     * @param input User input string.
     * @param rep String that will replace all even digits.
     */
    static void format(std::string& input, const std::string& rep);

  private:
    /**
     * Initializes a socket over IPv4 Internet protocol.
     */
    int create_socket();

    /**
     * Initializes receiver address with a specified port.
     *
     * @param port Port for the address to use.
     */
    sockaddr_in create_address(in_port_t port);

    /**
     * Sends a connection request to the receiver.
     */
    void request_connection();

    /**
     * Reading thread is responsible for accepting user inputs
     * and validating them before storing them in the buffer.
     */
    void read();

    /**
     * Processing thread is responsible for formatting the data
     * entered by the user and sending it to the Receiver.
     */
    void process();

    /// Protects the buffer from desync threads.
    std::mutex buf_lock;
    /// Notifies processing thread of updates in the buffer.
    std::condition_variable buf_cv;
    /// Stores connection state.
    bool connected;
    /// Buffer shared between reading and processing threads.
    std::queue<std::string> buffer;
    /// String that will replace even digits.
    std::string replacement;
    /// File descriptor of the socket.
    int receiver_fd;
    /// Address of the receiver.
    sockaddr_in address;
};

#endif
