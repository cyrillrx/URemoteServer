#include "FileManager.h"
#include "StringUtils.h"

#include <windows.h>
#include <sstream>

#define FindNextFile FindNextFileA
#define FindFirstFile FindFirstFileA
#define WIN32_FIND_DATA WIN32_FIND_DATAA

//////////////////////////////////////////////////////////////////////////////
// Fonctions publics
//////////////////////////////////////////////////////////////////////////////

void FileManager::HandleMessage(Response* _reply, Request_Code _code, string _param)
{

	switch (_code) {

	case Request_Code_GET_FILE_LIST:
		DirContent *dirContent = _reply->mutable_dircontent();
		if (SetDirectoryContent(dirContent, _param)) {
			_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		} else {
			_reply->set_returncode(Response_ReturnCode_RC_ERROR);
			_reply->set_message("SetDirectoryContent returned false");
		}
		break;

	case Request_Code_OPEN_FILE:
		_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		_reply->set_message(OpenFile(_param));
		break;

	default:
		_reply->set_returncode(Response_ReturnCode_RC_ERROR);
		_reply->set_message("Unknown code received : " + _code);
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////
// Fonctions privées
//////////////////////////////////////////////////////////////////////////////

bool FileManager::SetDirectoryContent(DirContent* _dirContent, string _dirPath)
{
	cout << "Target directory is " << _dirPath.c_str() << endl;

	
	// Préparation de la chaine pour l'utilisation de la fonction FindFile
	// On ajoute "\\*" à la fin du nom de repertoire.
	_dirPath += "\\*";
	
	// On vérifie que le chemin ne soit pas plus grand que la taille maximum autorisée (MAX_PATH) 
	if (_dirPath.length() > MAX_PATH) {
		cerr << "Directory path is too long." << endl;
		return false;
	}

	// Recherche du premier fichier dans le repertoire.
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	
	//LPCWSTR str = StringUtils::StringToBStr(_dirPath);
	hFind = FindFirstFile(_dirPath.c_str(), &ffd);
	if (hFind == INVALID_HANDLE_VALUE) {
		cerr << "FindFirstFile error : INVALID_HANDLE_VALUE" << endl;
		return false;
	}

	// Initialisation du vecteur à retourner
	DirContent* dirContent = new DirContent();
	dirContent->set_path(_dirPath);

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
		cerr << "GetDirectoryContent error : " << dwError << endl;
		return NULL;
	}

	FindClose(hFind);

	return true;
}

string FileManager::OpenFile(string _filePath)
{
	bstr_t filePath(_filePath.c_str());
	ShellExecute(NULL, NULL, filePath, NULL, NULL, SW_SHOWMAXIMIZED);
	return "Ouverture du fichier : "  + _filePath;
}

bool FileManager::AddFile(DirContent* _dirContent, string _filename, DirContent_File_FileType _type, int _size)
{
	DirContent_File *file = _dirContent->add_file();
	file->set_name(_filename);
	file->set_type(_type);
	file->set_size(_size);

	return true;
}
