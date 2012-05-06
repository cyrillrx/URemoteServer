#pragma once

#include <windows.h>
#include <string> 

using namespace std;

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

class App
{
private:
	static App* s_GomPlayer;
	string mLabel;
	string mExePath;
	string mClassName;

	App(string _label, string _className, string _exePath);
	~App(void);

	bool SetOnTop();
	bool Launch();

public:
	static App* GetGomPlayer();
	static void FreeGomPlayer();
	
	string Show();
	string Close();
	string Strech();
};

