#include <iostream>

#include "App.h"
#include "Keyboard.h"

//////////////////////////////////////////////////////////////////////////////
// Fonctions privées
//////////////////////////////////////////////////////////////////////////////

App* App::s_GomPlayer = NULL;

App::App(string _label, string _className, string _exePath)
{
	mLabel		= _label;
	mClassName	= _className;
	mExePath	= _exePath;
}

App::~App(void)
{
}

bool App::SetOnTop()
{
	HWND window = FindWindowA(mClassName.c_str(), NULL);
	if (!window) {
		cout << "Error while searching process." << endl;
		return false;
	}

	return SetForegroundWindow(window) == TRUE;
}

bool App::Launch()
{
	cout << "Launching " << mLabel << endl;
	HINSTANCE returnCode = ShellExecuteA(NULL, "open", mExePath.c_str(), "", NULL, SW_SHOWMAXIMIZED);
	//HINSTANCE returnCode = ShellExecuteA(NULL, "open", mExePath.c_str(), "", NULL, SW_SHOWMAXIMIZED);
	//LPCWSTR exePath = mExePath.c_str();
	//HINSTANCE returnCode = ShellExecute(NULL, L"open", exePath, L"", NULL, SW_SHOWMAXIMIZED);
	return ((int)returnCode > 32);
}

//////////////////////////////////////////////////////////////////////////////
// Fonctions publics
//////////////////////////////////////////////////////////////////////////////

App* App::GetGomPlayer() {
	if (s_GomPlayer)
		return s_GomPlayer;

	s_GomPlayer = new App("Gom Player", "GomPlayer1.x", "D:\\Programs\\GomPlayer\\GOM.exe");
	return s_GomPlayer;
}

void App::FreeGomPlayer() {
	delete(s_GomPlayer);
	s_GomPlayer = NULL;
}

/* 
 *! Met l'application au premier plan
 *! Lance l'application si elle n'est pas lancée
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

/* 
 *! Ferme l'application si elle est ouverte
 */
string App::Close()
{
	string resultMessage;

	HWND window = FindWindowA(mClassName.c_str(), NULL);
	if (!window) {
		resultMessage = mLabel + " is not open.";
	} else {
		PostMessage(window, WM_CLOSE, 0, 0);
		resultMessage = mLabel + " has been closed.";
	}

	cout << resultMessage << endl; 
	return resultMessage;
}

/* 
 *! Stretch l'application si elle est ouverte
 */
string App::Strech()
{
	string resultMessage;

	HWND window = FindWindowA(mClassName.c_str(), NULL);
	if (!window) {
		resultMessage = mLabel + " is not open.";
	} else {
		Keyboard::CtrlEnter();
		resultMessage = mLabel + " has been streched.";
	}

	cout << resultMessage << endl; 
	return resultMessage;
}



