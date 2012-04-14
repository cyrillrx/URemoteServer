#pragma once

#include <windows.h>
#include <vector>
#include "Goemetry.h"

class MonUtils
{
private:
	static std::vector<HMONITOR> s_vcMonitors;
	
	static BOOL CALLBACK MonitorEnumProc(HMONITOR _monitor, HDC _hdcMonitor, LPRECT _lpMonitorRect, LPARAM _dwData);
	static int GetMonitorIndex(HMONITOR _monitor);
	static int GetNextMonitorIndex(HMONITOR _monitor);
	static HMONITOR GetNextMonitor(HMONITOR _monitor);
	static Rect* GetMonitorRect(HMONITOR _monitor);
	static Rect* GetRelativePos(HWND _window, Rect _rectMon);
	static PointF GetRectCoef(Rect _rect1, Rect _rect2);
	static Rect* ApplyCoef(Rect _rectIn, PointF _coef);
	static Rect* ApplyOffest(Rect _relativePos, int _offsetX, int _offsetY);
	static bool MoveWindow(HWND _window, HMONITOR _monitorSrc, HMONITOR _monitorDest);

public:
	static bool SwitchWindow();
};

