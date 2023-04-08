// Template, IGAD version 3
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/UU - Jacco Bikker - 2006-2023

#pragma once

#include "dungeon.h"
#include "physics.h"

enum class RenderState : int8_t
{
	CREATE_ROOMS,
	SEPARATE_ROOMS,
	DECIDING_MAIN_ROOMS,
	CREATE_CONCAVE_POLYGON,
	TRIANGULATE,
	DONE
};

inline const char* to_string(const RenderState e)
{
	switch (e)
	{
	case RenderState::CREATE_ROOMS: return "Create Rooms";
	case RenderState::SEPARATE_ROOMS: return "Separate Rooms";
	case RenderState::DECIDING_MAIN_ROOMS: return "Deciding Main Rooms";
	case RenderState::CREATE_CONCAVE_POLYGON: return "Create Concave Polygon";
	case RenderState::TRIANGULATE: return "Triangulate";
	case RenderState::DONE: return "Done";
	}
	
	return nullptr;
}

namespace Tmpl8
{
	class Game : public TheApp
	{
		Physics physics{};
		unsigned int color{}, invertedColor{};
		std::vector<Room> rooms{};
		std::vector<Room> mainRooms{};
		std::vector<Room> otherRooms{};
		std::vector<Room> triangles{};
		RenderState renderState{};
		
	public:
		// game flow methods
		void Init() override;
		void Tick(float deltaTime) override;
		void Shutdown() override { /* implement if you want to do something on exit */ }
		// input handling
		void MouseUp(int button) override { /* implement if you want to detect mouse button presses */ }
		void MouseDown(int button) override { /* implement if you want to detect mouse button presses */ }
		void MouseMove(int x, int y) override { mousePos.x = x; mousePos.y = y; }
		void MouseWheel(float y) override { /* implement if you want to handle the mouse wheel */ }
		void KeyUp(int key) override { /* implement if you want to handle keys */ }
		void KeyDown(int key) override { /* implement if you want to handle keys */ }
		// data members
		int2 mousePos;
	};
}
