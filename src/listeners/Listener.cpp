#include "Listener.h"

#include "logger/console_logger.h"
#include "../logger/file_logger.h"

static const std::string LOG_DIR = "logs/";

Listener::Listener(const std::string& log_path)
{
    log_ = LoggerManager();
    std::unique_ptr<Logger> consoleLogger(new ConsoleLogger());
    log_.AddLogger(consoleLogger);

    std::unique_ptr<Logger> fileLogger(new FileLogger(LOG_DIR + log_path, DEBUG));
    log_.AddLogger(fileLogger);
}

Listener::~Listener() { }

/** Start the listening thread */
std::thread Listener::start()
{
    return std::thread(&Listener::doStart, this);
}

/** Stop the listener loop. */
void Listener::stop()
{
    continueToListen_ = false;
}

// TODO: Create a callback to AI via open port.
