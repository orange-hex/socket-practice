
#ifndef SENDER_HPP
#define SENDER_HPP

#include <thread>
#include <mutex>
#include <string>

/// Processes data and sends it over a socket.
class Sender
{
public:
    Sender() {}
    ~Sender() {}

    void startReading();
    void startProcessing();
    void stopReading();
    void stopProcessing();
    // TODO: Add socket stuffs~

private:
    /// Validate user input: string only has digits and is under 65 characters
    bool validate(const std::string& input);

    /// Reading and processing threads
    std::thread read, process;
    /// Protects buffer
    std::mutex buffer_mutex;
    /// Buffer shared between reading and processing threads
    std::string buffer;
};

#endif
