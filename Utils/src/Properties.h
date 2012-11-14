#pragma once

#include <vector>
#include "FileHandler.h"
#include "Property.h"

// Exceptions
class PropertyError { } ;
class LoadPropertyError  : public PropertyError { } ;
class SavePropertyError  : public PropertyError { } ;
class ReadPropertyError : public PropertyError { } ;

using namespace std;

class Properties
{
public:
	Properties();
	~Properties();
	
	bool LoadProperties(const string& path);
	bool SaveProperties();

	string Get(const string _key);
	vector<Property> GetAll();

private:

	vector<Property> m_Properties;
};