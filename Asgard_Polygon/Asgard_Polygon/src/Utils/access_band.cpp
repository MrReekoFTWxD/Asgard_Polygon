#include "pch.h"
#include "access_band.h"

#include <tchar.h>

CreateWindowInBand create_window_in_band = 0;

namespace band
{

	DWORD duplicate_winlogin_token(DWORD session_id, DWORD desired_access, PHANDLE token_phandle)
	{
		DWORD dwerr;
		PRIVILEGE_SET ps;
		ps.PrivilegeCount = 1;
		ps.Control = PRIVILEGE_SET_ALL_NECESSARY;
		if (LookupPrivilegeValue(NULL, SE_TCB_NAME, &ps.Privilege[0].Luid))
		{
			HANDLE snapshot_handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			if (INVALID_HANDLE_VALUE != snapshot_handle)
			{
				BOOL bcont, bfound = FALSE;
				PROCESSENTRY32 pe;
				pe.dwSize = sizeof(pe);
				dwerr = ERROR_NOT_FOUND;
				for (bcont = Process32First(snapshot_handle, &pe); bcont; bcont = Process32Next(snapshot_handle, &pe))
				{
					HANDLE process_handle;
					if (0 != _tcsicmp(pe.szExeFile, TEXT("winlogon.exe"))) continue;
					process_handle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pe.th32ProcessID);
					if (process_handle)
					{
						HANDLE token_handle;
						DWORD retlen, sid;
						if (OpenProcessToken(process_handle, TOKEN_QUERY | TOKEN_DUPLICATE, &token_handle))
						{
							BOOL ftcb;
							if (PrivilegeCheck(token_handle, &ps, &ftcb) && ftcb)
							{
								if (GetTokenInformation(token_handle, TokenSessionId, &sid, sizeof(sid), &retlen) && sid == session_id)
								{
									bfound = TRUE;
									if (DuplicateTokenEx(token_handle, desired_access, 0, SecurityImpersonation, TokenImpersonation, token_phandle))
									{
										dwerr = ERROR_SUCCESS;
									}
									else
									{
										dwerr = GetLastError();
									}
								}
							}
							CloseHandle(token_phandle);
						}
						CloseHandle(process_handle);
					}
					if (bfound) break;
				}
				CloseHandle(snapshot_handle);
			}
			else
			{
				dwerr = GetLastError();
			}
		}
		else
		{
			dwerr = GetLastError();
		}
		return dwerr;
	}

	DWORD create_ui_access_token(PHANDLE token_phandle)
	{
		DWORD dwerr;
		HANDLE token_self_handle;
		if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_DUPLICATE, &token_self_handle))
		{
			DWORD session_id, retlen;
			if (GetTokenInformation(token_self_handle, TokenSessionId, &session_id, sizeof(session_id), &retlen))
			{
				HANDLE token_system_handle;
				dwerr = duplicate_winlogin_token(session_id, TOKEN_IMPERSONATE, &token_system_handle);
				if (ERROR_SUCCESS == dwerr)
				{
					if (SetThreadToken(NULL, token_system_handle))
					{
						if (DuplicateTokenEx(token_self_handle, TOKEN_QUERY | TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY | TOKEN_ADJUST_DEFAULT, 0, SecurityAnonymous, TokenPrimary, token_phandle))
						{
							BOOL ui_access = TRUE;
							if (!SetTokenInformation(*token_phandle, TokenUIAccess, &ui_access, sizeof(ui_access)))
							{
								dwerr = GetLastError();
								CloseHandle(*token_phandle);
							}
						}
						else
						{
							dwerr = GetLastError();
						}
						RevertToSelf();
					}
					else
					{
						dwerr = GetLastError();
					}
					CloseHandle(token_system_handle);
				}
			}
			else
			{
				dwerr = GetLastError();
			}
			CloseHandle(token_self_handle);
		}
		else
		{
			dwerr = GetLastError();
		}
		return dwerr;
	}

	BOOL check_for_ui_acces(DWORD* pdwerr, DWORD* ui_access)
	{
		BOOL result = FALSE;
		HANDLE token_handle;
		if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token_handle))
		{
			DWORD retlen;
			if (GetTokenInformation(token_handle, TokenUIAccess, ui_access, sizeof(*ui_access), &retlen))
			{
				result = TRUE;
			}
			else
			{
				*pdwerr = GetLastError();
			}
			CloseHandle(token_handle);
		}
		else
		{
			*pdwerr = GetLastError();
		}
		return result;
	}

	DWORD prepare_for_ui_access()
	{
		DWORD dwerr;
		HANDLE token_ui_access_handle;
		BOOL ui_access;
		if (check_for_ui_acces(&dwerr, (DWORD*)&ui_access))
		{
			if (ui_access)
			{
				dwerr = ERROR_SUCCESS;
			}
			else
			{
				dwerr = create_ui_access_token(&token_ui_access_handle);
				if (ERROR_SUCCESS == dwerr)
				{
					STARTUPINFO si;
					PROCESS_INFORMATION pi;
					GetStartupInfo(&si);
					if (CreateProcessAsUser(token_ui_access_handle, 0, GetCommandLine(), 0, 0, FALSE, 0, 0, 0, &si, &pi))
					{
						CloseHandle(pi.hProcess), CloseHandle(pi.hThread);
						ExitProcess(0);
					}
					else
					{
						dwerr = GetLastError();
					}
					CloseHandle(token_ui_access_handle);
				}
			}
		}
		return dwerr;
	}


	bool init()
	{
		const DWORD err = prepare_for_ui_access();

		if (err != ERROR_SUCCESS)
			return false;

		const HMODULE user_dll = LoadLibraryA("user32.dll");

		if (!user_dll)
			return false;

		create_window_in_band = CreateWindowInBand(GetProcAddress(user_dll, "CreateWindowInBand"));

		if (!create_window_in_band)
		{
			const HMODULE win32_dll = LoadLibraryA("win32u.dll");

			if (!win32_dll)
				return false;

			create_window_in_band = CreateWindowInBand(GetProcAddress(win32_dll, "CreateWindowInBand"));

			if (!create_window_in_band)
				return false;
		}

		return true;

	}
}