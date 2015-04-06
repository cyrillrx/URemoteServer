#include "App.h"

#include <iostream>

#include "keyboard.h"
#include "exception/config_exception.h"

// TODO: comments FR => EN
# if defined(WINDOWS_PLATFORM)
#	include <comdef.h>
# endif

//////////////////////////////////////////////////////////////////////////////
// Private functions
//////////////////////////////////////////////////////////////////////////////

std::unique_ptr <App> App::s_GomPlayer = nullptr;

App::App(const std::string &label, const std::string &className, const std::string &exePath)
{
    m_label = label;
    m_class_name = className;
    m_exe_path = exePath;
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
	HWND hwnd(nullptr);
	LPCSTR lpOperation("open");
	bstr_t lpFile(m_exe_path.c_str());
	LPCSTR lpParameters("");
	LPCSTR lpDirectory(nullptr);
	auto returnCode = ShellExecute(hwnd, lpOperation, lpFile, lpParameters, lpDirectory, SW_SHOWMAXIMIZED);
	return ((int)returnCode > 32);
# else
    // TODO: Code Linux function for App::Launch()
	throw platform_exception("Timer::frequency()");
# endif
}

//////////////////////////////////////////////////////////////////////////////
// Public functions
//////////////////////////////////////////////////////////////////////////////

std::unique_ptr <App> App::GetGomPlayer()
{
    if (s_GomPlayer) {
        return move(s_GomPlayer);
	}

    s_GomPlayer = std::unique_ptr<App>(new App("Gom Player", "GomPlayer1.x", "D:\\Programs\\GomPlayer\\GOM.exe"));
    return move(s_GomPlayer);
}

void App::FreeGomPlayer()
{
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
    throw platform_exception("App::Close()");
# endif
}

/** 
 * Stretch l'application si elle est ouverte
 */
std::string App::Stretch()
{
# if defined(WINDOWS_PLATFORM)
	std::string resultMessage;
	
	bstr_t className(m_class_name.c_str());
	auto window = FindWindow(className, nullptr);
	if (!window) {
		resultMessage = m_label + " is not open.";
	} else {
		keyboard::CtrlEnter();
		resultMessage = m_label + " has been stretched.";
	}

	std::cout << resultMessage << std::endl; 
	return resultMessage;
# else
    // TODO: Code Linux function for App::Stretch()
    throw platform_exception("App::Stretch()");
# endif
}



