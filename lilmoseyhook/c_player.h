#pragma once
#include "draw.h"
#include "SDK.h"

__forceinline void player_loop() {
	auto entlist_copy = entityList;

	float closest_distance = FLT_MAX;
	DWORD_PTR closest_pawn = NULL;

	for (int i = 0; i < entlist_copy.size(); ++i) {
		actorEntity entity = entlist_copy.at(i);
		uint64_t mesh = entity.mesh;
		Vector3 head_bone = GetBoneWithRotation(mesh, 66);
		Vector3 head_w2s = ProjectWorldToScreen(head_bone);
		Vector3 base_bone = GetBoneWithRotation(mesh, 0);
		Vector3 base_w2s = ProjectWorldToScreen(base_bone);

		if (Debug::entity_bone_locations) {
			std::cout << E("Head Bone Rotation : ") << head_bone.x << E(" , ") << head_bone.y << E(" , ") << head_bone.z << std::endl;
			std::cout << E("Base Bone Rotation : ") << base_bone.x << E(" , ") << base_bone.y << E(" ,") << base_bone.z << std::endl;
			std::cout << E("Head Bone W2S : ") << head_w2s.x << E(" , ") << head_w2s.y << E(" , ") << head_w2s.z << std::endl;
			std::cout << E("Base Bone W2S : ") << base_w2s.x << E(" , ") << base_w2s.y << E(" , ") << base_w2s.z << std::endl;
		}

		if (Config::visuals::enable) {
			ImColor col_snapline;
			if (Config::visuals::visible && isVisible(mesh)) {
				col_snapline = ImColor(Config::visuals::snapline_color_vis[0], Config::visuals::snapline_color_vis[1], Config::visuals::snapline_color_vis[2]);
			}
			else {
				col_snapline = ImColor(Config::visuals::snapline_color[0], Config::visuals::snapline_color[1], Config::visuals::snapline_color[2]);
			}

			if (Config::visuals::snapline) {
				draw_line(1920 / 2, 1080, base_bone.x, base_bone.y, col_snapline, Config::visuals::thick_snapline);
			}
		}
	}
}