#include "pch.h"
#include "visuals.h"


D3DMATRIX to_matrix(FVector rot, FVector origin = FVector(0, 0, 0))
{
	float radpitch = (rot.x * M_PI / 180);
	float radyaw = (rot.y * M_PI / 180);
	float radroll = (rot.z * M_PI / 180);
	float sp = sinf(radpitch);
	float cp = cosf(radpitch);
	float sy = sinf(radyaw);
	float cy = cosf(radyaw);
	float sr = sinf(radroll);
	float cr = cosf(radroll);
	D3DMATRIX matrix{};
	matrix.m[0][0] = cp * cy;
	matrix.m[0][1] = cp * sy;
	matrix.m[0][2] = sp;
	matrix.m[0][3] = 0.f;
	matrix.m[1][0] = sr * sp * cy - cr * sy;
	matrix.m[1][1] = sr * sp * sy + cr * cy;
	matrix.m[1][2] = -sr * cp;
	matrix.m[1][3] = 0.f;
	matrix.m[2][0] = -(cr * sp * cy + sr * sy);
	matrix.m[2][1] = cy * sr - cr * sp * sy;
	matrix.m[2][2] = cr * cp;
	matrix.m[2][3] = 0.f;
	matrix.m[3][0] = origin.x;
	matrix.m[3][1] = origin.y;
	matrix.m[3][2] = origin.z;
	matrix.m[3][3] = 1.f;
	return matrix;
}

bool project_world_to_screen(FVector world_location, FVector2D& screen)
{
	D3DMATRIX temp_matrix = to_matrix(game::CameraCachePrivate.Rotation);
	FVector vaxisx = FVector(temp_matrix.m[0][0], temp_matrix.m[0][1], temp_matrix.m[0][2]);
	FVector vaxisy = FVector(temp_matrix.m[1][0], temp_matrix.m[1][1], temp_matrix.m[1][2]);
	FVector vaxisz = FVector(temp_matrix.m[2][0], temp_matrix.m[2][1], temp_matrix.m[2][2]);

	FVector vdelta = world_location - game::CameraCachePrivate.Location;
	FVector vtransformed = FVector(vdelta.DotProduct(vaxisy), vdelta.DotProduct(vaxisz), vdelta.DotProduct(vaxisx));

	if (vtransformed.z < 0.098f)
		return false;

	auto screen_center_x = global->width / 2;
	auto screen_center_y = global->height / 2;

	screen.x = screen_center_x + vtransformed.x * ((screen_center_x / tanf(game::CameraCachePrivate.FOV * M_PI / 360))) / vtransformed.z;
	screen.y = screen_center_y - vtransformed.y * ((screen_center_x / tanf(game::CameraCachePrivate.FOV * M_PI / 360))) / vtransformed.z;

	return true;
}



void draw_bone(FVector bone, FVector bone2, ImColor color)
{
	FVector2D bone_pos[2];
	if (!project_world_to_screen(bone, bone_pos[0]))
		return;
	if (!project_world_to_screen(bone2, bone_pos[1]))
		return;

	game_render_line(bone_pos[0], bone_pos[1], color, 1);
}

void boundingBox(AActor actor, ImColor color, bool rotation = true)
{
	static FVector boxRescaling = { 55.f, 5.f, 5.f };

	auto WHeadPos = actor.bone.headPos;
	auto WFeetPos = actor.Location;
	WHeadPos.Z += 25.f;
	WFeetPos.Z -= 5.f;

	FVector b1, b2, b3, b4, t1, t2, t3, t4;
	FVector2D b1Out, b2Out, b3Out, b4Out, t1Out, t2Out, t3Out, t4Out;

	b1.Z = b2.Z = b3.Z = b4.Z = WFeetPos.Z - boxRescaling.Z;
	float yaw = rotation ? actor.Rotation.y : 0;

	b1.X = WFeetPos.X + cosf(TORAD(yaw + 45.f)) * boxRescaling.X;
	b1.Y = WFeetPos.Y + sinf(TORAD(yaw + 45.f)) * boxRescaling.X;

	b2.X = WFeetPos.X + cosf(TORAD(yaw + 135.f)) * boxRescaling.X;
	b2.Y = WFeetPos.Y + sinf(TORAD(yaw + 135.f)) * boxRescaling.X;

	b3.X = WFeetPos.X + cosf(TORAD(yaw + 225.f)) * boxRescaling.X;
	b3.Y = WFeetPos.Y + sinf(TORAD(yaw + 225.f)) * boxRescaling.X;

	b4.X = WFeetPos.X + cosf(TORAD(yaw + 315.f)) * boxRescaling.X;
	b4.Y = WFeetPos.Y + sinf(TORAD(yaw + 315.f)) * boxRescaling.X;

	t1 = b1, t2 = b2, t3 = b3, t4 = b4;
	t1.Z = t2.Z = t3.Z = t4.Z = WHeadPos.Z + boxRescaling.Y;

	bool a = project_world_to_screen(b1, b1Out);
	bool b = project_world_to_screen(b2, b2Out);
	bool c = project_world_to_screen(b3, b3Out);
	bool d = project_world_to_screen(b4, b4Out);

	bool e = project_world_to_screen(t1, t1Out);
	bool f = project_world_to_screen(t2, t2Out);
	bool g = project_world_to_screen(t3, t3Out);
	bool h = project_world_to_screen(t4, t4Out);

	if (!a)
		return;
	if (!b)
		return;
	if (!c)
		return;
	if (!d)
		return;
	if (!e)
		return;
	if (!f)
		return;
	if (!g)
		return;
	if (!h)
		return;


	game_render_line(b1Out, b2Out, color, 1);
	game_render_line(b2Out, b3Out, color, 1);
	game_render_line(b3Out, b4Out, color, 1);
	game_render_line(b4Out, b1Out, color, 1);

	game_render_line(b1Out, t1Out, color, 1);
	game_render_line(b2Out, t2Out, color, 1);
	game_render_line(b3Out, t3Out, color, 1);
	game_render_line(b4Out, t4Out, color, 1);

	game_render_line(t1Out, t2Out, color, 1);
	game_render_line(t2Out, t3Out, color, 1);
	game_render_line(t3Out, t4Out, color, 1);
	game_render_line(t4Out, t1Out, color, 1);

}

void draw_corners(int x, int y, int w, int h, ImColor color)
{
	float lineW = (w / 5.0f);
	float lineH = (h / 6.0f);
	float lineT = 1.0f;

	game_render_line(x, y, x, y + lineH, color, 1.0f); // top left
	game_render_line(x, y, x + lineW + 1, y, color, 1.0f);
	game_render_line(x + w - lineW - 2, y, x + w, y, color, 1.0f); // top right
	game_render_line(x + w, y, x + w, y + lineH, color, 1.0f);
	game_render_line(x, y + h - lineH, x, y + h, color, 1.0f); // bottom left
	game_render_line(x - 1, y + h, x + lineW + 1, y + h, color, 1.0f);
	game_render_line(x + w - lineW - 2, y + h, x + w, y + h, color, 1.0f); // bottom right
	game_render_line(x + w, y + h - lineH, x + w, y + h, color, 1.0f);
}


void player_esp_box(AActor actor, ImColor color)
{
	if (color.Value.w == 0)
		return;

	ImVec4 filledColor = ImVec4(color.Value.x, color.Value.y, color.Value.z, 0.08);

	FVector2D center, max, min;
	auto headOrigin = actor.bone.headPos;
	auto rootOrigin = actor.Location;

	headOrigin.Z += 25.f;
	rootOrigin.Z -= 5;

	if (!project_world_to_screen(headOrigin, max))
		return;
	if (!project_world_to_screen(rootOrigin, min))
		return;
	if (!project_world_to_screen(rootOrigin, center))
		return;

	float flWidth = fabs((max.Y - min.Y) / 4.f);

	switch (global->esp.boxType)
	{
	case 1: game_render_icon(center.X - flWidth - 6.f, max.Y - 4.f, flWidth * 2.f + 12.f, min.Y - max.Y + 8.f, color, RFOutline); break;
	case 2: boundingBox(actor, color, false); break;
	case 3: boundingBox(actor, color); break;
	case 4: draw_corners(center.X - flWidth - 6.f, max.Y - 4.f, flWidth * 2.f + 12.f, min.Y - max.Y + 8.f, color); break;
	case 5:
		game_render_icon(center.X - flWidth - 6.f, max.Y - 4.f, flWidth * 2.f + 12.f, min.Y - max.Y + 8.f, filledColor);
		game_render_icon(center.X - flWidth - 6.f, max.Y - 4.f, flWidth * 2.f + 12.f, min.Y - max.Y + 8.f, color, RFOutline);
		break;
	}
}

void player_esp_bones(AActor actor, ImColor esp_color)
{
	if (!global->esp.bones)
		return;

	//Head -> Pelvis
	draw_bone(actor.bone.headPos, actor.bone.neckPos, esp_color);
	draw_bone(actor.bone.spine1Pos, actor.bone.neckPos, esp_color);
	draw_bone(actor.bone.spine1Pos, actor.bone.spine2Pos, esp_color);
	draw_bone(actor.bone.spine3Pos, actor.bone.spine2Pos, esp_color);
	draw_bone(actor.bone.spine3Pos, actor.bone.pelvisPos, esp_color);

	//Neck -> left hand
	draw_bone(actor.bone.neckPos, actor.bone.upperarmlPos, esp_color);
	draw_bone(actor.bone.lowerarmlPos, actor.bone.upperarmlPos, esp_color);
	draw_bone(actor.bone.lowerarmlPos, actor.bone.handlPos, esp_color);

	//Neck -> right hand
	draw_bone(actor.bone.neckPos, actor.bone.upperarmrPos, esp_color);
	draw_bone(actor.bone.lowerarmrPos, actor.bone.upperarmrPos, esp_color);
	draw_bone(actor.bone.lowerarmrPos, actor.bone.handrPos, esp_color);

	//pelvis -> right leg
	draw_bone(actor.bone.pelvisPos, actor.bone.upperleglPos, esp_color);
	draw_bone(actor.bone.lowerleglPos, actor.bone.upperleglPos, esp_color);
	draw_bone(actor.bone.lowerleglPos, actor.bone.footlPos, esp_color);

	//pelvis -> left leg
	draw_bone(actor.bone.pelvisPos, actor.bone.upperlegrPos, esp_color);
	draw_bone(actor.bone.lowerlegrPos, actor.bone.upperlegrPos, esp_color);
	draw_bone(actor.bone.lowerlegrPos, actor.bone.footrPos, esp_color);
}

void player_esp_names(AActor actor, ImColor esp_color)
{
	if (!global->esp.name)
		return;

	FVector2D max, min;
	auto headOrigin = actor.bone.headPos;
	auto rootOrigin = actor.Location;

	headOrigin.Z += 25.f;
	rootOrigin.Z -= 5;

	if (!project_world_to_screen(headOrigin, max))
		return;
	if (!project_world_to_screen(rootOrigin, min))
		return;

	float y = max.Y - 22;

	game_render_string(actor.Name, min.X, y, esp_color, RAcenter | RFbold);
}

void player_esp_distance(AActor actor, ImColor esp_color)
{
	if (!global->esp.distance)
		return;

	FVector2D min;
	auto rootOrigin = actor.Location;

	rootOrigin.Z -= 15;

	if (!project_world_to_screen(rootOrigin, min))
		return;

	float y = min.Y + 8;

	game_render_string(va("%.2f", rootOrigin.distance(game::CameraCachePrivate.Location)/100), min.X, y, esp_color, RAcenter | RFbold);
}

void player_esp_weapon(AActor actor, ImColor esp_color)
{
	if (!global->esp.weapon)
		return;

	FVector2D min;
	auto rootOrigin = actor.Location;

	rootOrigin.Z -= 15;

	if (!project_world_to_screen(rootOrigin, min))
		return;

	float y = min.Y + 18;

	static const char* weapon_name[] = {"None", "Assault", "SMG", "Marksman", "LMG", "Shotgun", "Sniper", "Pistol", "Revolver", "MAX"};

	game_render_string(weapon_name[actor.gun_type], min.X, y, esp_color, RAcenter | RFbold);
}

void player_esp_snap(AActor actor, ImColor esp_color)
{
	if (global->esp.snapType == 0)
		return;

	auto location = actor.Location;
	location.z -= 10;

	FVector2D out;
	if (!project_world_to_screen(location, out))
		return;

	float Y = 0;

	switch (global->esp.snapType)
	{
	case 1: Y = 5; break;
	case 2: Y = game_get_screen_size().y / 2; break;
	case 3: Y = game_get_screen_size().y - 5; break;
	}
	game_render_line(FVector2D(game_get_screen_size().x / 2, Y), out, esp_color, 1);
}

//Pulled from Quake Engine
FVector AnglesToForward(FVector origin, FVector Angles, float distance)
{
	float angle, sr, sp, sy, cr, cp, cy, PiDiv;
	PiDiv = ((float)M_PI / 180);
	angle = TORAD(Angles.y);
	sy = (float)sinf(angle);
	cy = (float)cosf(angle);
	angle = TORAD(Angles.x);
	sp = sinf(angle);
	cp = (float)cosf(angle);
	angle = TORAD(Angles.y);
	sr = (float)sinf(angle);
	cr = (float)cosf(angle);
	FVector Final = FVector((cp * cy * distance) + origin.X, (cp * sy * distance) + origin.Y, (-sp * distance) + origin.Z);
	return Final;
}

void player_esp_angles(AActor actor, ImColor esp_color)
{
	if (!global->esp.view_angles)
		return;

	FVector2D screen[2];
	auto neckOrigin = actor.bone.neckPos;
	auto forward = AnglesToForward(neckOrigin, actor.Rotation, 90.f);

	if (!project_world_to_screen(neckOrigin, screen[0]))
		return;
	if (!project_world_to_screen(forward, screen[1]))
		return;

	game_render_line(screen[0], screen[1], esp_color, 1);
}

void player_esp_healthbar(AActor actor)
{
	if (!global->esp.health_bar)
		return;

	static float max_health = 100;

	ImVec4 color = ImVec4(.2, .96, .32, 1);

	FVector2D max, min;
	auto headOrigin = actor.bone.headPos;
	auto rootOrigin = actor.Location;

	headOrigin.Z += 25.f;
	rootOrigin.Z -= 5;

	if (!project_world_to_screen(headOrigin, max))
		return;
	if (!project_world_to_screen(rootOrigin, min))
		return;

	float flWidth = fabs((max.Y - min.Y) / 4.f);
	float height = min.Y - max.Y;
	height = height + 8;
	float width = height / 2;
	float x = min.X - (width / 2.f);
	float y = max.Y - 4;

	if (actor.health <= 75 && actor.health > 50)
		color = ImVec4(.96, .77, .25, 1);
	if (actor.health < 50)
		color = ImVec4(1, 0, 0, 1);

	auto health_height = (height) * float(actor.health / max_health);
	game_render_icon(x - 12, y, 3, height, ImVec4(0, 0, 0, .5));
	game_render_icon(x - 12, (y + (height)) - health_height, 3, health_height, color);
}

void render_esp()
{
	if (Actors.empty())
		return;

	for (auto actor : Actors)
	{
		ImColor esp_color = { 0,255,0 };

		if (bitset(actor.flags, actor_flags::ENEMY))
		{
			esp_color = { 255,0,0 };
			if (bitset(actor.flags, actor_flags::VISIBLE))
				esp_color = { 255,255,0 };
			
		}

		player_esp_box(actor, esp_color);
		player_esp_bones(actor, esp_color);
		player_esp_snap(actor, esp_color);
		player_esp_names(actor, { 255,255,255 });
		player_esp_distance(actor, { 255,255,255 });
		player_esp_weapon(actor, { 255,255,255 });
		player_esp_angles(actor, esp_color);
		player_esp_healthbar(actor);
	}
	Sleep(1);
}