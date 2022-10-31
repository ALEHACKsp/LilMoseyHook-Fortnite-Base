#pragma once
#include "overlay.h"

void draw_circle(int x, int y, int radius, ImColor color, int segments) {
	ImGui::GetOverlayDrawList()->AddCircle(ImVec2(x, y), radius, color, segments);
}

void draw_line(int x1, int y1, int x2, int y2, ImColor color, int thickness) {
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), color, thickness);
}

void draw_rect_fill(int x, int y, int w, int h, ImColor color) {
	ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color);
}

void draw_box_corner(int x, int y, int w, int h, int borderPx, ImColor color) {
	draw_rect_fill(x + borderPx, y, w / 3, borderPx, color); //top 
	draw_rect_fill(x + w - w / 3 + borderPx, y, w / 3, borderPx, color); //top 
	draw_rect_fill(x, y, borderPx, h / 3, color); //left 
	draw_rect_fill(x, y + h - h / 3 + borderPx * 2, borderPx, h / 3, color); //left 
	draw_rect_fill(x + borderPx, y + h + borderPx, w / 3, borderPx, color); //bottom 
	draw_rect_fill(x + w - w / 3 + borderPx, y + h + borderPx, w / 3, borderPx, color); //bottom 
	draw_rect_fill(x + w + borderPx, y, borderPx, h / 3, color);//right 
	draw_rect_fill(x + w + borderPx, y + h - h / 3 + borderPx * 2, borderPx, h / 3, color);//right 
}