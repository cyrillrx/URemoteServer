#pragma once

#include <string>
#include <memory>

#include "platform_config.h"
# if defined(WINDOWS_PLATFORM)
#	include <windows.h>
# endif

//BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

class App
{
public:
	static std::unique_ptr<App> GetGomPlayer();
	~App(void);
	static void FreeGomPlayer();
	
	std::string Show();
	std::string Close();
	std::string Strech();
	
private:
	static std::unique_ptr<App> s_GomPlayer;
	std::string mLabel;
	std::string mExePath;
	std::string mClassName;

	App(std::string label, std::string className, std::string exePath);

	bool SetOnTop();
	bool Launch();
};