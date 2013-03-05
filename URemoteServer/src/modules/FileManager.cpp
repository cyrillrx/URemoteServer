#include "FileManager.h"

#include <windows.h>
#include <sstream>

#include "StringUtils.h"
#include "Utils.h"

#define FindNextFile FindNextFileA
#define FindFirstFile FindFirstFileA
#define WIN32_FIND_DATA WIN32_FIND_DATAA

using namespace network;

//////////////////////////////////////////////////////////////////////////////
// Public functions
//////////////////////////////////////////////////////////////////////////////

void FileManager::HandleMessage(Response* reply, Request_Code code, std::string param)
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
		auto fileList = FileUtils::list_files(dirPath);

		// Initiate the vector to return
		DirContent *dirContent = reply->mutable_dircontent();
		dirContent->set_path(dirPath);

		for (FileUtils::File& file : fileList) {
			AddFile(dirContent, file);
		}

		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		return true;

	} catch (const std::exception& e) {
		// TODO: Catch FileException
		Utils::getLogger()->error(e.what());
		reply->set_returncode(Response_ReturnCode_RC_ERROR);
		reply->set_message(e.what());
		return false;
	}
}

void FileManager::OpenFile(std::string filePath)
{
	bstr_t path(filePath.c_str());
	ShellExecute(nullptr, nullptr, path, nullptr, nullptr, SW_SHOWMAXIMIZED);
}

bool FileManager::AddFile(DirContent* dirContent, FileUtils::File& file)
{
	DirContent_File *exchangefile = dirContent->add_file();
	exchangefile->set_name(file.getFilename());

	// TODO: Use a function to translate FileUtils::File::TYPE to DirContent_File_FileType
	if (file.type == FileUtils::file_type::directory_file) {
		exchangefile->set_type(DirContent_File_FileType_DIRECTORY);

	} else if (file.type == FileUtils::file_type::regular_file) {
		exchangefile->set_type(DirContent_File_FileType_FILE);
	} else {
		// TODO: Throw an exception
	}
	exchangefile->set_size(file.size);

	return true;
}
