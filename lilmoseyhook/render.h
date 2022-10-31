#pragma once
#include "overlay.h"
#include "c_player.h"

void render_menu() {
	static int tab = 0;
	static auto flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar;

	//CREATE YOUR OWN MENU, I JUST MADE THIS RQ IDK IF IT EVEN WORKS LOOOOL

	ImGui::Begin((E("The GodFather - Private Editon")), NULL, flags);
	{
		ImGui::SetWindowSize(ImVec2(590, 415), ImGuiCond_Once);
		{
			ImGui::BeginChild((E("##Tabs")), ImVec2(-1, 40.0f));
			{
					if (ImGui::Button(((E("aimbot"))), ImVec2(185, 20))) {
						tab = 0;
					}ImGui::SameLine();
					if (ImGui::Button(((E("visuals"))), ImVec2(185, 20))) {
						tab = 1;
					}
					ImGui::SameLine();
					if (ImGui::Button(((E("extra"))), ImVec2(185, 20))) {
						tab = 2;
					}
					ImGui::SameLine();
			}
			ImGui::EndChild();
		}

		switch (tab) {
		case 0: //Aimbot
			break;
		case 1: //Visuals
			ImGui::Checkbox(E("Enable"), &Config::visuals::enable);
			ImGui::Checkbox(E("Snaplies"), &Config::visuals::snapline);
			break;
		case 2: //Extra
			break;
		}
	}
	ImGui::End();
}

void render() {
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	player_loop();

	shortcurts();
	if (Config::menu::show) {
		render_menu();
		ImGui::GetIO().MouseDrawCursor = 1;
	}
	else {
		ImGui::GetIO().MouseDrawCursor = 0;
	}

	ImGui::EndFrame();
	Global::directx::device->SetRenderState(D3DRS_ZENABLE, false);
	Global::directx::device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	Global::directx::device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
	Global::directx::device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	if (Global::directx::device->BeginScene() >= 0) {
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		Global::directx::device->EndScene();
	}
	HRESULT result = Global::directx::device->Present(NULL, NULL, NULL, NULL);

	if (result == D3DERR_DEVICELOST && Global::directx::device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
		Global::directx::device->Reset(&Global::directx::params);
		ImGui_ImplDX9_CreateDeviceObjects();
	}
}