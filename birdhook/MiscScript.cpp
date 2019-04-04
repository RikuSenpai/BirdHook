#include "lua-5.3.5\src\lua.hpp"
#include <iostream>
#include <Windows.h>
#include "Configuration.hpp"
#include "menu.h"

static int GuiSetInt(lua_State *L)
{
	/* �õ��������� */
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
	/* �õ��������� */
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
	/* �õ��������� */
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
	/* �õ��������� */
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
	/* ��ʼ��Lua */
	lua_State* L;

	L = luaL_newstate();

	/* ����Lua������ */
	luaL_openlibs(L);
	/* ע�ắ�� */
	lua_newtable(L);
	//�Ȱ�һ��tableѹ��VS��Ȼ���ڵ���luaL_setfuncs�ͻ�����Ե�func�浽table��  
	//ע�ⲻ��luaL_register���table�Ǹ�����table�������ڵ�ʹ��ֻ��һ���������������table��  
	//e.g local clib = require "libname". �����Ͳ�����Ⱦȫ�ֻ�������luaL_register���á�  
	luaL_setfuncs(L, funcs, 0);
	lua_setglobal(L, "Gui"); //���ñ���ΪGui

	/* ���нű� */
	luaL_dofile(L, (char *)filepath);
	/* ���Lua */
	lua_close(L);

	return 0;
}