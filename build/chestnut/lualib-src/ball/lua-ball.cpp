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
#include <EntitasPP\SystemContainer.h>
#include "JoinSystem.h"
#include "MoveSystem.h"
#include "MapSystem.h"
#include "IndexSystem.h"

struct ball_aux {
	std::shared_ptr<Chestnut::EntitasPP::Pool> pool;
	std::shared_ptr<Chestnut::Ball::JoinSystem> joinSystem;
	std::shared_ptr<Chestnut::Ball::MoveSystem> moveSystem;
	std::shared_ptr<Chestnut::Ball::MapSystem> mapSystem;
	std::shared_ptr<Chestnut::Ball::IndexSystem> indexSystem;
};

static int
lalloc(lua_State *L) {
	luabridge::getGlobalNamespace(L)
		.beginNamespace("Chestnut")
		.beginNamespace("EntitasPP")
		.beginClass<Chestnut::EntitasPP::Pool>("Pool")
		.addConstructor<void(*)(void)>()
		.addFunction("Test", &Chestnut::EntitasPP::Pool::Test)
		.addFunction("CreateSystemPtr", &Chestnut::EntitasPP::Pool::CreateSystemPtr)
		.endClass()
		.beginClass<Chestnut::EntitasPP::ISystem>("ISystem")
		.addFunction("SystemType", &Chestnut::EntitasPP::ISystem::SystemType)
		.endClass()
		.beginClass<Chestnut::EntitasPP::ISetPoolSystem>("ISetPoolSystem")
		.addFunction("SetPool", &Chestnut::EntitasPP::ISetPoolSystem::SetPool)
		.endClass()
		.beginClass<Chestnut::EntitasPP::IInitializeSystem>("IInitializeSystem")
		.addFunction("Initialize", &Chestnut::EntitasPP::IInitializeSystem::Initialize)
		.endClass()
		.beginClass<Chestnut::EntitasPP::IExecuteSystem>("IExecuteSystem")
		.addFunction("Execute", &Chestnut::EntitasPP::IExecuteSystem::Execute)
		.endClass()
		.beginClass<Chestnut::EntitasPP::IFixedExecuteSystem>("IFixedExecuteSystem")
		.addFunction("FixedExecute", &Chestnut::EntitasPP::IFixedExecuteSystem::FixedExecute)
		.endClass()
		.endNamespace()
		.beginNamespace("Ball")
		.beginClass<Chestnut::Ball::MoveSystem>("MoveSystem")
		.addConstructor<void(*)(void)>()
		.endClass()
		.beginClass<Chestnut::Ball::JoinSystem>("JoinSystem")
		.addConstructor<void(*)(void)>()
		.endClass()
		.beginClass<Chestnut::Ball::IndexSystem>("IndexSystem")
		.addConstructor<void(*)(void)>()
		.endClass()
		.beginClass<Chestnut::Ball::MapSystem>("MapSystem")
		.addConstructor<void(*)(void)>()
		.endClass()
		.endNamespace()
		.endNamespace();

	return 1;
}

static int
lrelease(lua_State *L) {
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
	lua_pushcfunction(L, lrelease);
	lua_setfield(L, -2, "__gc");

	lua_pushcclosure(L, lalloc, 1);

	return 1;
}
