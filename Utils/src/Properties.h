#pragma once

#include <vector>
#include "FileHandler.h"
#include "Property.h"
#include "PropertyException.h"

using namespace std;

class Properties
{
public:
	Properties();
	~Properties();
	
	void LoadProperties(const string& path);
	void SaveProperties(const string& path);

	const string GetString(const string key);
	const int GetInt(const string key);
	const bool GetBool(const string key);
	vector<Property> GetAll();

private:

	vector<Property> m_Properties;
};