#include "pch.h"
#include "memory.h"


bool get_key_state(int key, int flag)
{
		/*if (FindWindowA("UnrealWindow", 0) != GetForegroundWindow()) seems broken when trying to find window now 
			return false;*/

	clock_t	c = clock();
	static BYTE			btKeyState[0x100] = { 0 };
	static clock_t		clockKeyState[0x100] = { c };

	bool	r = false;
	BYTE	btState = (GetAsyncKeyState(key) & 0x8000U) >> 0xF;
	short	wDelay = btKeyState[key] & 2 ? 0x50 : 0x200;
	if (btState) {
		if (flag & 1) {
			if (!(btKeyState[key] & 1) || c - clockKeyState[key] > wDelay)
			{
				if ((btKeyState[key] & 3) == 1)
					btKeyState[key] |= 2;
				r = true;
				clockKeyState[key] = c;
			}
		}
		else
			r = true;
	}
	else
		btKeyState[key] &= ~(2);
	btKeyState[key] ^= (-btState ^ btKeyState[key]) & 1;

	return r;
}

HWND get_process_wnd(HANDLE pid)
{
	std::pair<HWND, HANDLE> params = { 0, pid };
	BOOL bresult = EnumWindows([](HWND hwnd, LPARAM lparam) -> BOOL
		{
			auto pparams = (std::pair<HWND, uint32_t>*)(lparam);
			uint32_t processid = 0;
			if (GetWindowThreadProcessId(hwnd, reinterpret_cast<LPDWORD>(&processid)) && processid == pparams->second)
			{
				SetLastError((uint32_t)-1);
				pparams->first = hwnd;
				return FALSE;
			}
			return TRUE;
		}, (LPARAM)&params);
	if (!bresult && GetLastError() == -1 && params.first) return params.first;
	return 0;
}
