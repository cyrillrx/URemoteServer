#include "StringUtils.h"


bstr_t StringUtils::StringToBStr(string _str)
{
	bstr_t bstr(_str.c_str());
	return bstr;
}
