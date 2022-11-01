#include <Windows.h>
#include <iostream>
#include <thread>

#include "xor.h"
#include "global.h"
#include "driver.h"
#include "overlay.h"
#include "render.h"

#include "c_world.h"

WPARAM main_loop();

int main() {
	std::cout << E("[=] Initialising Driver") << std::endl;
	if (driver->Init(FALSE)) {
		std::cout << E("[+] Completed") << std::endl;
		if (!driver->Attach(E(L"FortniteClient-Win64-Shipping.exe"))) {
			std::cout << E("[-] Failed to attach driver") << std::endl;
			exit(0);
		}

		Global::process::id = driver->GetProcessId(E(L"FortniteClient-Win64-Shipping.exe"));
		Global::process::base = driver->GetModuleBase(E(L"FortniteClient-Win64-Shipping.exe"));

		if (Debug::process) {
			std::cout << E("[DEBUG] Process ID : ") << Global::process::id << std::endl;
			std::cout << E("[DEBUG] Process Base : ") << Global::process::base << std::endl;
		}

		std::cout << E("[=] Setting up window") << std::endl;

		setup_window();
		init_directx(Global::overlay::window);

		std::cout << E("[=] Creating Threads") << std::endl;

		std::thread(setup_world).detach();

		while (true) {
			main_loop();
		}
	}
		std::cout << E("[-] Driver Failed, did you map it correctly?") << std::endl;
		system(E("pause"));
		exit(0);
}

WPARAM main_loop()
{
	static RECT old_rc;
	ZeroMemory(&Message, sizeof(MSG));

	while (Message.message != WM_QUIT)
	{
		if (PeekMessage(&Message, Global::overlay::window, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		HWND hwnd_active = GetForegroundWindow();
		if (GetAsyncKeyState(0x23) & 1)
			exit(8);

		if (hwnd_active == Global::process::window) {
			HWND hwndtest = GetWindow(hwnd_active, GW_HWNDPREV);
			SetWindowPos(Global::overlay::window, hwndtest, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}
		RECT rc;
		POINT xy;

		ZeroMemory(&rc, sizeof(RECT));
		ZeroMemory(&xy, sizeof(POINT));
		GetClientRect(Global::process::window, &rc);
		ClientToScreen(Global::process::window, &xy);
		rc.left = xy.x;
		rc.top = xy.y;

		ImGuiIO& io = ImGui::GetIO();
		io.ImeWindowHandle = Global::process::window;
		io.DeltaTime = 1.0f / 60.0f;

		POINT p;
		GetCursorPos(&p);
		io.MousePos.x = p.x - xy.x;
		io.MousePos.y = p.y - xy.y;

		if (GetAsyncKeyState(0x1)) {
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;
			io.MouseClickedPos[0].x = io.MousePos.x;
			io.MouseClickedPos[0].x = io.MousePos.y;
		}
		else
			io.MouseDown[0] = false;

		if (rc.left != old_rc.left || rc.right != old_rc.right || rc.top != old_rc.top || rc.bottom != old_rc.bottom) {

			old_rc = rc;

			Global::process::width = rc.right;
			Global::process::height = rc.bottom;

			Global::directx::params.BackBufferWidth = Global::process::width;
			Global::directx::params.BackBufferHeight = Global::process::height;
			SetWindowPos(Global::overlay::window, (HWND)0, xy.x, xy.y, Global::process::width, Global::process::height, SWP_NOREDRAW);
			Global::directx::device->Reset(&Global::directx::params);
		}
		render();
	}
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	clean_directx();
	DestroyWindow(Global::overlay::window);

	return Message.wParam;
}
