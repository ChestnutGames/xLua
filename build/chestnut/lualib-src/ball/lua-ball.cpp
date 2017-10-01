#define LUA_LIB

#ifdef __cplusplus
extern "C" {
#endif
#include <lua.h>
#include <lauxlib.h>

LUAMOD_API int luaopen_ball(lua_State *L);

#ifdef __cplusplus
}
#endif

#include <LuaBridge\LuaBridge.h>
#include "JoinSystem.h"
#include "MoveSystem.h"

struct ball_aux {
	int dummy;
};

static int
lball_new(lua_State *L) {
	luabridge::getGlobalNamespace(L)
		.beginNamespace("Chestnut")
		.beginNamespace("Ball")
		.beginClass<Chestnut::Ball::MoveSystem>("MoveSystem")
		.endClass()
		.beginClass<Chestnut::Ball::JoinSystem>("JoinSystem")
		.endClass()
		.endNamespace()
		.endNamespace();

	return 0;
}

static int
lball_release(lua_State *L) {
	return 0;
}


int
luaopen_ball(lua_State *L) {
	luaL_Reg l[] = {
		{ NULL, NULL },
	};
	// create metatable
	int n = 0;
	while (l[n].name)
		++n;
	lua_newtable(L);
	lua_createtable(L, 0, n);
	int i = 0;
	for (; i < n; ++i) {
		lua_pushcfunction(L, l[i].func);
		lua_setfield(L, -2, l[i].name);
	}
	lua_setfield(L, -2, "__index");
	lua_pushstring(L, "ball");
	lua_setfield(L, -2, "__metatable");
	lua_pushcfunction(L, lball_release);
	lua_setfield(L, -2, "__gc");

	lua_pushcclosure(L, lball_new, 1);

	return 1;
}
