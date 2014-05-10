#include "file_manager.h"

#include <sstream>

#include "platform_config.h"
#include "exception/file_exception.h"
#include "string_utils.h"
#include "Utils.h"

using namespace network_io;

//////////////////////////////////////////////////////////////////////////////
// Public functions
//////////////////////////////////////////////////////////////////////////////

void file_manager::handle_message(Response* reply, Request::Code code, const std::string& param)
{
	switch (code) {

	case Request_Code_QUERY_ROOTS:
		// TODO : return strings containing pathes
		reply->set_returncode(Response_ReturnCode_RC_ERROR);
		reply->set_message("QUERY_ROOTS not yet implemented : " + code);
		break;

	case Request_Code_QUERY_CHILDREN:
		query_children(reply, param);
		break;

	case Request_Code_OPEN_SERVER_SIDE:
		open_file(param);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("Opening file : "  + param);
		break;

	case Request_Code_OPEN_CLIENT_SIDE:
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
		dirPath = "L:/";
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

		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		return true;

	} catch (const file_exception& e) {
		// TODO: Catch file_exception
		Utils::get_logger()->error(e.whatAsString());
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
	auto* exchangefile = directoryInfo->add_child();

	exchangefile->set_absolutefilepath(file.path());
	exchangefile->set_filename(file.filename());
	exchangefile->set_isdirectory(file.type == fs_utils::file_type::directory_file);
	exchangefile->set_size(file.size());

	return true;
}
