#pragma once

#include <windows.h>
#include <string>
#include <memory>

using namespace std;

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

class App
{
private:
	static unique_ptr<App> s_GomPlayer;
	string mLabel;
	string mExePath;
	string mClassName;

	App(string _label, string _className, string _exePath);

	bool SetOnTop();
	bool Launch();

public:
	static unique_ptr<App> GetGomPlayer();
	~App(void);
	static void FreeGomPlayer();
	
	string Show();
	string Close();
	string Strech();
};