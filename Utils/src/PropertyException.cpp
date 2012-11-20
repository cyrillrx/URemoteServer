#include "stdafx.h"
#include "PropertyException.h"

PropertyException::PropertyException() { }

LoadPropertyException::LoadPropertyException()
{
	m_Message = "LoadPropertyException : Error while loading property."; 
}

SavePropertyException::SavePropertyException()
{
	m_Message = "SavePropertyException : Error while saving property."; 
}

ReadPropertyException::ReadPropertyException()
{
	m_Message = "ReadPropertyException : Error while reading property."; 
}

WritePropertyException::WritePropertyException()
{
	m_Message = "ReadPropertyException : Error while writing property."; 
}