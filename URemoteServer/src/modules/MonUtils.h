#pragma once

#include "platform_config.h"
# if defined(WINDOWS_PLATFORM)
#   include <windows.h>
# endif

#include <vector>
#include "goemetry.h"

class MonUtils
{
public:
	static bool SwitchWindow();

private:

# if defined(WINDOWS_PLATFORM)
	static std::vector<HMONITOR> monitors_;

	static BOOL CALLBACK MonitorEnumProc(HMONITOR monitor, HDC hdcMonitor, LPRECT lpMonitorRect, LPARAM dwData);
	static int GetMonitorIndex(HMONITOR monitor);
	static int GetNextMonitorIndex(HMONITOR monitor);
	static HMONITOR GetNextMonitor(HMONITOR monitor);
	static rect* GetMonitorRect(HMONITOR monitor);
	static rect* GetRelativePos(HWND window, rect rectMon);
	static pointf GetRectCoef(rect rect1, rect rect2);
	static rect* ApplyCoef(rect rectIn, pointf coef);
	static rect* ApplyOffest(rect relativePos, const int& offsetX, const int& offsetY);
	static bool MoveWindow(HWND window, HMONITOR monitorSrc, HMONITOR monitorDest);
# endif
};
