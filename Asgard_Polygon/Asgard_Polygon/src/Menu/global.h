#pragma once

enum targeting_type
{
	TARGET_VISIBLE,
	TARGET_FOV,
	TARGET_FOV_VISIBLE,
	TARGET_CLOSEST
};

struct GlobalVariables
{
	int width{ GetSystemMetrics(SM_CXSCREEN) };
	int height{ GetSystemMetrics(SM_CYSCREEN) };

	struct
	{
		IDirect3D9Ex* directObject;
		IDirect3DDevice9Ex* device;
		D3DPRESENT_PARAMETERS p_params;

		HWND overlay_hwnd;
		HWND game_hwnd;

	}dx9;

	struct
	{
		bool enabled;

		int aimbot_targeting_type;
		int target_bone;

		int hotkey{ VK_RBUTTON };

		bool fov_circle;
		float fov_size{ 220 };

		bool nospread;
		bool norecoil;

		struct
		{
			bool smoothing;
			float smoothing_speed{ 35 };
			bool randomness;
		}smoothness;

	}aimbot;

	struct
	{
		int boxType;
		int snapType;

		bool name;
		bool distance;
		bool health_bar;
		bool bones;
		bool view_angles;
		bool weapon;
	}esp;

	uint64_t base;
};
extern GlobalVariables* global;