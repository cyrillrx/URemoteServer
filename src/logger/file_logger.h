//
// Created by Cyril Leroux on 16/05/2015.
//

#pragma once

#include <string>
#include "logger/logger.h"

class FileLogger : public Logger
{
private:
    virtual void DoWrite(const std::string& message) const override;

    std::string log_file_;

public:
    FileLogger(const std::string& logFile, const LogSeverity& severity_filter) : Logger(severity_filter), log_file_(logFile) { }

    FileLogger(const std::string& logFile) : Logger(), log_file_(logFile) { }
};
