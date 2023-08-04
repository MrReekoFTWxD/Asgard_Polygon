// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#pragma warning(disable : 4244)
#pragma warning(disable : 4552)
#pragma warning(disable : 4305)
#pragma warning(disable : 4312)
#pragma warning(disable : 4267)

#include <Windows.h>
#include <sdkddkver.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <stdexcept>
#include <type_traits>
#include <vector>
#include <map>
#include <unordered_map>
#include <thread>
#include <string>
#include <cstring>
#include <math.h>
#include <conio.h>
#include <detours.h>
#include <functional>
#include <optional>
#include <d3d11.h>
#include <fstream>
#include <deque>
#include <mutex>
#include <assert.h> 
#include <filesystem>
#include <codecvt>
#include <queue> 
#include <iterator>
#include <csetjmp>
#include <unordered_map>
#include <unordered_set>
#include <TlHelp32.h>
#include <d3d9.h>
#include <vector>
#include <dwmapi.h>

#define IMGUI_DEFINE_MATH_OPERATORS

#include "DirectX/imgui/imgui.h"
#include "DirectX/imgui/imgui_impl_dx9.h"
#include "DirectX/imgui/imgui_impl_win32.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dwmapi.lib")


#include "Utils/memory.h"
#include "Utils/string.h"
#include "Utils/access_band.h"
#include "Utils/Xors.h"

#include "DirectX/dx9.h"

#include "Menu/global.h"
#include "Menu/gui.h"

#include "Game/game.h"
#include "Thread/engine_thread.h"
#include "DirectX/rendering.h"

#include "Features/visuals.h"
#include "Features/aimbot.h"

#endif //PCH_H
