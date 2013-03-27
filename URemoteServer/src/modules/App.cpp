#include "App.h"

#include <iostream>

#include "Keyboard.h"
#include "exception/config_exception.h"

# if defined(WINDOWS_PLATFORM)
#	include <comdef.h>
# endif

//////////////////////////////////////////////////////////////////////////////
// Fonctions privées
//////////////////////////////////////////////////////////////////////////////

std::unique_ptr<App> App::s_GomPlayer = nullptr;

App::App(const std::string& label, const std::string& className, const std::string& exePath)
{
	m_label			= label;
	m_class_name	= className;
	m_exe_path		= exePath;
}

App::~App(void)
{
}

bool App::SetOnTop()
{
# if defined(WINDOWS_PLATFORM)
	bstr_t className(m_class_name.c_str());
	auto window = FindWindow(className, nullptr);
	if (!window) {
		std::cout << "Error while searching process." << std::endl;
		return false;
	}
	return SetForegroundWindow(window) == TRUE;
# else
	// TODO: Code Linux function for App::SetOnTop()
# endif
	throw platform_exception("Timer::frequency()");
}

bool App::Launch()
{
# if defined(WINDOWS_PLATFORM)
	std::cout << "Launching " << m_label << std::endl;
	bstr_t path(m_exe_path.c_str());
	auto returnCode = ShellExecute(nullptr, L"open", path, L"", nullptr, SW_SHOWMAXIMIZED);
	return ((int)returnCode > 32);
# else
	// TODO: Code Linux function for App::Launch()
# endif
	throw platform_exception("Timer::frequency()");
}

//////////////////////////////////////////////////////////////////////////////
// Fonctions publics
//////////////////////////////////////////////////////////////////////////////

std::unique_ptr<App> App::GetGomPlayer() {
	if (s_GomPlayer)
		return move(s_GomPlayer);

	s_GomPlayer = std::unique_ptr<App>(new App("Gom Player", "GomPlayer1.x", "D:\\Programs\\GomPlayer\\GOM.exe"));
	return move(s_GomPlayer);
}

void App::FreeGomPlayer() {
	s_GomPlayer = nullptr;
}

/** 
 * Met l'application au premier plan
 * Lance l'application si elle n'est pas lancée
 */
std::string App::Show()
{
	std::string resultMessage;

	if (!SetOnTop()) {
		std::cout << "Could not bring " << m_label << " to the front." << std::endl;
		std::cout << "Launching " << m_label << std::endl;
		if (Launch()) {
			resultMessage = "Launching " + m_label;
		} else {
			resultMessage = "Couldn't launch " + m_label;
		}
	} else {
		resultMessage = m_label + " is now on top.";
	}

	std::cout << resultMessage << std::endl; 
	return resultMessage;
}

/** 
 * Ferme l'application si elle est ouverte
 */
std::string App::Close()
{
# if defined(WINDOWS_PLATFORM)
	std::string resultMessage;
	
	bstr_t className(m_class_name.c_str());
	auto window = FindWindow(className, nullptr);
	if (!window) {
		resultMessage = m_label + " is not open.";
	} else {
		PostMessage(window, WM_CLOSE, 0, 0);
		resultMessage = m_label + " has been closed.";
	}

	std::cout << resultMessage << std::endl; 
	return resultMessage;
# else
	// TODO: Code Linux function for App::Close()
# endif
	throw platform_exception("App::Close()");
}

/** 
 * Stretch l'application si elle est ouverte
 */
std::string App::Strech()
{
# if defined(WINDOWS_PLATFORM)
	std::string resultMessage;
	
	bstr_t className(m_class_name.c_str());
	auto window = FindWindow(className, nullptr);
	if (!window) {
		resultMessage = m_label + " is not open.";
	} else {
		Keyboard::CtrlEnter();
		resultMessage = m_label + " has been streched.";
	}

	std::cout << resultMessage << std::endl; 
	return resultMessage;
# else
	// TODO: Code Linux function for App::Strech()
# endif
	throw platform_exception("App::Strech()");
}



