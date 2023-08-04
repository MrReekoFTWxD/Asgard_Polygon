#include "pch.h"
#include "engine_thread.h"

enum Bone
{
	Root = 0,
	pelvis = 8,
	spine_01 = 11,
	spine_02 = 10,
	spine_03 = 9,
	neck_01 = 62,
	Head = 63,

	upperarm_l = 13,
	lowerarm_l = 14,
	hand_l = 15,

	upperarm_r = 38,
	lowerarm_r = 39,
	hand_r = 40,

	upperleg_l = 64,
	lowerleg_l = 65,
	foot_l = 67,

	upperleg_r = 70,
	lowerleg_r = 71,
	foot_r = 73,
};

D3DMATRIX MatrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2)
{
	D3DMATRIX pOut;
	pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
	pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
	pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
	pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
	pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
	pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
	pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
	pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
	pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
	pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
	pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
	pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
	pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
	pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
	pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
	pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;
	return pOut;
}

FVector get_entity_bone(uintptr_t mesh, int bone_id)
{
	auto bone_array = driver.read<uint64_t>(mesh + 0x600);
	auto bone_array_size = driver.read<uint64_t>(mesh + 0x600 + 0x8);
	if (bone_array_size == 0)
		bone_array = driver.read<uint64_t>(mesh + 0x600 + 0x10);

	FTransform bone;
	driver.readm((void*)(bone_array + (bone_id * 0x60)), &bone, sizeof(FTransform));

	FTransform component_to_world = driver.read<FTransform>(mesh + 0x240);
	D3DMATRIX matrix = MatrixMultiplication(bone.ToMatrixWithScale(), component_to_world.ToMatrixWithScale());
	return FVector(matrix._41, matrix._42, matrix._43);
}

bool isVisible(DWORD_PTR mesh)
{
	float tolerance = .06f;
	float LastRenderTime = driver.read<float>(mesh + 0x35C);
	float LastSubmitTime = driver.read<float>(mesh + 0x358);

	return (LastRenderTime + tolerance) > LastSubmitTime;
}


void engine_thread()
{
	while (true)
	{
		game::GWorld = driver.read<uint64_t>(global->base + 0x7FE3C50);

		if (!game::GWorld)
		{
			Actors.clear();
			continue;
		}

		game::PersistentLevel = driver.read(game::GWorld + 0x30);

		if (!game::PersistentLevel)
		{
			Actors.clear();
			continue;
		}

		game::OwningGameInstance = driver.read(game::GWorld + 0x1B8);

		if (!game::OwningGameInstance)
		{
			Actors.clear();
			continue;
		}

		auto localPlayers = driver.read<TArray<uint64_t>>(game::OwningGameInstance + 0x38);
		game::LocalPlayer = localPlayers[0];

		if (!game::LocalPlayer)
		{
			Actors.clear();
			continue;
		}

		game::PlayerController = driver.read(game::LocalPlayer + 0x30);

		if (!game::PlayerController)
		{
			Actors.clear();
			continue;
		}

		game::LocalPlayerPawn = driver.read(game::PlayerController + 0x338);

		if (!game::LocalPlayerPawn)
		{
			Actors.clear();
			continue;
		}

		game::LocalPlayerState = driver.read(game::LocalPlayerPawn + 0x2B0);

		if (!game::LocalPlayerState)
		{
			Actors.clear();
			continue;
		}

		if (global->aimbot.nospread)
		{
			auto inventory = driver.read(game::LocalPlayerState + 0x430);
			auto weapon = driver.read(inventory + 0xF8);

			driver.write<float>(weapon + 0x6C4, 0.0000000001f); //Setting 0 breaks things
		}


		if (global->aimbot.norecoil)
		{
			auto inventory = driver.read(game::LocalPlayerState + 0x430);
			auto weapon = driver.read(inventory + 0xF8);

			driver.write<float>(weapon + 0x3F4, 0.0000000001f); //Setting 0 breaks things
			driver.write<float>(weapon + 0x3F8, 0.0000000001f);
			driver.write<float>(weapon + 0x3FC, 0.0000000001f);
			driver.write<float>(weapon + 0x400, 0.0000000001f);
			driver.write<float>(weapon + 0x404, 0.0000000001f);
			driver.write<float>(weapon + 0x408, 0.0000000001f);
		}

		game::PlayerCameraManager = driver.read(game::PlayerController + 0x348);

		if (!game::PlayerCameraManager)
		{
			Actors.clear();
			continue;
		}

		game::LocalPlayerTeam = driver.read<uint8_t>(game::LocalPlayerState + 0x418);

		game::CameraCachePrivate = driver.read<FMinimalViewInfo>(game::PlayerCameraManager + 0x22B0 + 0x10);

		if (!NamePoolData)
		{
			NamePoolData = new FNamePool();
			driver.readm((void*)(global->base + 0x7DE4500), NamePoolData, sizeof(FNamePool));
		}

		TArray actor_array = driver.read<TArray<uint64_t>>(game::PersistentLevel + 0xA8);

		game::ActorArray = actor_array.Data;
		game::ActorCount = actor_array.Num();

		std::vector<AActor> temp_Array;
		for (auto index{ 0 }; index < actor_array.Num(); index++)
		{
			auto actor = actor_array[index];

			if (!actor)
				continue;
			if (actor == game::LocalPlayerPawn)
				continue;

			auto filter_name = driver.read<FName>(actor + 0x18);
			auto bp_name = NamePoolData->GetName(filter_name.Index);

			if (!strstr(bp_name.data(), "BP_PG_Game_Character"))
				continue;

			auto RootComponent = driver.read(actor + 0x198);
			if (!RootComponent)
				continue;

			auto playerstate = driver.read(actor + 0x2B0);
			if (!playerstate)
				continue;

			auto fstring_name = FString(playerstate + 0x388);
			auto team = driver.read<uint8_t>(playerstate + 0x418);

			auto health_comp = driver.read(actor + 0x730);
			if (!health_comp)
				continue;
			int health = driver.read<char>(health_comp + 0xD0);

			if (!(health > 0))
				continue;

			auto mesh = driver.read(actor + 0x318);
			if (!mesh)
				continue;

			auto location = get_entity_bone(mesh, Root);
			auto rotation = driver.read<FVector>(RootComponent + 0x140);
			int flag = NONE;

			if (isVisible(mesh))
				flag |= actor_flags::VISIBLE;
			if (team != game::LocalPlayerTeam)
				flag |= actor_flags::ENEMY;

			auto inventory = driver.read(playerstate + 0x430);
			auto weapon = driver.read(inventory + 0xF8);

			auto grenade_count = driver.read<uint8_t>(inventory + 0x100);
			auto gun_type = driver.read<uint8_t>(weapon + 0x395);


			//Body
			auto headPos = get_entity_bone(mesh, Head);
			auto neckPos = get_entity_bone(mesh, neck_01);
			auto spine1Pos = get_entity_bone(mesh, spine_01);
			auto spine2Pos = get_entity_bone(mesh, spine_02);
			auto spine3Pos = get_entity_bone(mesh, spine_03);
			auto pelvisPos = get_entity_bone(mesh, pelvis);

			//Arms
			auto upperarmlPos = get_entity_bone(mesh, upperarm_l);
			auto lowerarmlPos = get_entity_bone(mesh, lowerarm_l);
			auto handlPos = get_entity_bone(mesh, hand_l);
			auto upperarmrPos = get_entity_bone(mesh, upperarm_r);
			auto lowerarmrPos = get_entity_bone(mesh, lowerarm_r);
			auto handrPos = get_entity_bone(mesh, hand_r);

			//Legs
			auto upperleglPos = get_entity_bone(mesh, upperleg_l);
			auto lowerleglPos = get_entity_bone(mesh, lowerleg_l);
			auto footlPos = get_entity_bone(mesh, foot_l);
			auto upperlegrPos = get_entity_bone(mesh, upperleg_r);
			auto lowerlegrPos = get_entity_bone(mesh, lowerleg_r);
			auto footrPos = get_entity_bone(mesh, foot_r);

			AActor temp;

			temp.Location = location;
			temp.Rotation = rotation;
			temp.Name = fstring_name.ToString();
			temp.health = health;

			temp.gun_type = gun_type;
			temp.grenade_count = grenade_count;

			temp.bone.headPos = headPos;
			temp.bone.neckPos = neckPos;
			temp.bone.spine1Pos = spine1Pos;
			temp.bone.spine2Pos = spine2Pos;
			temp.bone.spine3Pos = spine3Pos;
			temp.bone.pelvisPos = pelvisPos;

			temp.bone.upperarmlPos = upperarmlPos;
			temp.bone.lowerarmlPos = lowerarmlPos;
			temp.bone.handlPos = handlPos;
			temp.bone.upperarmrPos = upperarmrPos;
			temp.bone.lowerarmrPos = lowerarmrPos;
			temp.bone.handrPos = handrPos;

			temp.bone.upperleglPos = upperleglPos;
			temp.bone.lowerleglPos = lowerleglPos;
			temp.bone.footlPos = footlPos;
			temp.bone.upperlegrPos = upperlegrPos;
			temp.bone.lowerlegrPos = lowerlegrPos;
			temp.bone.footrPos = footrPos;

			temp.flags = flag;

			temp_Array.push_back(temp);

		}
		Actors = temp_Array;

	}
}