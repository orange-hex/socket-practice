
#ifndef RECEIVER_HPP
#define RECEIVER_HPP

#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

/**
 * Receives data over a socket and displays analysis results.
 */
class Receiver {
  public:
    static const size_t buffer_size = 1024;

    Receiver(uint16_t port)
        : receiver_fd(create_socket()), connected(false),
          address(create_address(htons(port))) {}
    ~Receiver() {
        if (connected)
            close(sender_fd);
        close(receiver_fd);
    }

    /**
     * Begin Receiver operations. This is a blocking function.
     */
    void start();

    /**
     * Analyze data by checking it's length.
     *
     * @param data Data to be analyzed.
     * @return true if length of data is more than 2 and is a multiple of 32.
     */
    static bool analyze(const std::string& data);

  private:
    /**
     * Initialize a socket over IPv4 Internet protocol.
     */
    int create_socket();

    /**
     * Initialize sender address with a specified port.
     *
     * @param port Port for the address to use.
     */
    sockaddr_in create_address(in_port_t port);

    /**
     * Accepts a connection from sender.
     */
    void accept_connection();

    /**
     * Receives data from sender.
     */
    void receive_data();

    /**
     * Connects to sender, waits for data and analyzes it.
     */
    void await_data();

    /// File descriptor of the socket.
    int receiver_fd, sender_fd;
    /// Syncronizes awaiting connection with accepting requests.
    std::mutex receive_lock;
    /// Notifies threads when client has connected or disconnected.
    std::condition_variable receive_cv;
    /// Stores connection state.
    bool connected;
    /// Address of the sender.
    sockaddr_in address;
};

#endif
