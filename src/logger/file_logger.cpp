//
// Created by Cyril Leroux on 16/05/2015.
//

#include "file_logger.h"

#include <iostream>
#include "fs/file_handler.h"


// TODO: Move to file_handler or FileWriter (create the class)
/**
 * Appends a line in a file.
 * @param logFilename the logfile to log in.
 * @param message The message to write in the file.
 */
void AppendInFile(const std::string& logFilename, const std::string& logMessage)
{
    file_handler fwriter(logFilename, fh_open_mode::open_mode::append);
    fwriter.get_file() << logMessage << std::endl;
}

void FileLogger::DoWrite(const std::string& message) const
{
    AppendInFile(log_file_, message);
}