#pragma once

#include <windows.h>
#include <vector>
#include "Goemetry.h"

class MonUtils
{
public:
	static bool SwitchWindow();

private:
	static std::vector<HMONITOR> s_vcMonitors;
	
	static BOOL CALLBACK MonitorEnumProc(HMONITOR monitor, HDC hdcMonitor, LPRECT lpMonitorRect, LPARAM dwData);
	static int GetMonitorIndex(HMONITOR monitor);
	static int GetNextMonitorIndex(HMONITOR monitor);
	static HMONITOR GetNextMonitor(HMONITOR monitor);
	static Rect* GetMonitorRect(HMONITOR monitor);
	static Rect* GetRelativePos(HWND window, Rect rectMon);
	static PointF GetRectCoef(Rect rect1, Rect rect2);
	static Rect* ApplyCoef(Rect rectIn, PointF coef);
	static Rect* ApplyOffest(Rect relativePos, int offsetX, int offsetY);
	static bool MoveWindow(HWND window, HMONITOR monitorSrc, HMONITOR monitorDest);
};