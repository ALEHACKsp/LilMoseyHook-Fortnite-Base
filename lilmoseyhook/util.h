#pragma once
#include <Windows.h>
#include <cstdint>
#include <utility>

MSG Message = { NULL };

static HWND get_process_wnd(uint32_t pid) {
	std::pair<HWND, uint32_t> params = { 0, pid };

	BOOL bResult = EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
		auto pParams = (std::pair<HWND, uint32_t>*)(lParam);
		uint32_t processId = 0;

		if (GetWindowThreadProcessId(hwnd, reinterpret_cast<LPDWORD>(&processId)) && processId == pParams->second) {
			SetLastError((uint32_t)-1);
			pParams->first = hwnd;
			return FALSE;
		}

		return TRUE;

		}, (LPARAM)&params);

	if (!bResult && GetLastError() == -1 && params.first)
		return params.first;

	return NULL;
}

class CKey {
private:
	BYTE bPrevState[0x100];
public:
	CKey() {
		memset(bPrevState, 0, sizeof(bPrevState));
	}

	BOOL IsKeyPushing(BYTE vKey) {
		return GetAsyncKeyState(vKey) & 0x8000;
	}

	BOOL IsKeyPushed(BYTE vKey) {
		BOOL bReturn = FALSE;

		if (IsKeyPushing(vKey))
			bPrevState[vKey] = TRUE;
		else
		{
			if (bPrevState[vKey] == TRUE)
				bReturn = TRUE;
			bPrevState[vKey] = FALSE;
		}

		return bReturn;
	}
};
CKey Key;

void shortcurts() {
	if (Key.IsKeyPushing(VK_INSERT)) {
		if (Config::menu::show == false) {
			Config::menu::show = true;
		}
		else if (Config::menu::show == true) {
			Config::menu::show = false;
		}
		Sleep(200);
	}
}