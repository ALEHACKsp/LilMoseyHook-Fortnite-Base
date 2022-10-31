#pragma once
#include "SDK.h"

__forceinline void setup_world() {
	while (true) {
		std::vector<actorEntity> entList;
		//std::vector<LootEntity> lootList;

		Pointer::uworld = read<DWORD_PTR>(Global::process::base + OFFSET_UWORLD); //+
		Pointer::game_instance = read<DWORD_PTR>(Pointer::uworld + OFFSET_GAME_INSTANCE); //+
		Pointer::persistentlevel = read<DWORD_PTR>(Pointer::uworld + OFFSET_PERSISTENTLEVEL);
		Pointer::local_players = read<DWORD_PTR>(Pointer::game_instance + OFFSET_LOCALPLAYERS);
		Pointer::local_player = read<DWORD_PTR>(Pointer::local_players);
		Pointer::player_controller = read<DWORD_PTR>(Pointer::local_player + OFFSET_PLAYERCONTROLLER); //+
		Pointer::local_pawn = read<DWORD_PTR>(Pointer::player_controller + OFFSET_PAWN); //+
		Pointer::player_state = read<DWORD_PTR>(Pointer::local_pawn + OFFSET_PLAYERSTATE);
		Pointer::root_comp = read<DWORD_PTR>(Pointer::local_pawn + OFFSET_ROOTCOMP); //+

		if (Debug::world) {
			std::cout << E("Uworld : ") << Pointer::uworld << std::endl;
			std::cout << E("game_instance : ") << Pointer::game_instance << std::endl;
			std::cout << E("persistentlevel : ") << Pointer::persistentlevel << std::endl;
			std::cout << E("local_players : ") << Pointer::local_players << std::endl;
			std::cout << E("local_player : ") << Pointer::local_players << std::endl;
			std::cout << E("player_controller : ") << Pointer::player_controller << std::endl;
			std::cout << E("local_pawn : ") << Pointer::local_pawn << std::endl;
			std::cout << E("player_state : ") << Pointer::player_state << std::endl;
			std::cout << E("root_comp : ") << Pointer::root_comp << std::endl;
		}

		DWORD ActorCount = read<DWORD>(Pointer::persistentlevel + 0xA0);
		DWORD_PTR AActors = read<DWORD_PTR>(Pointer::persistentlevel + 0x98);

		for (int i = 0; i < ActorCount; ++i) {
			DWORD_PTR CurrentActor = read<DWORD_PTR>(AActors + i * 0x8);
			uint64_t CurrentActorMesh = read<uint64_t>(CurrentActor + OFFSET_MESH);
			int curactorid = read<int>(CurrentActor + 0x18);
			DWORD64 actorPlayerState = read<DWORD64>(CurrentActor + OFFSET_PLAYERSTATE);
			int MyTeamId = read<int>(Pointer::player_state + OFFSET_TEAMINDEX);
			int ActorTeamId = read<int>(actorPlayerState + OFFSET_TEAMINDEX);
			if (MyTeamId == ActorTeamId) continue;

			float player_check = read<float>(CurrentActor + OFFSET_REVIVE_DBNO_TIME);
			if (player_check == 10) {
				actorEntity fnlEntity{ };

				fnlEntity.Actor = CurrentActor;
				fnlEntity.mesh = CurrentActorMesh;
				fnlEntity.ID = curactorid;

				entList.push_back(fnlEntity);
			}
		}
		entityList = entList;
		Sleep(1);
	}
}