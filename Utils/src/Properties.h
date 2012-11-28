#pragma once

#include <vector>
#include "FileHandler.h"
#include "Property.h"
#include "PropertyException.h"

class Properties
{
public:
	Properties(const std::string& path);
	~Properties();

protected:
	void LoadProperties(const std::string& path);
	void SaveProperties(const std::string& path);
	void SaveProperties();
	
	const std::string GetString(const std::string& key);
	const std::string GetString(const std::string& key, const std::string& defaultValue);
	void SetString(const std::string& key, const std::string& value);

	const int GetInt(const std::string& key);
	const int GetInt(const std::string& key, const int& defaultValue);
	void SetInt(const std::string& key, const int& value);

	const bool GetBool(const std::string& key);
	const bool GetBool(const std::string& key, const bool& defaultValue);
	void SetBool(const std::string& key, const bool& value);

private:
	std::string m_FilePath;
	std::vector<Property> m_Properties;
};