extern "C"
{
#include "../lua/include/lua.h"
#include "../lua/include/lauxlib.h"
#include "../lua/include/lualib.h"
}

#include "luaextern.h"
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>

#ifdef _WIN32
#pragma comment(lib, "../lua/liblua53.a")
#endif

struct Obs
{
	float x = 0.0;
	float y = 0.0;
	unsigned char type = 0;
};

static lua_State* L;
static bool luaFailed = false;
static Player* player;

static bool luaOkay(lua_State* L, const int status);
static bool luaCallFunction(lua_State* L, char* functionName, int numParams, int numReturns);
static int luaSetTurn(lua_State* L);
static int luaGetObstacles(lua_State* L);
static void luaCreateObsTable(lua_State* L, Point object);
static int luaGetPosition(lua_State* L);
static int luaGetHeight(lua_State* L);
static int luaGetWidth(lua_State* L);
static void addModule(lua_State* L, const luaL_Reg* funcs, const char* name);
static bool luaFileExists();
static void createDefaultLuaFile();
static int luaGetVelocity(lua_State* L);

static const luaL_Reg snekFunctions[] = {
	{"setTurn", luaSetTurn},
	{"getObstacles", luaGetObstacles},
	{"getPosition", luaGetPosition},
	{"getVelocity", luaGetVelocity},
	{NULL, NULL}
};

static const luaL_Reg screenFunctions[] = {
	{"getWidth", luaGetWidth},
	{"getHeight", luaGetHeight},
	{NULL,NULL}
};

void addModule(lua_State* L, const luaL_Reg* funcs, const char* name)
{
	lua_newtable(L);
	luaL_setfuncs(L, funcs, 0);
	lua_pushvalue(L, -1);
	lua_setglobal(L, name);
}

bool luaFileExists()
{
	struct stat info;
	if (stat("snek.lua", &info) != 0)
	{
		return false;
	}
	return true;
}

void initLua(Player* playerInstance)
{
	luaFailed = false;
	player = playerInstance;
	L = luaL_newstate(); //Init lua state
	luaL_openlibs(L); //Allow standard libraries

	addModule(L, snekFunctions, "snek");
	addModule(L, screenFunctions, "screen");
	//lua_pushstring(L, "snek");
	//luaL_setfuncs(L, snekFunctions, 1);
	//lua_setfield(L, -2, "snek");

	//lua_register(L, "getObstacles", luaGetObstacles);
	//lua_register(L, "getPosition", luaGetPosition);
	
	if (!luaFileExists())
	{
		createDefaultLuaFile();
	}
	luaOkay(L, luaL_loadfile(L, "snek.lua")); //Load file
	luaOkay(L, lua_pcall(L, 0, LUA_MULTRET, 0)); //Run code once for definitions
}

void closeLua()
{
	lua_close(L);
}

bool luaOkay(lua_State* L, const int status)
{
	if (status != LUA_OK)
	{
		std::cout << lua_tostring(L, -1) << std::endl;
		luaFailed = true;
		return false;
	}
	return true;
}

bool luaCallFunction(lua_State* L, char* functionName, int numParams, int numReturns)

{
	if (lua_getglobal(L, functionName))
	{
		if (lua_isfunction(L, -1))
		{
			if (luaOkay(L, lua_pcall(L, numParams, numReturns, 0)))
			{
				return true; //Return values are on the lua stack so we just return whether it worked or not.
			}
		}
	}
	return false;
}

int luaSetTurn(lua_State* L)
{
	if (lua_gettop(L) == 1)
	{
		int turn = (int)lua_tonumber(L, 1);
		if (turn > 1)
		{
			turn = 1;
		}
		else if (turn < -1)
		{
			turn = -1;
		}
		player->setSteering(turn);
	}
	return 0;
}

void luaCreateObsTable(lua_State* L, Point object)
{
	lua_newtable(L);
	lua_pushstring(L, "x");
	lua_pushnumber(L, object.m_position.x);
	lua_settable(L, -3);
	lua_pushstring(L, "y");
	lua_pushnumber(L, object.m_position.y);
	lua_settable(L, -3);
	lua_pushstring(L, "r");
	lua_pushnumber(L, object.m_radius);
	lua_settable(L, -3);
	lua_pushstring(L, "type");
	switch (object.m_type)
	{
	case Point::PointType::Snek:
		lua_pushstring(L, "snek");
		lua_settable(L, -3);
		break;
	default:
		lua_pop(L, 2);
		break;
	}
}

int luaGetObstacles(lua_State* L)
{
	if (lua_gettop(L) == 1)
	{
		float radius = 5.0;
		if (lua_isnumber(L, -1))
		{
			radius = (float)lua_tonumber(L, -1);
		}
		//luaCreateObsTable(L, Point(Vec2f(10.0,2.0), 10.0, Point::PointType::Snek));
		std::vector<Point> points;
		player->getObstacles(radius, points);
		
		lua_newtable(L);
		for (int i = 0; i < points.size(); i++)
		{
			lua_pushnumber(L, i+1);
			luaCreateObsTable(L, points[i]);
			lua_settable(L, -3);
		}
	}
	return 1;
}

int luaGetPosition(lua_State* L)
{
	Vec2f pos = player->getPosition();
	lua_pushnumber(L, pos.x);
	lua_pushnumber(L, pos.y);
	return 2;
}

int luaGetVelocity(lua_State* L)
{
	Vec2f vel = player->getVelocity();
	lua_pushnumber(L, vel.x);
	lua_pushnumber(L, vel.y);
	return 2;
}

int luaGetHeight(lua_State* L)
{
	lua_pushnumber(L, player->getHeight());
	return 1;
}

int luaGetWidth(lua_State* L)
{
	lua_pushnumber(L, player->getWidth());
	return 1;
}

void luaUpdate()
{
	if (luaFailed)
		return;

	luaCallFunction(L, (char*)"update", 0, 0);

	////char* str = new char"a = 13.5 + 10.0";
	//std::string str = "a = 13 + 10asda";
	//int r = luaL_dostring(L, str.c_str());
	//if (r == LUA_OK)
	//{
	//	lua_getglobal(L, "a");
	//	if (lua_isnumber(L, -1))
	//	{
	//		float a = (float)lua_tonumber(L, -1);
	//		std::cout << "a = " << a << std::endl;
	//	}
	//}
	//else
	//{
	//	std::cout << lua_tostring(L, -1) << std::endl;
	//	luaFailed = true;
	//}
}

void createDefaultLuaFile()
{
	const char* defaultCode = \
"--[[\n\
The update function is called every frame.In addition to this you can create your own global variables and functions.\n\
\n\
	Usable Modules :\n\
		You can use most default libraries. Calling print will print to the console that opens with the game.\n\
\n\
		Game specific modules :\n\
\n\
		snek:\n\
			snek.setTurn(turnDirection)\n\
				(IN) Number between -1 and 1. -1 Turns left and 1 turns right. 0 goes straight.\n\
				(RETURNS) No return value\n\
\n\
			snek.getObstacles(radius)\n\
				(IN) Radius of search.The search is from the position of the snek which is its head.\n\
				(RETURN) List of points. A point structure has an x, y position a radius rand a type.\n\
				Current types :\n\
					-snek\n\
\n\
			snek.getPosition()\n\
				(IN) Nothing\n\
				(OUT) Position of snake head. Returned as two values so the syntax x, y = snek.getPosition() will work.\n\
\
			snek.getVelocity()\n\
				(IN) Nothing\n\
				(OUT) Velocity of snek. Returned as two values so the syntax velx, vely = snek.getVelocity() will work.\n\
\n\
		screen :\n\
			screen.getWidth()\n\
				(IN) Nothing\n\
				(OUT) Width of playable area\n\
\n\
			screen.getHeight()\n\
				(IN) Nothing\n\
				(OUT) Height of playable area\n\
--]]\n\
\n\
function update()\n\
\n\
end";

	std::ofstream file("snek.lua");
	if (file.is_open())
	{
		file << defaultCode;
		file.close();
	}
}