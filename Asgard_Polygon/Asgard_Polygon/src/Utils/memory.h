#pragma once
#include <Windows.h>
#include <mutex>
#include <vector>

bool get_key_state(int key, int flag = 1);

//Driver class removed

HWND get_process_wnd(HANDLE pid);
