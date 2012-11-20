#pragma once

#include <string>

/**
 * Mother class for property exceptions.
 */
class PropertyException
{
public:
	PropertyException();
protected:
	std::string m_Message;
};

class LoadPropertyException : public PropertyException
{
public:
	LoadPropertyException();
};

class SavePropertyException : public PropertyException
{
public:
	SavePropertyException();
};

class ReadPropertyException : public PropertyException
{
public:
	ReadPropertyException();
};

class WritePropertyException : public PropertyException
{
public:
	WritePropertyException();
};