#pragma once

#include <vector>
#include "FileHandler.h"
#include "Property.h"
#include "PropertyException.h"

using namespace std;

class Properties
{
public:
	Properties(const string& path);
	~Properties();

protected:
	void LoadProperties(const string& path);
	void SaveProperties(const string& path);
	void SaveProperties();
	
	const string GetString(const string& key);
	const string GetString(const string& key, const string& defaultValue);
	void SetString(const string& key, const string& value);

	const int GetInt(const string& key);
	const int GetInt(const string& key, const int& defaultValue);
	void SetInt(const string& key, const int& value);

	const bool GetBool(const string& key);
	const bool GetBool(const string& key, const bool& defaultValue);
	void SetBool(const string& key, const bool& value);

private:
	string m_FilePath;
	vector<Property> m_Properties;
};