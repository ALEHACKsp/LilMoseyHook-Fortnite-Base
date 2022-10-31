#pragma once
#include <Windows.h>
#include <d3d9.h>

#include "vector.h"

namespace Global {
	namespace process {
		DWORD id;
		DWORD64 base;
		HWND window;
		RECT rect = { NULL };
		int width = GetSystemMetrics(SM_CXSCREEN);
		int height = GetSystemMetrics(SM_CYSCREEN);
	}
	namespace overlay {
		HWND window;
		DWORD screenc_x;
		DWORD screenc_y;
	}
	namespace directx {
		IDirect3D9Ex* object;
		IDirect3DDevice9Ex* device;
		D3DPRESENT_PARAMETERS params;
	}
}

namespace Config {
	namespace aim {

	}

	namespace visuals {
		bool enable = false;
		bool visible = false;
		bool snapline = false;

		//Customise Visuals
		float thick_snapline = 2.f;
		
		//Config Player - Color Visible
		float snapline_color_vis[3] = { 15 / 255.f, 235 / 255.f, 255 / 255.f };

		//Config Player - Color Invisible
		float snapline_color[3] = { 147 / 255.f, 15 / 255.f, 255 / 255.f };
	}

	namespace misc {

	}

	namespace menu {
		bool show = true;
	}
}

namespace Debug {
	bool process = false;
	bool bone_index = false;
	bool bone_rotation = false;
	bool camera = false;
	bool world = false;
	bool entity_bone_locations = false;
}

struct Camera
{
	float FieldOfView;
	Vector3 Rotation;
	Vector3 Location;
};