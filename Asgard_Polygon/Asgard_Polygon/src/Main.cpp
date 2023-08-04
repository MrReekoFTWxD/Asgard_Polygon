// External_Template.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

int main()
{
	global = new GlobalVariables();

	printf("Waiting for POLYGON-Win64-Shipping...\n");
    while (global->dx9.game_hwnd == 0)
    {
        Sleep(1);

        driver.process_id = driver.get_process_id(L"POLYGON-Win64-Shipping.exe");
        global->dx9.game_hwnd = get_process_wnd(driver.process_id);

        Sleep(1);
    }

    if (!driver.init())
    {
        printf("Driver Failed\n");
        Sleep(3000);
        exit(0);
    }
    if (!band::init())
    {
        printf("Band Failed\n");
        Sleep(3000);
        exit(0);
    }

    global->base = driver.get_base_address();
    printf(Xors("Base: %llX\n"), global->base);

    DirectX::CreateOverlay();
    DirectX::Init();

    std::thread engine(engine_thread);
    std::thread aimbot(aimbot_thread);
    engine.detach();
    aimbot.detach();

    gui::instance();
    exit(0);
}
