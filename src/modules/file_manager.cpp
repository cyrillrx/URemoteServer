#include "file_manager.h"

#include <sstream>

#include "platform_config.h"
#include "exception/file_exception.h"
#include "string_utils.h"
#include "logger_manager.h"
#include "console_logger.h"

#if defined(WINDOWS_PLATFORM)
#include <comutil.h>
#endif

using namespace network_io;

LoggerManager loggerManager;
std::unique_ptr<Logger> consoleLogger(new ConsoleLogger(DEBUG));
loggerManager.AddLogger(consoleLogger);

//////////////////////////////////////////////////////////////////////////////
// Public functions
//////////////////////////////////////////////////////////////////////////////

void file_manager::handle_message(Response* reply, const Request_Code& code, const std::string& strExtra)
{
	switch (code) {

	case Request::QUERY_ROOTS:
		// TODO : return strings containing paths
		reply->set_returncode(Response_ReturnCode_RC_ERROR);
		reply->set_message("QUERY_ROOTS not yet implemented : " + code);
		break;

	case Request::QUERY_CHILDREN:
		query_children(reply, strExtra);
		break;

	case Request::OPEN_SERVER_SIDE:
		open_file(strExtra);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("Opening file : "  + strExtra);
		break;

	case Request::OPEN_CLIENT_SIDE:
		// TODO : Stream client side
		reply->set_returncode(Response_ReturnCode_RC_ERROR);
		reply->set_message("OPEN_CLIENT_SIDE not yet implemented : " + code);
		break;

	default:
		reply->set_returncode(Response_ReturnCode_RC_ERROR);
		reply->set_message("Unknown code received : " + code);
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////
// Private functions
//////////////////////////////////////////////////////////////////////////////

bool file_manager::query_children(Response* reply, std::string dirPath)
{
	// Default root
	if (dirPath == "default_path") {
		dirPath = "L:";
	}

	try {
		auto fileList = fs_utils::list_files(dirPath);

		// Initiate the vector to return
		auto* directory = reply->mutable_file();
		directory->set_absolutefilepath(dirPath);
		directory->set_filename(dirPath);
		directory->set_isdirectory(true);
		directory->set_size(0);

		for (auto& file : fileList) {
			add_file(directory, file);
		}

		reply->set_message("\"" + dirPath + "\"" + " successfully loaded.");
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		return true;

	} catch (const file_exception& e) {
		// TODO: Catch file_exception
		loggerManager.Error(e.whatAsString());
		reply->set_returncode(Response_ReturnCode_RC_ERROR);
		reply->set_message(e.what());
		return false;
	}
}

void file_manager::open_file(const std::string& filePath)
{
#if defined(WINDOWS_PLATFORM)
	bstr_t path(filePath.c_str());
	ShellExecute(nullptr, nullptr, path, nullptr, nullptr, SW_SHOWMAXIMIZED);
#else
    system(filePath.c_str());
#endif
}

// Adds a file to the 
bool file_manager::add_file(FileInfo* directoryInfo, fs_utils::file& file)
{
	auto* exchangeFile = directoryInfo->add_child();

	exchangeFile->set_absolutefilepath(file.path());
	exchangeFile->set_filename(file.filename());
	exchangeFile->set_isdirectory(file.type == fs_utils::file_type::directory_file);
	exchangeFile->set_size(file.size());

	return true;
}