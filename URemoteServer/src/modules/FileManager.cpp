#include "FileManager.h"

#include "StringUtils.h"
#include <windows.h>
#include <sstream>

#define FindNextFile FindNextFileA
#define FindFirstFile FindFirstFileA
#define WIN32_FIND_DATA WIN32_FIND_DATAA

using namespace std;
using namespace network;

//////////////////////////////////////////////////////////////////////////////
// Fonctions publics
//////////////////////////////////////////////////////////////////////////////

void FileManager::HandleMessage(Response* reply, Request_Code code, string param)
{

	switch (code) {

	case Request_Code_GET_FILE_LIST:
		GetDirectoryContent(reply, param);
		break;

	case Request_Code_OPEN_FILE:
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message(OpenFile(param));
		break;

	default:
		reply->set_returncode(Response_ReturnCode_RC_ERROR);
		reply->set_message("Unknown code received : " + code);
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////
// Fonctions privées
//////////////////////////////////////////////////////////////////////////////

bool FileManager::GetDirectoryContent(Response* reply, string dirPath)
{
	cout << "Target directory is " << dirPath.c_str() << endl;

	
	// Préparation de la chaine pour l'utilisation de la fonction FindFile
	// On ajoute "\\*" à la fin du nom de repertoire.
	string searchPath = dirPath + "\\*";
	string errorMessage;

	// On vérifie que le chemin ne soit pas plus grand que la taille maximum autorisée (MAX_PATH) 
	if (searchPath.length() > MAX_PATH) {
		errorMessage = "Directory path is too long.";
		cerr << errorMessage << endl;
		reply->set_returncode(Response_ReturnCode_RC_ERROR);
		reply->set_message(errorMessage);
		return false;
	}

	// Recherche du premier fichier dans le repertoire.
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	
	hFind = FindFirstFile(searchPath.c_str(), &ffd);
	if (hFind == INVALID_HANDLE_VALUE) {
		errorMessage = "FindFirstFile error : INVALID_HANDLE_VALUE";
		cerr << errorMessage << endl;
		reply->set_returncode(Response_ReturnCode_RC_ERROR);
		reply->set_message(errorMessage);
		return false;
	}

	// Initialisation du vecteur à retourner
	//DirContent* dirContent = new DirContent();
	DirContent *dirContent = reply->mutable_dircontent();
	dirContent->set_path(dirPath);

	// Lister tous les fichiers du repertoire en récupérant quelques infos.
	LARGE_INTEGER filesize;
	ostringstream osFilename;

	do {
		osFilename.str("");
		osFilename << ffd.cFileName;

		filesize.LowPart = ffd.nFileSizeLow;
		filesize.HighPart = ffd.nFileSizeHigh;
		//data << "<" << filesize.QuadPart << " bytes>";
		int fileSize = (int) filesize.QuadPart;

		// Si c'est un répertoire
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			
			// skip le répertoire "." et "$RECYCLE.BIN"
			if (strcmp(ffd.cFileName, ".") == 0 || strcmp(ffd.cFileName, "$RECYCLE.BIN") == 0 ) {
				continue;
			}
			
			AddFile(dirContent, osFilename.str(), DirContent_File_FileType_DIRECTORY, fileSize);
			wcout << "<DIR> " << ffd.cFileName << " " << filesize.QuadPart << " bytes" << endl;

		// Si c'est un fichier
		} else {
			AddFile(dirContent, osFilename.str(), DirContent_File_FileType_FILE, fileSize);
			wcout << "<FILE> " << ffd.cFileName << " " << filesize.QuadPart << " bytes" << endl;

		}
	} while (FindNextFile(hFind, &ffd) != 0);
 
	DWORD dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES) {
		errorMessage = "FindNextFile error : " + dwError;
		cerr << errorMessage << endl;
		reply->set_returncode(Response_ReturnCode_RC_ERROR);
		reply->set_message(errorMessage);
		return false;
	}
	
	FindClose(hFind);
	reply->set_returncode(Response_ReturnCode_RC_SUCCESS);

	return true;
}

string FileManager::OpenFile(string filePath)
{
	bstr_t path(filePath.c_str());
	ShellExecute(nullptr, nullptr, path, nullptr, nullptr, SW_SHOWMAXIMIZED);
	return "Opening file : "  + filePath;
}

bool FileManager::AddFile(DirContent* dirContent, string filename, DirContent_File_FileType type, int size)
{
	DirContent_File *file = dirContent->add_file();
	file->set_name(filename);
	file->set_type(type);
	file->set_size(size);

	return true;
}
