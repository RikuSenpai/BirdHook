#include "lua-5.3.5\src\lua.hpp"
#include <iostream>
#include <Windows.h>
#include "Configuration.hpp"
#include "menu.h"

static int GuiSetInt(lua_State *L)
{
	/* 得到参数个数 */
	if (!(lua_gettop(L) == 2))
		return 0;

	int value = 0;
	const char *name;
	name = lua_tostring(L, 1);
	value = lua_tointeger(L, 2);

	Config->SetValueInt((char*)name, value);

	return 0;
}

static int GuiSetColor(lua_State *L)
{
	/* 得到参数个数 */
	if (!(lua_gettop(L) == 5))
		return 0;

	float r, g, b, a;
	const char *name;
	name = lua_tostring(L, 1);
	r = lua_tonumber(L, 2);
	g = lua_tonumber(L, 3);
	b = lua_tonumber(L, 4);
	a = lua_tonumber(L, 5);


	Config->SetValueColor((char*)name, r, g, b, a);

	return 0;
}

static int GuiSetIntArray(lua_State *L)
{
	/* 得到参数个数 */
	if (!(lua_gettop(L) == 2))
		return 0;

	const char* value;
	const char *name;
	name = lua_tostring(L, 1);
	value = lua_tostring(L, 2);

	Config->SetValueIntArray((char*)name, (char*)value);

	return 0;
}

static int GuiRefreshMenuStyle(lua_State *L)
{
	/* 得到参数个数 */
	if (!(lua_gettop(L) == 0))
		return 0;

	//set_my_diy_style();

	return 0;
}

static const luaL_Reg funcs[] = {
	{"SetValueInt", GuiSetInt},
	{"SetValueIntArray", GuiSetIntArray},
	{"SetValueColor",GuiSetColor},
	{"RefreshMenuStyle",GuiRefreshMenuStyle},
	{NULL, NULL}
};

DWORD WINAPI execluafile(LPVOID *filepath)
{
	/* 初始化Lua */
	lua_State* L;

	L = luaL_newstate();

	/* 载入Lua基本库 */
	luaL_openlibs(L);
	/* 注册函数 */
	lua_newtable(L);
	//先把一个table压入VS，然后在调用luaL_setfuncs就会把所以的func存到table中  
	//注意不像luaL_register这个table是个无名table，可以在的使用只用一个变量来存入这个table。  
	//e.g local clib = require "libname". 这样就不会污染全局环境。比luaL_register更好。  
	luaL_setfuncs(L, funcs, 0);
	lua_setglobal(L, "Gui"); //设置表名为Gui

	/* 运行脚本 */
	luaL_dofile(L, (char *)filepath);
	/* 清除Lua */
	lua_close(L);

	return 0;
}