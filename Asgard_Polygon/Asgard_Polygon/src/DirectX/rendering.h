#pragma once

#define bitset(bit, value) ((bit & value) == value)

enum renderFlags
{
	NO_FLAGS = 0,

	// X
	RAleft = (1 << 1),
	RAcenter = (1 << 2),
	RAright = (1 << 3),

	//Y
	RAtop = (1 << 4),
	RAmiddle = (1 << 5),
	RAbottom = (1 << 6),

	//Style
	RFbold = (1 << 7),
	RFOutline = (1 << 8),
};

FVector2D game_get_screen_size();
void measure(const std::string& text, float& width, float& height);
void game_render_string(const std::string& text, float x, float y, ImColor color, int flags = 0);
void game_render_icon(float x, float y, float width, float height, ImColor color, int flags = 0);
void game_render_circle(float x, float y, float radius, ImColor color, int flags = 0);
void game_render_line(FVector2D start, FVector2D end, ImColor color, float size);
void game_render_line(float x, float y, float x2, float y2, ImColor color, float size);
void game_render_arrow(float x, float y, float scale, float angle, ImColor color, float size = 1);