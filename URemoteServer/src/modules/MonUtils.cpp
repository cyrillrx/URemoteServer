#include "MonUtils.h"
#include <iostream>

#define ERROR_INDEX -1
#define FIRST_MON_ID 0

// Initialisation du vecteur
std::vector<HMONITOR> MonUtils::s_vcMonitors = std::vector<HMONITOR>();

/**
 * CALLBACK permettant de lister les moniteurs
 */
BOOL CALLBACK MonUtils::MonitorEnumProc(HMONITOR _monitor, HDC _hdcMonitor, LPRECT _lpMonitorRect, LPARAM _dwData)
{	
	s_vcMonitors.push_back(_monitor);
	return TRUE;
}

/**
 * @param _monitor Le moniteur dont on veut connaitre l'index
 * @return L'index du moniteur courant.
 * Retourne -1 en cas d'erreur.
 */
int MonUtils::GetMonitorIndex(HMONITOR _monitor)
{
	const int monitorsCount = s_vcMonitors.size();
	for (int i= 0; i < monitorsCount; i++) {
		if (s_vcMonitors[i] == _monitor)
			return i;
	}

	return ERROR_INDEX;
}

/**
 * @param _monitor Le moniteur précédent celui dont on veut connaitre l'index.
 * @return L'index du moniteur suivant. 
 * Si on est sur le dernier moniteur, on boucle sur le premier.
 * Retourne -1 en cas d'erreur.
 */
int MonUtils::GetNextMonitorIndex(HMONITOR _monitor)
{
	const int monitorsCount = GetSystemMetrics(SM_CMONITORS);
	const int currentIndex = GetMonitorIndex(_monitor);

	// Si GetMonitorIndex a renvoyé une erreur
	if (currentIndex == ERROR_INDEX) {
		return ERROR_INDEX;

	// Si Le moniteur n'est pas le dernier moniteur de la liste
	} else if (currentIndex < monitorsCount -1) {
		return currentIndex + 1;
	}
	
	// Le moniteur est le dernier, on boucle sur le premier
	return FIRST_MON_ID;
}

/**
 * @param _monitor Le moniteur précédent celui que l'on veut récupérer.
 * @return Le moniteur suivant celui passé en paramètre.
 */
HMONITOR MonUtils::GetNextMonitor(HMONITOR _monitor)
{
	int nextMonitorIndex = GetNextMonitorIndex(_monitor);

	if (nextMonitorIndex == ERROR_INDEX)
		return nullptr;
	
	return s_vcMonitors[nextMonitorIndex];
}

/**
 * @param _monitor Le moniteur dont on veut connaitre les dimensions.
 * @return Le rectangle délimitant la position et les dimensions du moniteur.
 * Retourne nullptr en cas d'erreur.
 */
Rect* MonUtils::GetMonitorRect(HMONITOR _monitor)
{
	MONITORINFO monitorInfo;
	monitorInfo.cbSize = sizeof(monitorInfo);
	
	if (GetMonitorInfo(_monitor, &monitorInfo) != 0) {
		return new Rect(monitorInfo.rcMonitor);
	}
	
	return nullptr;
}

/**
 * @param _window La fenêtre dont on veut connaitre la position relative.
 * @param _rectMon La position du moniteur.
 * @return La position relative de la fenêtre par rappot au moniteur.
 * Retourne nullptr en cas d'erreur.
 */
Rect* MonUtils::GetRelativePos(HWND _window, Rect _rectMon)
{
	// Récuperation de la position de la fenêtre
	RECT windowPos;
	if ( GetWindowRect(_window, &windowPos)  !=0 ) {
		Rect *relativePos = new Rect();
		// Calcul de l'offset en pixels
		relativePos->left	= windowPos.left	- _rectMon.left;
		relativePos->top	= windowPos.top		- _rectMon.top;
		relativePos->right	= windowPos.right	- _rectMon.left;
		relativePos->bottom	= windowPos.bottom	- _rectMon.top;
		return relativePos;
	}

	return nullptr;
}

/**
 * Permet de calculer les coefficients de redimensionnement en largeur et en hauteur entre deux rectangles donnés
 * @param _rec1 Le rectangle 1
 * @param _rec2 Le rectangle 2
 * @return Le coefficient de transition entre les deux rectangles.
 */
PointF MonUtils::GetRectCoef(Rect _rect1, Rect _rect2)
{
	PointF coef;
	coef.x = (float) (_rect2.GetWidth()) / (float) (_rect1.GetWidth());
	coef.y = (float) (_rect2.GetHeight()) / (float) (_rect1.GetHeight());
	
	return coef;
}

/**
 * Application d'un coefficient à un rectange.
 * @param rectIn Le rectangle à redimensionner.
 * @param coef Le coefficient à appliquer.
 * @return Le rectangle redimensionné.
 */
Rect* MonUtils::ApplyCoef(Rect rectIn, PointF coef)
{
	Rect *rectOut = new Rect();
	rectOut->left	= (int) ((float)rectIn.left		* coef.x);
	rectOut->top	= (int) ((float)rectIn.top		* coef.y);
	rectOut->right	= (int) ((float)rectIn.right	* coef.x);
	rectOut->bottom	= (int) ((float)rectIn.bottom	* coef.y);

	return rectOut;
}

/**
 * Applique un décalage à un rectangle.
 * @param _relativePosition La position relative de la fenêtre sur le moniteur
 * @param _offsetX Le décalage à appliquer sur l'axe des abscisses
 * @param _offsetY Le décalage à appliquer sur l'axe des ordonnées
 */
Rect* MonUtils::ApplyOffest(Rect _relativePos, int _offsetX, int _offsetY)
{
	Rect *absPos = new Rect();
	absPos->left	= _relativePos.left	+ _offsetX;
	absPos->top		= _relativePos.top	+ _offsetY;
	absPos->right	= _relativePos.right	+ _offsetX;
	absPos->bottom	= _relativePos.bottom	+ _offsetY;
	
	return absPos;
}

/**
 * Déplace la fenêtre vers l'écran spécifié
 * @param _window HWND La fenêtre à déplacer.
 * @param _monitor HMONITOR L'écran source.
 * @param _monitor HMONITOR L'écran vers lequel on veut déplacer la fenêtre.
 * @return true si le déplacement est effectué.
 */
bool MonUtils::MoveWindow(HWND _window, HMONITOR _srcMonitor, HMONITOR _destMonitor)
{
	if (_srcMonitor == _destMonitor) {
		return false;
	}

	// Récupèration de la position du moniteur source
	Rect *srcMonPos = GetMonitorRect(_srcMonitor);
	if (srcMonPos) {
		srcMonPos->Display("srcMon");
	} else {
		std::cerr << "!!! GetMonitorRect(_srcMonitor) failed !!!" << std::endl;
	}
	
	// Récupèration de la position du nouveau moniteur
	Rect *destMonPos = GetMonitorRect(_destMonitor);
	if (destMonPos) {
		destMonPos->Display("destMon");
	} else {
		std::cerr << "!!! GetMonitorRect(_destMonitor) failed !!!" << std::endl;
	}

	// Calcul de la position relative de la fenêtre par rapport au moniteur source
	Rect *windowRelPos	= GetRelativePos(_window, *srcMonPos); 
	if (windowRelPos) {
		windowRelPos->Display("windowRelPos");
	} else {
		std::cerr << "!!! GetRelativePos(_window, *rectSrcMon) failed !!!" << std::endl;
	}

	// Calcul du coefficient d'agrandissement pour passer d'un moniteur à l'autre
	PointF coef	= GetRectCoef(*srcMonPos, *destMonPos); 
	coef.Display("coef");

	// Application du coef à la position relative de la fenêtre
	Rect *newRelPos = ApplyCoef(*windowRelPos, coef);
	if (newRelPos) {
		newRelPos->Display("newRelPos");
	} else {
		std::cout << "!!! ApplyCoef(*windowRelPos, coef) failed !!!" << std::endl;
	}

	// Calcul de la position absolue finale de la fenêtre
	Rect *newPos = ApplyOffest(*newRelPos, destMonPos->left, destMonPos->top); 
	if (newPos) {
		newPos->Display("newPos (final)");
	} else {
		std::cout << "!!! ApplyOffest(*newRelPos, rectDestMon->left, rectDestMon->top) failed !!!" << std::endl;
	}

	SetWindowPos(_window, nullptr, newPos->left, newPos->top, newPos->GetWidth(), newPos->GetHeight(), SWP_SHOWWINDOW);
	
	// Suppression des pointeurs créés
	delete(srcMonPos);
	delete(destMonPos);
	delete(windowRelPos);
	delete(newRelPos);
	delete(newPos);

	return true;
}

/**
 * Déplace la fenêtre courante de l'écran dans lequel elle se trouve vers l'écran suivant.
 * @return true si le déplacement est effectué.
 */
bool MonUtils::SwitchWindow()
{
	// On récupère le nombre de moniteur
	int screenCount = GetSystemMetrics(SM_CMONITORS);
	std::cout << "Using " << screenCount << " screens." << std::endl << std::endl;

	if (screenCount < 2) { // Si on a pas au moins 2 moniteurs, on abandonne l'action en cours
		return false;
	}

	// On établit la liste des moniteurs
	EnumDisplayMonitors(nullptr, nullptr, MonitorEnumProc, 0);  

	// Récupération de la fenêtre au premier plan
	HWND foregroundWindow = GetForegroundWindow();
	// Récupération de l'écran courant
	HMONITOR currentMonitor = MonitorFromWindow(foregroundWindow, MONITOR_DEFAULTTONEAREST);
	// Récupération du moniteur suivant
	HMONITOR nextMonitor = GetNextMonitor(currentMonitor);

	// Déplacement de la fenêtre
	return MoveWindow(foregroundWindow, currentMonitor, nextMonitor);
}