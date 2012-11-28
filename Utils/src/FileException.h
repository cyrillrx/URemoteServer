#pragma once

#include <string>

/**
 * Mother class for file exceptions.
 */
class FileException
{
public:
	FileException(std::string message);
protected:
	std::string m_Message;
};

/*
 * Derived classes of FileException
 */
class OpenFileException		: public FileException { public: OpenFileException();	};
class CloseFileException	: public FileException { public: CloseFileException();	};
class ReadFileException		: public FileException { public: ReadFileException();	};
class WriteFileException	: public FileException { public: WriteFileException();	};