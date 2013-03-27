#include "MonUtils.h"
#include <iostream>

#define ERROR_INDEX -1
#define FIRST_MON_ID 0

// TODO: comments FR => EN
// Initialisation du vecteur
std::vector<HMONITOR> MonUtils::s_vcMonitors = std::vector<HMONITOR>();

/**
 * CALLBACK permettant de lister les moniteurs
 */
BOOL CALLBACK MonUtils::MonitorEnumProc(HMONITOR monitor, HDC hdcMonitor, LPRECT lpMonitorRect, LPARAM dwData)
{	
	s_vcMonitors.push_back(monitor);
	return TRUE;
}

/**
 * @param monitor Le moniteur dont on veut connaitre l'index
 * @return L'index du moniteur courant.
 * Retourne -1 en cas d'erreur.
 */
int MonUtils::GetMonitorIndex(HMONITOR monitor)
{
	const int monitorsCount = s_vcMonitors.size();
	for (auto i = 0; i < monitorsCount; i++) {
		if (s_vcMonitors[i] == monitor)
			return i;
	}

	return ERROR_INDEX;
}

/**
 * @param monitor Le moniteur précédent celui dont on veut connaitre l'index.
 * @return L'index du moniteur suivant. 
 * Si on est sur le dernier moniteur, on boucle sur le premier.
 * Retourne -1 en cas d'erreur.
 */
int MonUtils::GetNextMonitorIndex(HMONITOR monitor)
{
	const auto monitorsCount	= GetSystemMetrics(SM_CMONITORS);
	const auto currentIndex		= GetMonitorIndex(monitor);

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
 * @param monitor Le moniteur précédent celui que l'on veut récupérer.
 * @return Le moniteur suivant celui passé en paramètre.
 */
HMONITOR MonUtils::GetNextMonitor(HMONITOR monitor)
{
	const auto nextMonitorIndex = GetNextMonitorIndex(monitor);

	if (nextMonitorIndex == ERROR_INDEX) {
		return nullptr;
	}
	
	return s_vcMonitors[nextMonitorIndex];
}

/**
 * @param monitor Le moniteur dont on veut connaitre les dimensions.
 * @return Le rectangle délimitant la position et les dimensions du moniteur.
 * Retourne nullptr en cas d'erreur.
 */
Rect* MonUtils::GetMonitorRect(HMONITOR monitor)
{
	MONITORINFO monitorInfo;
	monitorInfo.cbSize = sizeof(monitorInfo);
	
	if (GetMonitorInfo(monitor, &monitorInfo) != 0) {
		return new Rect(monitorInfo.rcMonitor);
	}
	
	return nullptr;
}

/**
 * @param window La fenêtre dont on veut connaitre la position relative.
 * @param rectMon La position du moniteur.
 * @return La position relative de la fenêtre par rappot au moniteur.
 * Retourne nullptr en cas d'erreur.
 */
Rect* MonUtils::GetRelativePos(HWND window, Rect rectMon)
{
	// Récuperation de la position de la fenêtre
	RECT windowPos;
	if ( GetWindowRect(window, &windowPos)  !=0 ) {
		auto* relativePos = new Rect();
		// Calcul de l'offset en pixels
		relativePos->left	= windowPos.left	- rectMon.left;
		relativePos->top	= windowPos.top		- rectMon.top;
		relativePos->right	= windowPos.right	- rectMon.left;
		relativePos->bottom	= windowPos.bottom	- rectMon.top;
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
PointF MonUtils::GetRectCoef(Rect rect1, Rect rect2)
{
	PointF coef;
	coef.x = (float) (rect2.getWidth())		/ (float) (rect1.getWidth());
	coef.y = (float) (rect2.getHeight())	/ (float) (rect1.getHeight());
	
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
	auto* rectOut = new Rect();
	rectOut->left	= (int) ((float)rectIn.left		* coef.x);
	rectOut->top	= (int) ((float)rectIn.top		* coef.y);
	rectOut->right	= (int) ((float)rectIn.right	* coef.x);
	rectOut->bottom	= (int) ((float)rectIn.bottom	* coef.y);

	return rectOut;
}

/**
 * Applique un décalage à un rectangle.
 * @param relativePosition La position relative de la fenêtre sur le moniteur
 * @param offsetX Le décalage à appliquer sur l'axe des abscisses
 * @param offsetY Le décalage à appliquer sur l'axe des ordonnées
 */
Rect* MonUtils::ApplyOffest(Rect relativePos, const int& offsetX, const int& offsetY)
{
	auto* absPos = new Rect();
	absPos->left	= relativePos.left		+ offsetX;
	absPos->top		= relativePos.top		+ offsetY;
	absPos->right	= relativePos.right		+ offsetX;
	absPos->bottom	= relativePos.bottom	+ offsetY;
	
	return absPos;
}

/**
 * Déplace la fenêtre vers l'écran spécifié
 * @param window HWND La fenêtre à déplacer.
 * @param monitor HMONITOR L'écran source.
 * @param monitor HMONITOR L'écran vers lequel on veut déplacer la fenêtre.
 * @return true si le déplacement est effectué.
 */
bool MonUtils::MoveWindow(HWND window, HMONITOR srcMonitor, HMONITOR destMonitor)
{
	if (srcMonitor == destMonitor) {
		return false;
	}

	// Récupèration de la position du moniteur source
	auto* srcMonPos = GetMonitorRect(srcMonitor);
	if (srcMonPos) {
		srcMonPos->display("srcMon");
	} else {
		std::cerr << "!!! GetMonitorRect(_srcMonitor) failed !!!" << std::endl;
	}
	
	// Récupèration de la position du nouveau moniteur
	auto* destMonPos = GetMonitorRect(destMonitor);
	if (destMonPos) {
		destMonPos->display("destMon");
	} else {
		std::cerr << "!!! GetMonitorRect(_destMonitor) failed !!!" << std::endl;
	}

	// Calcul de la position relative de la fenêtre par rapport au moniteur source
	auto* windowRelPos	= GetRelativePos(window, *srcMonPos); 
	if (windowRelPos) {
		windowRelPos->display("windowRelPos");
	} else {
		std::cerr << "!!! GetRelativePos(_window, *rectSrcMon) failed !!!" << std::endl;
	}

	// Calcul du coefficient d'agrandissement pour passer d'un moniteur à l'autre
	auto coef	= GetRectCoef(*srcMonPos, *destMonPos); 
	coef.display("coef");

	// Application du coef à la position relative de la fenêtre
	auto* newRelPos = ApplyCoef(*windowRelPos, coef);
	if (newRelPos) {
		newRelPos->display("newRelPos");
	} else {
		std::cout << "!!! ApplyCoef(*windowRelPos, coef) failed !!!" << std::endl;
	}

	// Calcul de la position absolue finale de la fenêtre
	auto* newPos = ApplyOffest(*newRelPos, destMonPos->left, destMonPos->top); 
	if (newPos) {
		newPos->display("newPos (final)");
	} else {
		std::cout << "!!! ApplyOffest(*newRelPos, rectDestMon->left, rectDestMon->top) failed !!!" << std::endl;
	}

	SetWindowPos(window, nullptr, newPos->left, newPos->top, newPos->getWidth(), newPos->getHeight(), SWP_SHOWWINDOW);
	
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
	const auto screenCount = GetSystemMetrics(SM_CMONITORS);
	std::cout << "Using " << screenCount << " screens." << std::endl << std::endl;

	if (screenCount < 2) { // Si on a pas au moins 2 moniteurs, on abandonne l'action en cours
		return false;
	}

	// On établit la liste des moniteurs
	EnumDisplayMonitors(nullptr, nullptr, MonitorEnumProc, 0);  

	// Récupération de la fenêtre au premier plan
	auto foregroundWindow = GetForegroundWindow();
	// Récupération de l'écran courant
	auto currentMonitor = MonitorFromWindow(foregroundWindow, MONITOR_DEFAULTTONEAREST);
	// Récupération du moniteur suivant
	auto nextMonitor = GetNextMonitor(currentMonitor);

	// Déplacement de la fenêtre
	return MoveWindow(foregroundWindow, currentMonitor, nextMonitor);
}