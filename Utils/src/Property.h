#pragma once

#include <string>

class Property
{
public:
	std::string Key;
	std::string Value;
	int Type;

	Property(void);
	~Property(void);
};