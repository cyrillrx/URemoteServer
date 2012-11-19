#include "StdAfx.h"
#include "Properties.h"
#include <sstream>

Properties::Properties() {}

Properties::~Properties() {}

/**
 * @param key The property key;
 * @return The property as a string.
 */
const string Properties::GetString(const string key)
{
	for (auto prop : m_Properties) {
		if (prop.Key == key) {
			return prop.Value;
		}
	}

	throw ReadPropertyException();
}

/**
 * @param key 
 * @return The property as an integer.
 */
const int Properties::GetInt(const string key)
{
	auto stringProperty = GetString(key);

	stringstream convertedStr(stringProperty);
	int result;
	if (!(convertedStr >> result)) {
		throw ReadPropertyException();
	}
	
	return result;
}

/**
 * @param key 
 * @return The property as a boolean.
 */
const bool Properties::GetBool(const string key)
{
	auto stringProperty = GetString(key);if (stringProperty == "true") {
		return true;
	} else if (stringProperty == "false") {
		return false;
	}

	throw LoadPropertyException();
}

/**
 * @return All the properties as a vector.
 */
vector<Property> Properties::GetAll()
{
	return m_Properties;
}

/**
 * Load the properties from a file.
 * @param path : The property file to read.
 */
void Properties::LoadProperties(const string& path)
{
	try {
		FileHandler fh(path, FileHandler::OPEN_TYPE_READ);

		if (!fh.GetFile()) {
			cerr << "Impossible d'ouvrir le fichier !" << endl;
			throw LoadPropertyException();
		}
	
		
		string line;
		while(getline(fh.GetFile(), line)) {

			const int equalPos	= line.find('=');

			if (equalPos != -1) {
				cout << line << endl;  // on affiche la ligne
				Property prop;
				prop.Key	= line.substr(0, equalPos);
				prop.Value	= line.substr(equalPos + 1);
				m_Properties.push_back(prop);
				//cout << "Before trim:=" << value << endl;
				//remove(value.begin(), value.end(), ' ');
				//cout << "After trim:=" << value << endl;

			} else {
				cout << "No '=' sign for line :" << line << endl;  // on affiche la ligne
			}
		}
		cout << endl;
	} catch (...) {
		cout << "Error while loading file " << path << endl;
	}
}

void Properties::SaveProperties(const string& path)
{
	
}