#ifndef _LUAPP_H_
#define _LUAPP_H_
#include "CCLuaEngine.h"

void lua_dump_stack(lua_State *L);

void register_all_modules(lua_State *L);

void lua_test(lua_State*);

// 
int call_lua_func(lua_State* lua, const char* func, const char* fmt, ...);

/*
* lua interactive functions
*/
void lua_table_setvalue(lua_State* L, const char* tableName, const char* tableItem, int value);
void lua_table_setvalue(lua_State* L, const char* tableName, const char* tableItem, const char* value, int len);

/*
* lua value iterator helper
*/
class luaw_value
{
public:
	luaw_value(lua_State* l) : l_(l){}

	/// for array elements
	bool        is_string(void) const { return lua_isstring(l_, -1); }
	bool        is_number(void) const { return lua_isnumber(l_, -1); }
	bool        is_boolean(void) const { return lua_isboolean(l_, -1); }

	const char* to_string(void) const { return lua_tostring(l_, -1); }
	double      to_number(void) const { return lua_tonumber(l_, -1); }
	bool        to_boolean(void) const { return lua_toboolean(l_, -1); }
	LUA_INTEGER to_integer(void) const { return lua_tointeger(l_, -1); }

	/// for table fields
	const char* getfield(const char* name, const char* default_value) const { const char* ret;  lua_getfield(l_, -1, name); ret = lua_tostring(l_, -1); lua_pop(l_, 1);  return ret; }
	double      getfield(const char* name, double default_value) const { LUA_NUMBER ret; lua_getfield(l_, -1, name); ret = lua_tonumber(l_, -1); lua_pop(l_, 1); return ret; }
	bool        getfield(const char* name, bool default_value) const { bool ret; lua_getfield(l_, -1, name); ret = lua_toboolean(l_, -1); lua_pop(l_, 1); return ret; }
	LUA_INTEGER getfield(const char* name, LUA_INTEGER default_value) const { LUA_INTEGER ret; lua_getfield(l_, -1, name); ret = lua_tointeger(l_, -1); lua_pop(l_, 1); return ret; }

private:
	lua_State* l_;
};

/*
usage:
[] (luaw_value value) {
}
*/
template<typename _Fty>
void luaw_gettable(lua_State* L, const char* table, const _Fty& callback)
{
	lua_getglobal(L, table);
	callback(L);
	lua_pop(L, 1);
}

/*
usage:
  [] (luaw_value value) {
  }
*/
template<typename _Fty>
void luaw_foreach(lua_State* L, const char* table, const _Fty& callback)
{
	auto top = lua_gettop(L);

	lua_getglobal(L, table);
	bool istable = lua_istable(L, -1);
	
	if (istable)
	{
		auto n = luaL_len(L, -1);
		for (auto i = 1; i <= n; ++i)
		{
			lua_rawgeti(L, -1, i);
			callback(L);
			lua_pop(L, 1);
		}
	}

	lua_settop(L, top);
}

#endif
