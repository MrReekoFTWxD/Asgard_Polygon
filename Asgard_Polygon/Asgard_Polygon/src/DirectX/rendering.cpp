#include "pch.h"
#include "rendering.h"


FVector2D game_get_screen_size()
{
	return FVector2D(global->width, global->height);
}

void measure(const std::string& text, float& width, float& height)
{
	auto text_size = ImGui::CalcTextSize(text.data());

	width = text_size.x;
	height = text_size.y;
}

void game_render_string(const std::string& text, float x, float y, ImColor color, int flags)
{
	float width;
	float height;

	measure(text, width, height);

	if (bitset(flags, RAcenter)) x -= (width * .5f);
	if (bitset(flags, RAright)) x -= width;

	if (bitset(flags, RAmiddle)) y -= (height * .5f);
	if (bitset(flags, RAbottom)) y -= height;

	if (bitset(flags, RFbold))
	{
		ImVec4 black = { 0,0,0,.5f };

		ImGui::GetBackgroundDrawList()->AddText(NULL, 12.f, ImVec2(x + 1, y - 1), ImColor(black), text.data());
		ImGui::GetBackgroundDrawList()->AddText(NULL, 12.f, ImVec2(x + 1, y + 1), ImColor(black), text.data());
		ImGui::GetBackgroundDrawList()->AddText(NULL, 12.f, ImVec2(x - 1, y - 1), ImColor(black), text.data());
		ImGui::GetBackgroundDrawList()->AddText(NULL, 12.f, ImVec2(x - 1, y + 1), ImColor(black), text.data());

	}


	ImGui::GetBackgroundDrawList()->AddText(NULL, 12.f, ImVec2(x, y), color, text.data());
}

void game_render_icon(float x, float y, float width, float height, ImColor color, int flags)
{

	if (bitset(flags, RAcenter)) x -= width * 0.5f;
	if (bitset(flags, RAright)) x -= width;
	if (bitset(flags, RAmiddle)) y -= height * 0.5f;
	if (bitset(flags, RAbottom)) y -= height;

	ImVec2 p_min(x, y);
	ImVec2 p_max(x + width, y + height);

	if (bitset(flags, RFOutline))
	{
		p_min.x - 3;
		p_min.y - 3;
		p_max.x + 6;
		p_max.y + 6;
		ImGui::GetBackgroundDrawList()->AddRect(p_min, p_max, color, 0, 0, 2.f);
	}
	else
		ImGui::GetBackgroundDrawList()->AddRectFilled(p_min, p_max, color);
}

void game_render_circle(float x, float y, float radius, ImColor color, int flags)
{
	if (bitset(flags, RAcenter)) x -= radius * 0.5f;
	if (bitset(flags, RAright)) x -= radius;
	if (bitset(flags, RAmiddle)) y -= radius * 0.5f;
	if (bitset(flags, RAbottom)) y -= radius;

	int segment = ImGui::GetBackgroundDrawList()->_CalcCircleAutoSegmentCount(radius);

	if (bitset(flags, RFOutline))
		ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(x, y), radius, color, segment, 2);
	else
		ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(x, y), radius, color, segment);
}

void game_render_line(FVector2D start, FVector2D end, ImColor color, float size)
{
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(start.X, start.Y), ImVec2(end.X, end.Y), color, size);
}

void game_render_line(float x, float y, float x2, float y2, ImColor color, float size)
{
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x, y), ImVec2(x2, y2), color, size);
}

void game_render_arrow(float x, float y, float scale, float angle, ImColor color, float size)
{
	scale /= 2;
	x += scale;
	y += scale;
	angle = (3.14f / 180) * (angle - 27);

	game_render_line((x - (cos(angle) * scale)), (y - (sin(angle) * scale)), (x - (cos(M_PI * 0.66667 + angle) * scale)), (y - (sin(M_PI * 0.66667 + angle) * scale)), color, size);
	game_render_line((x - (cos(angle) * scale)), (y - (sin(angle) * scale)), (x - (cos(angle) * scale / 80.76667)), (y - (sin(angle) * scale / 80.76667)), color, size);

	game_render_line((x - (cos(M_PI * 1.33333 + angle) * scale)), (y - (sin(M_PI * 1.33333 + angle) * scale)), (x - (cos(M_PI * 0.66667 + angle) * scale)), (y - (sin(M_PI * 0.66667 + angle) * scale)), color, size);
	game_render_line((x - (cos(M_PI * 1.33333 + angle) * scale)), (y - (sin(M_PI * 1.33333 + angle) * scale)), (x - (cos(angle) * scale / 80.76667)), (y - (sin(angle) * scale / 80.76667)), color, size);
}