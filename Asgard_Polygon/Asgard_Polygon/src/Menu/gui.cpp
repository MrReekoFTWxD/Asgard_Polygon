#include "pch.h"
#include "gui.h"

#include "DirectX/imgui/imgui_internal.h"

MSG messager = { NULL };

//Hate this method but eh dont really feel like redoing it for time being
const char* const key_names[] = { "None", "VK_LBUTTON", "VK_RBUTTON", "VK_CANCEL", "VK_MBUTTON", "VK_XBUTTON1", "VK_XBUTTON2", "Unknown", "VK_BACK", "VK_TAB", "Unknown", "Unknown", "VK_CLEAR", "VK_RETURN", "Unknown", "Unknown", "VK_SHIFT", "VK_CONTROL", "VK_MENU", "VK_PAUSE", "VK_CAPITAL", "VK_KANA", "Unknown", "VK_JUNJA", "VK_FINAL", "VK_KANJI", "Unknown", "VK_ESCAPE", "VK_CONVERT", "VK_NONCONVERT", "VK_ACCEPT", "VK_MODECHANGE", "VK_SPACE", "VK_PRIOR", "VK_NEXT", "VK_END", "VK_HOME", "VK_LEFT", "VK_UP", "VK_RIGHT", "VK_DOWN", "VK_SELECT", "VK_PRINT", "VK_EXECUTE", "VK_SNAPSHOT", "VK_INSERT", "VK_DELETE", "VK_HELP", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "VK_LWIN", "VK_RWIN", "VK_APPS", "Unknown", "VK_SLEEP", "VK_NUMPAD0", "VK_NUMPAD1", "VK_NUMPAD2", "VK_NUMPAD3", "VK_NUMPAD4", "VK_NUMPAD5", "VK_NUMPAD6", "VK_NUMPAD7", "VK_NUMPAD8", "VK_NUMPAD9", "VK_MULTIPLY", "VK_ADD", "VK_SEPARATOR", "VK_SUBTRACT", "VK_DECIMAL", "VK_DIVIDE", "VK_F1", "VK_F2", "VK_F3", "VK_F4", "VK_F5", "VK_F6", "VK_F7", "VK_F8", "VK_F9", "VK_F10", "VK_F11", "VK_F12", "VK_F13", "VK_F14", "VK_F15", "VK_F16", "VK_F17", "VK_F18", "VK_F19", "VK_F20", "VK_F21", "VK_F22", "VK_F23", "VK_F24", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "VK_NUMLOCK", "VK_SCROLL", "VK_OEM_NEC_EQUAL", "VK_OEM_FJ_MASSHOU", "VK_OEM_FJ_TOUROKU", "VK_OEM_FJ_LOYA", "VK_OEM_FJ_ROYA", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "VK_LSHIFT", "VK_RSHIFT", "VK_LCONTROL", "VK_RCONTROL", "VK_LMENU", "VK_RMENU" };
std::map<int, std::string> keyMap = {
	{0x00, "None"},
	{0x01, "VK_LBUTTON"},
	{0x02, "VK_RBUTTON"},
	{0x03, "VK_CANCEL"},
	{0x04, "VK_MBUTTON"},
	{0x05, "VK_XBUTTON1"},
	{0x06, "VK_XBUTTON2"},
	{0x08, "VK_BACK"},
	{0x09, "VK_TAB"},
	{0x0C, "VK_CLEAR"},
	{0x0D, "VK_RETURN"},
	{0x10, "VK_SHIFT"},
	{0x11, "VK_CONTROL"},
	{0x12, "VK_MENU"},
	{0x13, "VK_PAUSE"},
	{0x14, "VK_CAPITAL"},
	{0x15, "VK_KANA"},
	{0x17, "VK_JUNJA"},
	{0x18, "VK_FINAL"},
	{0x19, "VK_KANJI"},
	{0x1B, "VK_ESCAPE"},
	{0x1C, "VK_CONVERT"},
	{0x1D, "VK_NONCONVERT"},
	{0x1E, "VK_ACCEPT"},
	{0x1F, "VK_MODECHANGE"},
	{0x20, "VK_SPACE"},
	{0x21, "VK_PRIOR"},
	{0x22, "VK_NEXT"},
	{0x23, "VK_END"},
	{0x24, "VK_HOME"},
	{0x25, "VK_LEFT"},
	{0x26, "VK_UP"},
	{0x27, "VK_RIGHT"},
	{0x28, "VK_DOWN"},
	{0x2C, "VK_PRINT"},
	{0x2D, "VK_INSERT"},
	{0x2E, "VK_DELETE"},
	{0x2F, "VK_HELP"},
	{0x30, "0"},
	{0x31, "1"},
	{0x32, "2"},
	{0x33, "3"},
	{0x34, "4"},
	{0x35, "5"},
	{0x36, "6"},
	{0x37, "7"},
	{0x38, "8"},
	{0x39, "9"},
	{0x41, "A"},
	{0x42, "B"},
	{0x43, "C"},
	{0x44, "D"},
	{0x45, "E"},
	{0x46, "F"},
	{0x47, "G"},
	{0x48, "H"},
	{0x49, "I"},
	{0x4A, "J"},
	{0x4B, "K"},
	{0x4C, "L"},
	{0x4D, "M"},
	{0x4E, "N"},
	{0x4F, "O"},
	{0x50, "P"},
	{0x51, "Q"},
	{0x52, "R"},
	{0x53, "S"},
	{0x54, "T"},
	{0x55, "U"},
	{0x56, "V"},
	{0x57, "W"},
	{0x58, "X"},
	{0x59, "Y"},
	{0x5A, "Z"},
	{0x5B, "VK_LWIN"},
	{0x5C, "VK_RWIN"},
	{0x5D, "VK_APPS"},
	{0x5F, "VK_SLEEP"},
	{0x60, "VK_NUMPAD0"},
	{0x61, "VK_NUMPAD1"},
	{0x62, "VK_NUMPAD2"},
	{0x63, "VK_NUMPAD3"},
	{0x64, "VK_NUMPAD4"},
	{0x65, "VK_NUMPAD5"},
	{0x66, "VK_NUMPAD6"},
	{0x67, "VK_NUMPAD7"},
	{0x68, "VK_NUMPAD8"},
	{0x69, "VK_NUMPAD9"},
	{0x6A, "VK_MULTIPLY"},
	{0x6B, "VK_ADD"},
	{0x6C, "VK_SEPARATOR"},
	{0x6D, "VK_SUBTRACT"},
	{0x6E, "VK_DECIMAL"},
	{0x6F, "VK_DIVIDE"},
	{0x70, "VK_F1"},
	{0x71, "VK_F2"},
	{0x72, "VK_F3"},
	{0x73, "VK_F4"},
	{0x74, "VK_F5"},
	{0x75, "VK_F6"},
	{0x76, "VK_F7"},
	{0x77, "VK_F8"},
	{0x78, "VK_F9"},
	{0x79, "VK_F10"},
	{0x7A, "VK_F11"},
	{0x7B, "VK_F12"},
	{0x7C, "VK_F13"},
	{0x7D, "VK_F14"},
	{0x7E, "VK_F15"},
	{0x7F, "VK_F16"},
	{0x80, "VK_F17"},
	{0x81, "VK_F18"},
	{0x82, "VK_F19"},
	{0x83, "VK_F20"},
	{0x84, "VK_F21"},
	{0x85, "VK_F22"},
	{0x86, "VK_F23"},
	{0x87, "VK_F24"},
	{0x90, "VK_NUMLOCK"},
	{0x91, "VK_SCROLL"},
	{0x92, "VK_OEM_NEC_EQUAL"},
	{0x93, "VK_OEM_FJ_MASSHOU"},
	{0x94, "VK_OEM_FJ_TOUROKU"},
	{0x95, "VK_OEM_FJ_LOYA"},
	{0x96, "VK_OEM_FJ_ROYA"},
	{0xA0, "VK_LSHIFT"},
	{0xA1, "VK_RSHIFT"},
	{0xA2, "VK_LCONTROL"},
	{0xA3, "VK_RCONTROL"},
	{0xA4, "VK_LMENU"},
	{0xA5, "VK_RMENU"}
};


namespace ImGui
{
	void Hotkey(const std::string& label, int* k, const ImVec2& size_arg = ImVec2(0, 0))
	{
		static bool waitingforkey = false;

		if (get_key_state(VK_DELETE))
		{
			*k = 0;
			waitingforkey = false;
		}

		if (!waitingforkey)
		{
			if (ImGui::Button(keyMap[*k].c_str(), size_arg))
				waitingforkey = true;
			
		}
		else
		{
			ImGui::Button("Press any key to set as hotkey...", size_arg);
			


				// Check for a key press
				for (auto& key : keyMap)
				{
					if (get_key_state(key.first))
					{
						*k = key.first;
						waitingforkey = false;
						break;
					}
				}

		
			
		}
		ImGui::SameLine();
		ImGui::Text(label.c_str());
	}
}

namespace gui
{
	void instance()
	{
		static RECT old;
		ZeroMemory(&messager, sizeof(MSG));
		while (messager.message != WM_QUIT)
		{
			if (PeekMessage(&messager, global->dx9.overlay_hwnd, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&messager);
				DispatchMessage(&messager);
			}

			if (global->dx9.game_hwnd == NULL)
				exit(0);

			HWND active_hwnd = GetForegroundWindow();
			if (active_hwnd == global->dx9.game_hwnd)
			{
				HWND target_wnd = GetWindow(active_hwnd, GW_HWNDPREV);
				SetWindowPos(global->dx9.overlay_hwnd, target_wnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			}
			else
			{
				global->dx9.game_hwnd = get_process_wnd(driver.process_id);
				Sleep(250);
			}

			RECT rc;
			POINT xy;
			ZeroMemory(&rc, sizeof(RECT));
			ZeroMemory(&xy, sizeof(POINT));

			rc.left = xy.x;
			rc.top = xy.y;
			ImGuiIO& io = ImGui::GetIO();
			io.DeltaTime = 1.0f / 60.0f;
			POINT p;
			GetCursorPos(&p);
			io.MousePos.x = p.x - xy.x;
			io.MousePos.y = p.y - xy.y;
			if (GetAsyncKeyState(0x1))
			{
				io.MouseDown[0] = true;
				io.MouseClicked[0] = true;
				io.MouseClickedPos[0].x = io.MousePos.x;
				io.MouseClickedPos[0].x = io.MousePos.y;
			}
			else
			{
				io.MouseDown[0] = false;
			}
			if (rc.left != old.left || rc.right != old.right || rc.top != old.top || rc.bottom != old.bottom)
			{
				old = rc;
				global->width = rc.right;
				global->height = rc.bottom;
				global->dx9.p_params.BackBufferWidth = global->width;
				global->dx9.p_params.BackBufferHeight = global->height;
				SetWindowPos(global->dx9.overlay_hwnd, (HWND)0, xy.x, xy.y, global->width, global->height, SWP_NOREDRAW);
				global->dx9.device->Reset(&global->dx9.p_params);
			}

			ImGui_ImplDX9_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			if (get_key_state(VK_F12))
				exit(0);

			if (get_key_state(VK_F5))
				gui::set_rendering_state(!gui::rendering_state());

			gui::menu();
			render_esp();

			if(global->aimbot.fov_circle)
				game_render_circle(global->width / 2, global->height / 2, global->aimbot.fov_size, { 255,255,255 }, RFOutline);

			ImGui::EndFrame();
			global->dx9.device->SetRenderState(D3DRS_ZENABLE, false);
			global->dx9.device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
			global->dx9.device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
			global->dx9.device->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

			if (global->dx9.device->BeginScene() >= 0)
			{
				ImGui::Render();
				ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
				global->dx9.device->EndScene();
			}
			HRESULT result = global->dx9.device->Present(0, 0, 0, 0);
			if (result == D3DERR_DEVICELOST && global->dx9.device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
			{
				ImGui_ImplDX9_InvalidateDeviceObjects();
				global->dx9.device->Reset(&global->dx9.p_params);
				ImGui_ImplDX9_CreateDeviceObjects();
			}
		}
	}

	bool is_rendering = false;

	void set_rendering_state(bool value)
	{
		is_rendering = value;
	}

	bool rendering_state()
	{
		ImGui::GetIO().MouseDrawCursor = is_rendering;
		return is_rendering;
	}

	void menu()
	{
		if (!rendering_state())
			return;

		ImGui::SetNextWindowSize({ 450, 310 }, ImGuiCond_::ImGuiCond_Appearing);
		if (ImGui::Begin(Xors("Asgard [Polygon] by MrReeko")))
		{

			if (ImGui::BeginTabBar(Xors("##sections_")))
			{
				if (ImGui::BeginTabItem(Xors("Aimbot")))
				{

					ImGui::Checkbox("Aimbot", &global->aimbot.enabled);
					ImGui::Combo("Targeting Mode", &global->aimbot.aimbot_targeting_type, { Xors("Visible Only"), Xors("FOV"), Xors("FOV + Visible"), Xors("Closest") });
					ImGui::Combo("Targeting Bone", &global->aimbot.target_bone, { Xors("Head"),Xors("Neck"),Xors("Spine1"),Xors("Spine2"),Xors("Spine3"), Xors("Left Arm"), Xors("Right Arm") });

					auto width = ImGui::GetItemRectSize();

					ImGui::Hotkey("Aimbot Hotkey", &global->aimbot.hotkey, { width.x - 100 , 18});

					ImGui::Separator();

					ImGui::Checkbox("NoSpread", &global->aimbot.nospread);
					ImGui::Checkbox("NoRecoil", &global->aimbot.norecoil);

					ImGui::Checkbox("smoothing", &global->aimbot.smoothness.smoothing);

					if (global->aimbot.smoothness.smoothing)
					{
						ImGui::Checkbox("Smoothing Randomness", &global->aimbot.smoothness.randomness);
						ImGui::SliderFloat("Smoothing Factor", &global->aimbot.smoothness.smoothing_speed, 0, 40, "%.0f");
					}
					
					ImGui::Checkbox("Draw FOV Circle", &global->aimbot.fov_circle);

					if (global->aimbot.fov_circle)
						ImGui::SliderFloat("FOV Circle Size", &global->aimbot.fov_size, 50, 420, "%.0f");

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem(Xors("Visuals")))
				{
					ImGui::Combo(Xors("Box Type##player_esp"), &global->esp.boxType, { Xors("None"), Xors("2D"), Xors("3D"), Xors("3D Rotated"), Xors("Corner"), Xors("Filled") });
					ImGui::Combo(Xors("Snaplines##player_esp"), &global->esp.snapType, { Xors("None"), Xors("Top"), Xors("Middle"), Xors("Bottom") });

					ImGui::Separator();

					ImGui::Checkbox(Xors("Draw Name##player_esp"), &global->esp.name);
					ImGui::Checkbox(Xors("Draw Distance##player_esp"), &global->esp.distance);
					ImGui::Checkbox(Xors("Draw Bones##player_esp"), &global->esp.bones);
					ImGui::Checkbox(Xors("Draw Health Bar##player_esp"), &global->esp.health_bar);
					ImGui::Checkbox(Xors("Draw View Angles Bar##player_esp"), &global->esp.view_angles);
					ImGui::Checkbox(Xors("Draw Weapon##player_esp"), &global->esp.weapon);
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem(Xors("Misc")))
				{
					static float fov = 90.f;
					if (ImGui::SliderFloat("Field of View", &fov, 60, 120, "%.0f"))
					{
						auto controller = game::PlayerController;

						auto fov_manager = driver.read(controller + 0x8A8);
						
						if (fov_manager)
						{
							driver.write<float>(fov_manager + 0xA8, fov);
						}

					}


					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}

			ImGui::End();
		}
	}
}