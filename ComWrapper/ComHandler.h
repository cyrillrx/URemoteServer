#pragma once

class ComHandler
{
public:
	ComHandler();
	~ComHandler();
	
	void ckeckResult(HRESULT result);

private :
	std::string getResultMessage(HRESULT result);
};


