#include "FileManager.h"

#include <sstream>

#include "exception\file_exception.h"
#include "string_utils.h"
#include "Utils.h"

using namespace network_io;

//////////////////////////////////////////////////////////////////////////////
// Public functions
//////////////////////////////////////////////////////////////////////////////

void FileManager::HandleMessage(Response* reply, Request::Code code, const std::string& param)
{
	switch (code) {

	case Request_Code_GET_FILE_LIST:
		GetDirectoryContent(reply, param);
		break;

	case Request_Code_OPEN_FILE:
		OpenFile(param);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("Opening file : "  + param);
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

bool FileManager::GetDirectoryContent(Response* reply, std::string dirPath)
{
	try {
		auto fileList = fs_utils::list_files(dirPath);

		// Initiate the vector to return
		auto* dirContent = reply->mutable_dircontent();
		dirContent->set_path(dirPath);

		for (auto& file : fileList) {
			AddFile(dirContent, file);
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

void FileManager::OpenFile(const std::string& filePath)
{
	bstr_t path(filePath.c_str());
	ShellExecute(nullptr, nullptr, path, nullptr, nullptr, SW_SHOWMAXIMIZED);
}

bool FileManager::AddFile(DirContent* dirContent, fs_utils::file& file)
{
	auto* exchangefile = dirContent->add_file();
	exchangefile->set_name(file.filename());

	// TODO: Use a function to translate FileUtils::File::TYPE to DirContent_File_FileType
	if (file.type == fs_utils::file_type::directory_file) {
		exchangefile->set_type(DirContent_File_FileType_DIRECTORY);

	} else if (file.type == fs_utils::file_type::regular_file) {
		exchangefile->set_type(DirContent_File_FileType_FILE);
	} else {
		// TODO: Throw an exception
	}
	exchangefile->set_size(file.size());

	return true;
}
