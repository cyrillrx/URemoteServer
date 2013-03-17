#include "App.h"

#include <iostream>

#include "Keyboard.h"
#include "exception/config_exception.h"

# if defined(WINDOWS_PLATFORM)
#	include <comdef.h>
# endif

using namespace std;

//////////////////////////////////////////////////////////////////////////////
// Fonctions privées
//////////////////////////////////////////////////////////////////////////////

unique_ptr<App> App::s_GomPlayer = nullptr;

App::App(string label, string className, string exePath)
{
	mLabel		= label;
	mClassName	= className;
	mExePath	= exePath;
}

App::~App(void)
{
}

bool App::SetOnTop()
{
# if defined(WINDOWS_PLATFORM)
	bstr_t className(mClassName.c_str());
	HWND window = FindWindow(className, nullptr);
	if (!window) {
		cout << "Error while searching process." << endl;
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
	cout << "Launching " << mLabel << endl;
	bstr_t path(mExePath.c_str());
	HINSTANCE returnCode = ShellExecute(nullptr, L"open", path, L"", nullptr, SW_SHOWMAXIMIZED);
	return ((int)returnCode > 32);
# else
	// TODO: Code Linux function for App::Launch()
# endif
	throw platform_exception("Timer::frequency()");
}

//////////////////////////////////////////////////////////////////////////////
// Fonctions publics
//////////////////////////////////////////////////////////////////////////////

unique_ptr<App> App::GetGomPlayer() {
	if (s_GomPlayer)
		return move(s_GomPlayer);

	s_GomPlayer = unique_ptr<App>(new App("Gom Player", "GomPlayer1.x", "D:\\Programs\\GomPlayer\\GOM.exe"));
	return move(s_GomPlayer);
}

void App::FreeGomPlayer() {
	s_GomPlayer = nullptr;
}

/** 
 * Met l'application au premier plan
 * Lance l'application si elle n'est pas lancée
 */
string App::Show()
{
	string resultMessage;

	if (!SetOnTop()) {
		cout << "Could not bring " << mLabel << " to the front." << endl;
		cout << "Launching " << mLabel << endl;
		if (Launch()) {
			resultMessage = "Launching " + mLabel;
		} else {
			resultMessage = "Couldn't launch " + mLabel;
		}
	} else {
		resultMessage = mLabel + " is now on top.";
	}

	cout << resultMessage << endl; 
	return resultMessage;
}

/** 
 * Ferme l'application si elle est ouverte
 */
string App::Close()
{
# if defined(WINDOWS_PLATFORM)
	string resultMessage;
	
	bstr_t className(mClassName.c_str());
	HWND window = FindWindow(className, nullptr);
	if (!window) {
		resultMessage = mLabel + " is not open.";
	} else {
		PostMessage(window, WM_CLOSE, 0, 0);
		resultMessage = mLabel + " has been closed.";
	}

	cout << resultMessage << endl; 
	return resultMessage;
# else
	// TODO: Code Linux function for App::Close()
# endif
	throw platform_exception("App::Close()");
}

/** 
 * Stretch l'application si elle est ouverte
 */
string App::Strech()
{
# if defined(WINDOWS_PLATFORM)
	string resultMessage;
	
	bstr_t className(mClassName.c_str());
	HWND window = FindWindow(className, nullptr);
	if (!window) {
		resultMessage = mLabel + " is not open.";
	} else {
		Keyboard::CtrlEnter();
		resultMessage = mLabel + " has been streched.";
	}

	cout << resultMessage << endl; 
	return resultMessage;
# else
	// TODO: Code Linux function for App::Strech()
# endif
	throw platform_exception("App::Strech()");
}



