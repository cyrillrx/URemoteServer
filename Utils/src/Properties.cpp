#include "StdAfx.h"
#include "Properties.h"

Properties::Properties() {}

Properties::~Properties() {}

string Properties::Get(const string _key)
{
	for(vector<Property>::iterator prop = m_Properties.begin(); prop != m_Properties.end(); prop++) {
		if (prop->Key == _key) {
			return prop->Value;
		}
	}
	throw ReadPropertyError();
	cerr << "Error while reading property " << _key << endl;
	return "";
}

vector<Property> Properties::GetAll()
{
	return m_Properties;
}

bool Properties::LoadProperties(const string& path)
{
	FileHandler fh(path, FileHandler::OPEN_TYPE_READ);

	try {

		if (!fh.GetFile()) {
			cerr << "Impossible d'ouvrir le fichier !" << endl;
			throw LoadPropertyError();
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
	return true;
}

bool Properties::SaveProperties()
{
	return false;
}