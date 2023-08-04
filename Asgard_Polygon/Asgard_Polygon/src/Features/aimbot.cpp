#include "pch.h"
#include "aimbot.h"
#include <random>


FVector FindLookAtRotation(AActor target)
{
	FVector target_bone; //= target.bone.neckPos;

	switch (global->aimbot.target_bone)
	{
	case 0: target_bone = target.bone.headPos; break;
	case 1: target_bone = target.bone.neckPos; break;
	case 2: target_bone = target.bone.spine1Pos; break;
	case 3: target_bone = target.bone.spine2Pos; break;
	case 4: target_bone = target.bone.spine3Pos; break;
	case 5: target_bone = target.bone.lowerarmlPos; break;
	case 6: target_bone = target.bone.lowerarmrPos; break;
	}

	FVector direction = target_bone - game::CameraCachePrivate.Location;
	FVector lookAtRotation;

	lookAtRotation.X = -((acos(direction.Z / direction.length()) * (180.f / M_PI)) - 90);
	lookAtRotation.Y = std::atan2(direction.Y, direction.X) * (180.0f / M_PI);

	return lookAtRotation;
}

bool in_bounds(const FVector& point, const FVector2D& circleCenter, float circleRadius)
{
	FVector2D screen;
	project_world_to_screen(point, screen);

	float distance = std::sqrt((screen.x - circleCenter.x) * (screen.x - circleCenter.x) +
		(screen.y - circleCenter.y) * (screen.y - circleCenter.y));

	return distance <= circleRadius;
}

void GetNearestClient(AActor* target)
{
	float currentDistance = FLT_MAX;

	for (auto actor : Actors)
	{

		if (!bitset(actor.flags, actor_flags::ENEMY))
			continue;

		switch (global->aimbot.aimbot_targeting_type)
		{
		case TARGET_FOV:

			if (!in_bounds(actor.bone.neckPos, FVector2D(global->width / 2, global->height / 2), global->aimbot.fov_size))
				continue;

				break;
		case TARGET_VISIBLE:

			if (!bitset(actor.flags, actor_flags::VISIBLE))
				continue;

			break;
		case TARGET_FOV_VISIBLE:
			if (!in_bounds(actor.bone.neckPos, FVector2D(global->width / 2, global->height / 2), global->aimbot.fov_size))
				continue;
			if (!bitset(actor.flags, actor_flags::VISIBLE))
				continue;

			break;
		default:
			break;
		}

		auto location = actor.Location;
		float distance = location.distance(game::CameraCachePrivate.Location);

		if (distance < currentDistance)
		{
			*target = actor;
			currentDistance = distance;
		}
	}
}

bool randomBool() {
	std::srand(static_cast<unsigned int>(std::time(0)));
	int randomNum = std::rand() % 2;
	return static_cast<bool>(randomNum);
}

float randomFloat() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(-4.5, 4.5);
	return dist(gen);
}

ImVec2 SmoothAngle(FVector current_rotation, FVector new_rotation)
{
	float smoothing = (global->aimbot.smoothness.smoothing_speed / 100);
	auto should_random = randomBool();
	auto random_float = randomFloat();

	if (global->aimbot.smoothness.randomness)
		smoothing = should_random ? smoothing + random_float : smoothing - random_float;

	FVector VecDelta = current_rotation - new_rotation;
	VecDelta = VecDelta.normalize();

	VecDelta.x *= smoothing;
	VecDelta.y *= smoothing;
	auto To = current_rotation - VecDelta;

	return ImVec2(To.x, To.y);
}



void run_aimbot()
{
	AActor Target{};
	GetNearestClient(&Target);

	if (!Target.health)
		return;

	if (get_key_state(global->aimbot.hotkey, 0) || !global->aimbot.hotkey)
	{
		auto rotation = FindLookAtRotation(Target);

		auto re = global->aimbot.smoothness.smoothing ? SmoothAngle(game::CameraCachePrivate.Rotation, rotation) : ImVec2(rotation.x, rotation.y);
		driver.write<ImVec2>(game::LocalPlayerPawn + 0x674, re);
	}
}

void aimbot_thread()
{
	while (true)
	{
		if (global->aimbot.enabled)
			run_aimbot();
		
		Sleep(1);
	}
}
