﻿#define LUA_LIB

#ifdef __cplusplus
extern "C" {
#endif
#include <lua.h>
#include <lauxlib.h>

	LUAMOD_API int luaopen_fixmath_bounce(lua_State *L);

#ifdef __cplusplus
}
#endif

#include <bounce\bounce.h>
#include <bounce\common/math/b3r32.h>
#include <LuaBridge\LuaBridge.h>

// You should implement this function to listen when a profile scope is opened.
void b3BeginProfileScope(const char* name) {}

// You must implement this function if you have implemented b3BeginProfileScope.
// Implement this function to listen when a profile scope is closed.
void b3EndProfileScope() {}

b3Mat33 b3DiagonalWrap(float32 x, float32 y, float32 z) {
	return b3Diagonal(x, y, z);
}

struct ltable {};

class lb3QueryListener : b3QueryListener {
public:
	lb3QueryListener() : b3QueryListener() {}
	virtual ~lb3QueryListener() {}
	int Register(lua_State *L) {
		this->L = L;
		if (!lua_istable(L, 2)) {
			lua_error(L);
		} else {
			lua_settop(L, 2);
			lua_setglobal(L, "");
		}
		return 0;
	}
	virtual bool ReportShape(b3Shape* shape) {
		lua_getglobal(L, "");
	}
private:
	lua_State * L;
};

class lb3RayCastListener {
public:
	// The user must return the new ray cast fraction.
	// If fraction == zero then the ray cast query will be canceled.
	virtual ~lb3RayCastListener() {}
	virtual float32 ReportShape(b3Shape* shape, const b3Vec3& point, const b3Vec3& normal, float32 fraction) {

	}
};

class lb3ContactListener {
public:
	// Inherit from this class and set it in the world to listen for collision events.	
	// Call the functions below to inspect when a shape start/end colliding with another shape.
	/// @warning You cannot create/destroy Bounc3 objects inside these callbacks.
	virtual void BeginContact(b3Contact* contact) = 0;
	virtual void EndContact(b3Contact* contact) = 0;
	virtual void Persisting(b3Contact* contact) = 0;
};

namespace luabridge {

	template <>
	struct Stack <b3R32> {
		static void push(lua_State* L, b3R32 const& step) {
			lua_pushinteger(L, step._i);
		}

		static b3R32 get(lua_State* L, int index) {
			if (lua_isinteger(L, index)) {
				lua_Integer i = lua_tointeger(L, index);
				return b3R32(i);
			} else if (lua_isnumber(L, index)) {
				lua_Number i = lua_tonumber(L, index);
				return b3R32(i);
			} else {
				luaL_error(L, "#%d argments must be table", index);
			}
			return b3R32::zero();
		}
	};

	template <>
	struct Stack <b3R32 const&> : Stack <b3R32 > {};

	template <>
	struct Stack <b3Vec3> {
		static void push(lua_State* L, b3Vec3 const& vec3) {
			lua_createtable(L, 0, 3);
			Stack<b3R32>::push(L, vec3.x);
			lua_setfield(L, -2, "x");
			Stack<b3R32>::push(L, vec3.y);
			lua_setfield(L, -2, "y");
			Stack<b3R32>::push(L, vec3.z);
			lua_setfield(L, -2, "z");

			// create meta bable
		}

		static b3Vec3 get(lua_State* L, int index) {
			if (!lua_istable(L, index)) {
				luaL_error(L, "#%d argments must be table", index);
			}
			b3Vec3 vec3;
			lua_pushvalue(L, index);
			lua_getfield(L, -1, "x");
			vec3.x = Stack<b3R32>::get(L, -1);
			lua_pop(L, 1);
			lua_getfield(L, -1, "y");
			b3R32 t = Stack<b3R32>::get(L, -1);
			float32 y = t;
			vec3.y = y;
			lua_pop(L, 1);
			lua_getfield(L, -1, "z");
			vec3.z = Stack<b3R32>::get(L, -1);
			lua_pop(L, 2);
			return vec3;
		}
	};

	template <>
	struct Stack <b3Vec3 const&> : Stack <b3Vec3 > {};

	template <>
	struct Stack <b3Quat> {
		static void push(lua_State* L, b3Quat const& quat) {
			lua_createtable(L, 0, 4);
			Stack<b3R32>::push(L, quat.x);
			lua_setfield(L, -2, "x");
			Stack<b3R32>::push(L, quat.y);
			lua_setfield(L, -2, "y");
			Stack<b3R32>::push(L, quat.z);
			lua_setfield(L, -2, "z");
			Stack<b3R32>::push(L, quat.w);
			lua_setfield(L, -2, "w");

			// create meta bable
		}

		static b3Quat get(lua_State* L, int index) {
			if (!lua_istable(L, index)) {
				luaL_error(L, "#%d argments must be table", index);
			}
			b3Quat quat;
			lua_pushvalue(L, index);
			lua_getfield(L, -1, "x");
			quat.x = Stack<b3R32>::get(L, -1);
			lua_pop(L, 1);
			lua_getfield(L, -1, "y");
			b3R32 t = Stack<b3R32>::get(L, -1);
			quat.y = t;
			lua_pop(L, 1);
			lua_getfield(L, -1, "z");
			quat.z = Stack<b3R32>::get(L, -1);
			lua_pop(L, 1);
			lua_getfield(L, -1, "w");
			quat.w = Stack<b3R32>::get(L, -1);
			lua_pop(L, 2);
			return quat;
		}
	};

	template <>
	struct Stack <b3Quat const&> : Stack <b3Quat> {};

	template <>
	struct Stack <b3Mat33> {
		static void push(lua_State* L, b3Mat33 const& mat) {
			lua_createtable(L, 0, 3);
			Stack<b3Vec3>::push(L, mat.x);
			lua_setfield(L, -2, "x");
			Stack<b3Vec3>::push(L, mat.y);
			lua_setfield(L, -2, "y");
			Stack<b3Vec3>::push(L, mat.z);
			lua_setfield(L, -2, "z");


			// create meta bable
		}

		static b3Mat33 get(lua_State* L, int index) {
			if (!lua_istable(L, index)) {
				luaL_error(L, "#%d argments must be table", index);
			}
			b3Mat33 mat;
			lua_pushvalue(L, index);
			lua_getfield(L, -1, "x");
			mat.x = Stack<b3Vec3>::get(L, -1);
			lua_pop(L, 1);
			lua_getfield(L, -1, "y");
			mat.y = Stack<b3Vec3>::get(L, -1);
			lua_pop(L, 1);
			lua_getfield(L, -1, "z");
			mat.z = Stack<b3Vec3>::get(L, -1);
			lua_pop(L, 2);
			return mat;
		}
	};

	template <>
	struct Stack <b3Mat33 const&> : Stack <b3Mat33 > {};

	template <>
	struct Stack <b3Transform> {
		static void push(lua_State* L, b3Transform const& trans) {
			lua_createtable(L, 0, 4);
			Stack<b3Vec3>::push(L, trans.position);
			lua_setfield(L, -2, "position");
			Stack<b3Mat33>::push(L, trans.rotation);
			lua_setfield(L, -2, "rotation");

			// create meta bable
		}

		static b3Transform get(lua_State* L, int index) {
			if (!lua_istable(L, index)) {
				luaL_error(L, "#%d argments must be table", index);
			}
			b3Transform trans;
			lua_pushvalue(L, index);
			lua_getfield(L, -1, "position");
			trans.position = Stack<b3Vec3>::get(L, -1);
			lua_pop(L, 1);
			lua_getfield(L, -1, "rotation");
			trans.rotation = Stack<b3Mat33>::get(L, -1);
			lua_pop(L, 2);
			return trans;
		}
	};

	template <>
	struct Stack <b3Transform const&> : Stack <b3Transform > {};

	/*template <>
	struct Stack <b3TimeStep> {
		static void push(lua_State* L, b3TimeStep const& step) {
			lua_createtable(L, 0, 3);
			Stack<b3R32>::push(L, step.dt);
			lua_setfield(L, -2, "dt");
			lua_pushinteger(L, step.velocityIterations);
			lua_setfield(L, -2, "velocityIterations");
			lua_pushboolean(L, step.sleeping);
			lua_setfield(L, -2, "velocityIterations");
		}

		static b3TimeStep get(lua_State* L, int index) {
			if (!lua_istable(L, index)) {
				luaL_error(L, "#%d argments must be table", index);
			}

			b3TimeStep step;
			lua_pushvalue(L, index);
			lua_getfield(L, -1, "dt");
			step.dt = Stack<b3R32>::get(L, -1);
			lua_pop(L, 1);
			lua_getfield(L, -1, "velocityIterations");
			step.velocityIterations = static_cast<u32>(lua_tointeger(L, -1));
			lua_pop(L, 1);
			lua_getfield(L, -1, "sleeping");
			step.sleeping = lua_toboolean(L, -1);
			lua_pop(L, 2);
			return step;
		}
	};

	template <>
	struct Stack <b3TimeStep const&> : Stack <b3TimeStep > {};*/

	template <>
	struct Stack <b3Velocity> {
		static void push(lua_State* L, b3Velocity const& vel) {
			lua_createtable(L, 0, 2);
			Stack<b3Vec3>::push(L, vel.v);
			lua_setfield(L, -2, "v");
			Stack<b3Vec3>::push(L, vel.w);
			lua_setfield(L, -2, "w");
		}

		static b3Velocity get(lua_State* L, int index) {
			if (!lua_istable(L, index)) {
				luaL_error(L, "#%d argments must be table", index);
			}

			b3Velocity vel;
			lua_pushvalue(L, index);
			lua_getfield(L, -1, "v");
			vel.v = Stack<b3Vec3>::get(L, -1);
			lua_pop(L, 1);
			lua_getfield(L, -1, "w");
			vel.w = Stack<b3Vec3>::get(L, -1);
			lua_pop(L, 2);
			return vel;
		}
	};

	template <>
	struct Stack <b3Velocity const&> : Stack <b3Velocity > {};

	template <>
	struct Stack <b3BodyDef> {
		static void push(lua_State* L, b3BodyDef const& def) {
			lua_createtable(L, 0, 8);
			lua_pushinteger(L, (lua_Integer)def.type);
			lua_setfield(L, -2, "type");
			lua_pushboolean(L, def.awake);
			lua_setfield(L, -2, "awake");
			lua_pushlightuserdata(L, def.userData);
			lua_setfield(L, -2, "userData");
			Stack<b3Vec3>::push(L, def.position);
			lua_setfield(L, -2, "position");
			Stack<b3Quat>::push(L, def.orientation);
			lua_setfield(L, -2, "orientation");
			Stack<b3Vec3>::push(L, def.linearVelocity);
			lua_setfield(L, -2, "linearVelocity");
			Stack<b3Vec3>::push(L, def.angularVelocity);
			lua_setfield(L, -2, "angularVelocity");
			Stack<float32>::push(L, def.gravityScale);
			lua_setfield(L, -2, "gravityScale");
		}

		static b3BodyDef get(lua_State* L, int index) {
			if (!lua_istable(L, index)) {
				luaL_error(L, "#%d argments must be table", index);
			}

			b3BodyDef def;
			lua_pushvalue(L, index);
			lua_getfield(L, -1, "type");
			if (!lua_isnil(L, -1)) {
				def.type = (b3BodyType)luaL_checkinteger(L, -1);
			}
			lua_pop(L, 1);
			lua_getfield(L, -1, "awake");
			if (!lua_isnil(L, -1)) {
				def.awake = lua_toboolean(L, -1);
			}
			lua_pop(L, 1);
			lua_getfield(L, -1, "userData");
			if (!lua_isnil(L, -1)) {
				def.userData = lua_touserdata(L, -1);
			}
			lua_pop(L, 1);
			lua_getfield(L, -1, "position");
			if (!lua_isnil(L, -1)) {
				def.position = Stack<b3Vec3>::get(L, -1);
			}
			lua_pop(L, 1);
			lua_getfield(L, -1, "orientation");
			if (!lua_isnil(L, -1)) {
				def.orientation = Stack<b3Quat>::get(L, -1);
			}
			lua_pop(L, 1);
			lua_getfield(L, -1, "linearVelocity");
			if (!lua_isnil(L, -1)) {
				def.linearVelocity = Stack<b3Vec3>::get(L, -1);
			}
			lua_pop(L, 1);
			lua_getfield(L, -1, "angularVelocity");
			if (!lua_isnil(L, -1)) {
				def.angularVelocity = Stack<b3Vec3>::get(L, -1);
			}
			lua_pop(L, 1);
			lua_getfield(L, -1, "angularVelocity");
			if (!lua_isnil(L, -1)) {
				def.angularVelocity = Stack<b3Vec3>::get(L, -1);
			}
			lua_pop(L, 1);
			lua_getfield(L, -1, "gravityScale");
			if (!lua_isnil(L, -1)) {
				def.gravityScale = Stack<float32>::get(L, -1);
			}
			lua_pop(L, 2);
			return def;
		}
	};

	template <>
	struct Stack <b3BodyDef const&> : Stack <b3BodyDef > {};

	/*template <>
	struct Stack <b3SphericalJointDef> {
		static void push(lua_State* L, b3SphericalJointDef const& def) {
			lua_createtable(L, 0, 3);
			lua_pushlightuserdata(L, def.bodyA);
			lua_setfield(L, -2, "bodyA");
			lua_pushlightuserdata(L, def.bodyB);
			lua_setfield(L, -2, "bodyB");
			lua_pushlightuserdata(L, def.userData);
			lua_setfield(L, -2, "userData");
		}

		static b3SphericalJointDef get(lua_State* L, int index) {
			if (!lua_istable(L, index)) {
				luaL_error(L, "#%d argments must be table", index);
			}

			b3SphericalJointDef def;
			lua_pushvalue(L, index);
			lua_getfield(L, -1, "bodyA");
			def.bodyA = static_cast<b3Body*>(lua_touserdata(L, -1));
			lua_pop(L, 1);
			lua_getfield(L, -1, "bodyB");
			def.bodyB = static_cast<b3Body*>(lua_touserdata(L, -1));
			lua_pop(L, 1);
			lua_getfield(L, -1, "userData");
			def.userData = lua_touserdata(L, -1);
			lua_pop(L, 2);
			return def;
		}
	};

	template <>
	struct Stack <b3SphericalJointDef const&> : Stack <b3SphericalJointDef > {};*/

	template <>
	struct Stack <b3ShapeDef> {
		static void push(lua_State* L, b3ShapeDef const& def) {
			lua_createtable(L, 0, 8);
			Stack<const b3Shape*>::push(L, def.shape);
			lua_setfield(L, -2, "shape");
			lua_pushlightuserdata(L, def.userData);
			lua_setfield(L, -2, "userData");
			lua_pushboolean(L, def.isSensor);
			lua_setfield(L, -2, "isSensor");
			Stack<b3R32>::push(L, def.density);
			lua_setfield(L, -2, "density");
			Stack<b3R32>::push(L, def.friction);
			lua_setfield(L, -2, "friction");
			Stack<b3R32>::push(L, def.restitution);
			lua_setfield(L, -2, "restitution");
		}

		static b3ShapeDef get(lua_State* L, int index) {
			if (!lua_istable(L, index)) {
				luaL_error(L, "#%d argments must be table", index);
			}

			b3ShapeDef def;
			lua_pushvalue(L, index);
			lua_getfield(L, -1, "shape");
			int idx = lua_absindex(L, -1);
			b3Shape *shape = Stack<b3Shape *>::get(L, idx);
			if (shape == NULL) {
				luaL_error(L, "shape must be");
			}
			if (shape->GetType() == e_hullShape) {
				def.shape = dynamic_cast<b3HullShape*>(shape);
			}
			lua_pop(L, 1);
			lua_getfield(L, -1, "isSensor");
			if (!lua_isnil(L, -1)) {
				def.isSensor = lua_toboolean(L, -1);
			}
			lua_pop(L, 1);
			lua_getfield(L, -1, "density");
			if (!lua_isnil(L, -1)) {
				def.density = Stack<float32>::get(L, -1);
			}
			lua_pop(L, 1);
			lua_getfield(L, -1, "friction");
			if (!lua_isnil(L, -1)) {
				def.friction = Stack<float32>::get(L, -1);
			}
			lua_pop(L, 1);
			lua_getfield(L, -1, "restitution");
			if (!lua_isnil(L, -1)) {
				def.restitution = Stack<float32>::get(L, -1);
			}
			lua_pop(L, 2);
			return def;
		}
	};

	template <>
	struct Stack <b3ShapeDef const&> : Stack <b3ShapeDef > {};

} // namespace luabridge

int
luaopen_fixmath_bounce(lua_State *L) {
	luabridge::getGlobalNamespace(L)
		.beginNamespace("bounce")
		.beginClass<b3R32>("b3R32")
		.addStaticFunction("MAX", &b3R32::max)
		.addStaticFunction("MIN", &b3R32::min)
		.addStaticFunction("PI", &b3R32::pi)
		.addStaticFunction("E", &b3R32::e)
		.addStaticFunction("ToFloat32", &b3R32::ToFloat32)
		.addStaticFunction("ToFloat64", &b3R32::ToFloat64)
		.endClass()
		.beginClass<b3Mat33>("b3Mat33")
		//.addStaticFunction("b3Mul", &b3Mul)
		//.addStaticFunction("b3Transpose", &b3Transpose)
		.addStaticFunction("b3Diagonal", &b3DiagonalWrap)
		.endClass()
		/*.beginClass<b3Time>("b3Time")
		.addStaticFunction("GetRealTime", &b3Time::GetRealTime)
		.addConstructor<void(*) ()>()
		.addFunction("SetResolution", &b3Time::SetResolution)
		.addFunction("GetCurMicros", &b3Time::GetCurMicros)
		.addFunction("GetDeltaMicros", &b3Time::GetDeltaMicros)
		.addFunction("GetCurSecs", &b3Time::GetCurSecs)
		.addFunction("GetDeltaSecs", &b3Time::GetDeltaSecs)
		.endClass()*/
		.beginClass<b3World>("b3World")
		.addConstructor<void(*) ()>()
		.addFunction("SetContactFilter", &b3World::SetContactFilter)
		.addFunction("SetContactListener", &b3World::SetContactListener)
		.addFunction("SetSleeping", &b3World::SetSleeping)
		.addFunction("SetWarmStart", &b3World::SetWarmStart)
		.addFunction("SetGravity", &b3World::SetGravity)
		.addFunction("CreateBody", &b3World::CreateBody)
		.addFunction("DestroyBody", &b3World::DestroyBody)
		//.addFunction("CreateJoint", &b3World::CreateJoint)
		.addFunction("DestroyJoint", &b3World::DestroyJoint)
		.addFunction("QueryAABB", &b3World::QueryAABB)
		.addFunction("RayCast", &b3World::RayCast)
		.addFunction("Step", &b3World::Step)
		.endClass()
		.beginClass<b3Body>("b3Body")
		.addFunction("GetType", &b3Body::GetType)
		.addFunction("CreateShape", &b3Body::CreateShape)
		.addFunction("DestroyShape", &b3Body::DestroyShape)
		//.addFunction("DestroyContacts", &b3Body::DestroyContacts)
		/*.addFunction("DestroyJoints", &b3Body::DestroyJoints)
		.addFunction("DestroyShapes", &b3Body::DestroyShapes)*/
		.addFunction("GetTransform", &b3Body::GetTransform)
		.addFunction("GetPosition", &b3Body::GetPosition)
		.addFunction("GetOrientation", &b3Body::GetOrientation)
		.addFunction("ApplyForce", &b3Body::ApplyForce)
		.addFunction("ApplyForceToCenter", &b3Body::ApplyForceToCenter)
		.addFunction("ApplyTorque", &b3Body::ApplyTorque)
		.addFunction("ApplyLinearImpulse", &b3Body::ApplyLinearImpulse)
		.addFunction("ApplyAngularImpulse", &b3Body::ApplyLinearImpulse)
		.addFunction("SetLinearVelocity", &b3Body::SetLinearVelocity)
		.addFunction("SetAngularVelocity", &b3Body::SetAngularVelocity)
		.endClass()
		.beginClass<b3Shape>("b3Shape")
		//.addConstructor<void(*)()>()
		.addFunction("GetType", &b3Shape::GetType)
		//.addFunction("GetBody", &b3Shape::GetBody)
		.addFunction("SetSensor", &b3Shape::SetSensor)
		.addFunction("IsSensor", &b3Shape::IsSensor)
		.endClass()
		.deriveClass<b3HullShape, b3Shape>("b3HullShape")
		.addConstructor<void(*) ()>()
		.addData("m_hull", &b3HullShape::m_hull)
		.endClass()
		.beginClass<b3Hull>("b3Hull")
		.addConstructor<void(*) ()>()
		.addData("centroid", &b3Hull::centroid)
		.addData("vertexCount", &b3Hull::vertexCount)
		.endClass()
		.deriveClass<b3BoxHull, b3Hull>("b3BoxHull")
		.addConstructor<void(*)()>()
		.addFunction("SetIdentity", &b3BoxHull::SetIdentity)
		.addFunction("Set", &b3BoxHull::Set)
		.addFunction("SetTransform", &b3BoxHull::SetTransform)
		.endClass()
		.beginClass<lb3QueryListener>("lb3QueryListener")
		.addCFunction("Register", &lb3QueryListener::Register)
		.endClass()
		.endNamespace();
	return 0;
}
