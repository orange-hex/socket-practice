
#ifndef SENDER_HPP
#define SENDER_HPP

#include <mutex>
#include <string>
#include <thread>

/// Processes data and sends it over a socket.
class Sender {
  public:
    Sender() {}
    ~Sender() {}

    void start();
    void stop();

    // TODO: Add socket stuffs~

  private:
    /// Validate that a string consists only of digits and is under 65
    /// characters.
    bool validate(const std::string&);

    /**
     * Sort user input and replace all even digits with some string.
     *
     * @param input User input string.
     * @param rep String that will replace all even digits.
     */
    void format(std::string& input, const std::string& rep);

    /**
     * Reading thread
     */
    void read();

    /**
     * Processing thread
     */
    void process();

    /// Reading and processing threads.
    std::thread reading, processing;
    /// Protects buffer.
    std::mutex buffer_mutex;
    /// Buffer shared between reading and processing threads.
    std::string buffer;
};

#endif
