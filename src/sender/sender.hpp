
#ifndef SENDER_HPP
#define SENDER_HPP

#include <condition_variable>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <string>

/// Processes data and sends it over a socket.
class Sender {
  public:
    Sender(const std::string rep)
        : replacement(rep) {}
    ~Sender() {}

    void start();
    void start_detached();
    // void move_execution();

    // TODO: Add socket stuffs~

  private:
    /**
     * Validate that a string consists only of digits and is under 65
     * characters.
     */
    static bool validate(const std::string&);

    /**
     * Sort user input and replace all even digits with some string.
     *
     * @param input User input string.
     * @param rep String that will replace all even digits.
     */
    static void format(std::string& input, const std::string& rep);

    /**
     * Reading thread
     */
    void read();

    /**
     * Processing thread
     */
    void process();

    /// Protects the buffer from desync threads.
    std::mutex buf_lock;
    /// Notifies processing thread of updates in the buffer.
    std::condition_variable buf_cv;
    /// Buffer shared between reading and processing threads.
    std::queue<std::unique_ptr<std::string>> buffer;
    /// String that will replace even digits.
    std::string replacement;
};

#endif
