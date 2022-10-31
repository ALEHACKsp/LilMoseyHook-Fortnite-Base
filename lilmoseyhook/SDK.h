#pragma once
#include "driver.h"
#include "offsets.h"
#include "vector.h"
#include "global.h"

namespace Pointer {
	inline DWORD_PTR uworld;
	inline DWORD_PTR game_instance;
	inline DWORD_PTR local_players;
	inline DWORD_PTR local_player;
	inline DWORD_PTR local_pawn;
	inline DWORD_PTR player_state;
	inline DWORD_PTR root_comp;
	inline DWORD_PTR relative_location;
	inline DWORD_PTR player_controller;
	inline DWORD_PTR persistentlevel;
	inline DWORD_PTR ulevel;
	inline Vector3 local_pawn_pos;

	inline int localplayerID;
	inline uintptr_t curactoridA;
	inline uint64_t current_actor;
	inline Vector3 camera;
	inline Vector3 localactorpos;
}

typedef struct actorEntity {
	uint64_t Actor;
	int ID;
	uint64_t mesh;
}actorEntity;
std::vector<actorEntity> entityList;

__forceinline FTransform GetBoneIndex(DWORD_PTR mesh, int index) {
	DWORD_PTR bonearray;
	bonearray = read<uintptr_t>(mesh + OFFSET_BONEARRAY); // 0x5B0
	if (bonearray == NULL)
		bonearray = read<uintptr_t>(mesh + OFFSET_BONEARRAY + 0x10);

	if (Debug::bone_index) {
		std::cout << E("Bone Index : ") << bonearray << std::endl;
	}

	return read<FTransform>(bonearray + (index * 0x60));
}

__forceinline Vector3 GetBoneWithRotation(DWORD_PTR mesh, int id) {
	FTransform bone = GetBoneIndex(mesh, id);
	FTransform ComponentToWorld = read<FTransform>(mesh + OFFSET_COMPTOWORLD); // old 0x1C0
	D3DMATRIX Matrix;

	Matrix = MatrixMultiplication(bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());

	if (Debug::bone_rotation) {
		std::cout << E("GBR Matrix : ") << Matrix._41 << E(",") << Matrix._42 << E(",") << Matrix._43 << E(",") << std::endl;
	}

	return Vector3(Matrix._41, Matrix._42, Matrix._43);
}

__forceinline bool isVisible(uint64_t mesh) {
	if (!mesh)
		return false;
	float fLastSubmitTime = read<float>(mesh + 0x330); //0x32c
	float fLastRenderTimeOnScreen = read<float>(mesh + 0x334); //0x324
	const float fVisionTick = 0.06f;
	bool bVisible = fLastRenderTimeOnScreen + fVisionTick >= fLastSubmitTime;
	return bVisible;
}

__forceinline Camera GetCamera(__int64 a1) {
	Camera LocalCamera;
	__int64 v1;
	__int64 v6;
	__int64 v7;
	__int64 v8;

	v1 = read<__int64>(Pointer::local_player + 0xd0);
	__int64 v9 = read<__int64>(v1 + 0x10);

	LocalCamera.FieldOfView = 80.f / (read<double>(v9 + 0x600) / 1.19f);
	LocalCamera.Rotation.x = read<double>(v9 + 0x7D0);
	LocalCamera.Rotation.y = read<double>(Pointer::root_comp + 0x148);

	uint64_t VSPointerLc = read<uint64_t>(Pointer::uworld + 0x100); //
	LocalCamera.Location = read<Vector3>(VSPointerLc);

	return LocalCamera;
}

__forceinline Vector3 ProjectWorldToScreen(Vector3 WorldLocation) {
	Camera vCamera = GetCamera(Pointer::root_comp);
	vCamera.Rotation.x = (asin(vCamera.Rotation.x)) * (180.0 / M_PI);

	D3DMATRIX tempMatrix = Matrix(vCamera.Rotation);

	Vector3 vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
	Vector3 vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
	Vector3 vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

	Vector3 vDelta = WorldLocation - vCamera.Location;
	Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

	if (Debug::camera) {
		std::cout << E("Field Of View : ") << vCamera.FieldOfView << std::endl;
		std::cout << E("Rotation : ") << vCamera.Rotation.x << E(" , ") << vCamera.Rotation.y << E(" , ") << vCamera.Rotation.z << std::endl;
		std::cout << E("Location : ") << vCamera.Location.x << E(" , ") << vCamera.Location.y << E(" , ") << vCamera.Location.z << std::endl;
	}

	if (vTransformed.z < 1.f)
		vTransformed.z = 1.f;

	return Vector3((1910 / 2.0f) + vTransformed.x * (((1910 / 2.0f) / tanf(vCamera.FieldOfView * (float)M_PI / 360.f))) / vTransformed.z, (1070 / 2.0f) - vTransformed.y * (((1070 / 2.0f) / tanf(vCamera.FieldOfView * (float)M_PI / 360.f))) / vTransformed.z, 0);
}