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
	std::string Stretch();
	
private:
	static std::unique_ptr<App> s_GomPlayer;
	std::string m_label;
	std::string m_exe_path;
	std::string m_class_name;

	App(const std::string& label, const std::string& className, const std::string& exePath);

	bool SetOnTop();
	bool Launch();
};