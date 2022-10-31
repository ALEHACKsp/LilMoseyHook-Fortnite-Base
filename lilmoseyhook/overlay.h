#pragma once
#include <Windows.h>
#include <Uxtheme.h>
#include <dwmapi.h>

#include "util.h"
#include "global.h"
#include "xor.h"

#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "d3d9.lib")

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"

void set_win_to_target() {
	while (true) {
		Global::process::window = get_process_wnd(Global::process::id);
		if (Global::process::window) {
			ZeroMemory(&Global::process::rect, sizeof(Global::process::rect));
			GetWindowRect(Global::process::window, &Global::process::rect);
			Global::process::width = Global::process::rect.right - Global::process::rect.left;
			Global::process::height = Global::process::rect.bottom - Global::process::rect.top;
			DWORD dwStyle = GetWindowLong(Global::process::window, GWL_STYLE);
			if (dwStyle & WS_BORDER)
			{
				Global::process::rect.top += 32;
				Global::process::height -= 39;
			}
			Global::overlay::screenc_x = Global::process::width / 2;
			Global::overlay::screenc_y = Global::process::height / 2;
			MoveWindow(Global::overlay::window, Global::process::rect.left, Global::process::rect.top, Global::process::width, Global::process::height, true);
		}
	}
}

void setup_window() {
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)set_win_to_target, 0, 0, 0);

	WNDCLASSEXA wcex = {
		sizeof(WNDCLASSEXA),
		0,
		DefWindowProcA,
		0,
		0,
		nullptr,
		LoadIcon(nullptr, IDI_APPLICATION),
		LoadCursor(nullptr, IDC_ARROW),
		nullptr,
		nullptr,
		("X"),
		LoadIcon(nullptr, IDI_APPLICATION)
	};

	RECT Rect;
	GetWindowRect(GetDesktopWindow(), &Rect);

	RegisterClassExA(&wcex);

	Global::overlay::window = CreateWindowExA(NULL, E("X"), E("X"), WS_POPUP, Rect.left, Rect.top, Rect.right, Rect.bottom, NULL, NULL, wcex.hInstance, NULL);
	SetWindowLong(Global::overlay::window, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
	SetLayeredWindowAttributes(Global::overlay::window, RGB(0, 0, 0), 255, LWA_ALPHA);
	//SetWindowDisplayAffinity(MyWnd, 1);

	MARGINS margin = { -1 };
	DwmExtendFrameIntoClientArea(Global::overlay::window, &margin);

	ShowWindow(Global::overlay::window, SW_SHOW);
	UpdateWindow(Global::overlay::window);
}

HRESULT init_directx(HWND hWnd)
{
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &Global::directx::object)))
		exit(3);

	ZeroMemory(&Global::directx::params, sizeof(Global::directx::params));
	Global::directx::params.Windowed = TRUE;
	Global::directx::params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	Global::directx::params.hDeviceWindow = hWnd;
	Global::directx::params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	Global::directx::params.BackBufferFormat = D3DFMT_A8R8G8B8;
	Global::directx::params.BackBufferWidth = Global::process::width;
	Global::directx::params.BackBufferHeight = Global::process::height;
	Global::directx::params.EnableAutoDepthStencil = TRUE;
	Global::directx::params.AutoDepthStencilFormat = D3DFMT_D16;
	Global::directx::params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(Global::directx::object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &Global::directx::params, 0, &Global::directx::device)))
	{
		Global::directx::object->Release();
		exit(4);
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	Global::directx::object->Release();
	return S_OK;
}

void clean_directx() {
	if (Global::directx::device != NULL) {
		Global::directx::device->EndScene();
		Global::directx::device->Release();
	}
	if (Global::directx::object != NULL) {
		Global::directx::object->Release();
	}
}