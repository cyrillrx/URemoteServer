#include "FileManager.h"
#include <windows.h>
#include <sstream>
#include <iostream>

vector<string> FileManager::ListFiles(string path)
{
	cout << "Target directory is " << path.c_str() << endl;

	// Initialisation du vecteur à retourner
	vector<string> fileList;
	
	// Préparation de la chaine pour l'utilisation de la fonction FindFile
	// On ajoute "\\*" à la fin du nom de repertoire.
	path += "\\*";
	
	// On vérifie que le chemin ne soit pas plus grand que la taille maximum autorisée (MAX_PATH) 
	if (path.length() > MAX_PATH) {
		cout << "Directory path is too long." << endl;
		return fileList;
	}

	// Recherche du premier fichier dans le repertoire.
	WIN32_FIND_DATAA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;
	hFind = FindFirstFileA(path.c_str(), &ffd);
	if (hFind == INVALID_HANDLE_VALUE) {
		cout << "FindFirstFile error : INVALID_HANDLE_VALUE" << endl;
		return fileList;
	} 
   
	// Lister tous les fichiers du repertoire en récupérant quelques infos.
	LARGE_INTEGER filesize;
	//string data;
	ostringstream data;
	do {
		data.str("");
		data << ffd.cFileName;
		// Si c'est un répertoire
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if (ffd.cFileName != ".") { // skip le répertoire "."
				data << "<DIR>"; 
				fileList.push_back(data.str());
				cout << ffd.cFileName << " <DIR>" << endl;
			}

		// Si c'est un fichier
		} else {
			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;
			cout << ffd.cFileName << " " << filesize.QuadPart << " bytes" << endl;
			data << "<" << filesize.QuadPart << " bytes>";
			fileList.push_back(data.str());

		}
	} while (FindNextFileA(hFind, &ffd) != 0);
 
	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES) {
		cout << "FindFirstFile error : " << dwError << endl;
		return fileList;
	}

	FindClose(hFind);

	return fileList;
}

string FileManager::ListFilesStr(string path)
{
	vector<string> fileList = ListFiles(path);
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
