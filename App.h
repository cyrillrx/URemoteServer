#pragma once

#include <windows.h>
#include <string> 

using namespace std;

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

class App
{
private:

	string mLabel;
	string mExePath;
	string mClassName;

	bool SetOnTop();
	bool Launch();

public:
	static string sCurrentClassName;
	static App GetGomPlayer();
	App(string _label, string _className, string _exePath);
	~App(void);
	
	string Show();
	string Close();
	string Strech();
};

