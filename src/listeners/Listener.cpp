#include "Listener.h"

#include "logger/console_logger.h"
#include "../logger/file_logger.h"
#include "exception/Exception.h"

static const std::string log_dir = "logs/";
static const std::string log_path = log_dir + "uremote.log";

Listener::Listener()
{
    log_ = LoggerManager();
    std::unique_ptr<Logger> consoleLogger(new ConsoleLogger(DEBUG));
    log_.AddLogger(consoleLogger);

    std::unique_ptr<Logger> fileLogger(new FileLogger(log_path, DEBUG));
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
