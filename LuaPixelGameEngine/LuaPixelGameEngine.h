#pragma once
#include "olcPixelGameEngine.h"
#include "Lua/lua.hpp"
#pragma comment (lib, "Lua/olcLua.lib")
#include "sol.hpp"

inline std::vector<const char*> SList = { "ballcatch.lua" };

inline LPVOID EnginePtr;
inline std::unique_ptr<sol::state> LuaGameState;

struct ScreenNamespace{};
struct IoNamespace{};

bool LoadLua(const std::vector<const char*>&);
void UnLoadLua();