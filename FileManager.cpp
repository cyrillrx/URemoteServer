#include "FileManager.h"
#include "StringUtils.h"

#include <windows.h>
#include <sstream>

#define FindNextFile FindNextFileA
#define FindFirstFile FindFirstFileA
#define WIN32_FIND_DATA WIN32_FIND_DATAA

//////////////////////////////////////////////////////////////////////////////
// Fonctions privées
//////////////////////////////////////////////////////////////////////////////

vector<string> FileManager::ListFiles(string _dirPath)
{
	cout << "Target directory is " << _dirPath.c_str() << endl;

	// Initialisation du vecteur à retourner
	vector<string> fileList;
	
	// Préparation de la chaine pour l'utilisation de la fonction FindFile
	// On ajoute "\\*" à la fin du nom de repertoire.
	_dirPath += "\\*";
	
	// On vérifie que le chemin ne soit pas plus grand que la taille maximum autorisée (MAX_PATH) 
	if (_dirPath.length() > MAX_PATH) {
		cout << "Directory path is too long." << endl;
		return fileList;
	}

	// Recherche du premier fichier dans le repertoire.
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;
	//LPCWSTR str = StringUtils::StringToBStr(_dirPath);
	hFind = FindFirstFile(_dirPath.c_str(), &ffd);
	if (hFind == INVALID_HANDLE_VALUE) {
		cout << "FindFirstFile error : INVALID_HANDLE_VALUE" << endl;
		return fileList;
	} 
   
	// Lister tous les fichiers du repertoire en récupérant quelques infos.
	LARGE_INTEGER filesize;
	ostringstream data;
	do {
		data.str("");
		data << ffd.cFileName;
		// Si c'est un répertoire
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			
			// skip le répertoire "." et "$RECYCLE.BIN"
			if (strcmp(ffd.cFileName, ".") == 0 || strcmp(ffd.cFileName, "$RECYCLE.BIN") == 0 ) {
				continue;
			}

			data << "<DIR>"; 
			fileList.push_back(data.str());
			wcout << ffd.cFileName << " <DIR>" << endl;

		// Si c'est un fichier
		} else {
			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;
			data << "<" << filesize.QuadPart << " bytes>";
			fileList.push_back(data.str());
			wcout << ffd.cFileName << " " << filesize.QuadPart << " bytes" << endl;

		}
	} while (FindNextFile(hFind, &ffd) != 0);
 
	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES) {
		cout << "FindFirstFile error : " << dwError << endl;
		return fileList;
	}

	FindClose(hFind);

	return fileList;
}

//////////////////////////////////////////////////////////////////////////////
// Fonctions publics
//////////////////////////////////////////////////////////////////////////////

string FileManager::ListFilesStr(string _dirPath)
{
	vector<string> fileList = ListFiles(_dirPath);
	const int listSize = fileList.size();

	string result = "";
	for(int i = 0; i < listSize; i++) {
		result.append(fileList[i]);
		if (i != listSize - 1) {
			result.append("|");
		}
	}
	return result;
}

string FileManager::OpenFile(string _filePath)
{
	bstr_t filePath(_filePath.c_str());
	ShellExecute(NULL, NULL, filePath, NULL, NULL, SW_SHOWMAXIMIZED);
	return "Ouverture du fichier : "  + _filePath;
}