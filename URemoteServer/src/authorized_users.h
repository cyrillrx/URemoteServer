#pragma once

#include <string>
#include "properties.h"

class authorized_users : public properties
{
public:
	authorized_users(const std::string& path);
	~authorized_users();
	
	bool is_authorized(const std::string& key);
	std::string get_user(const std::string& key);
};

