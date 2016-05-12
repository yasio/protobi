#include "luapp.h"

#include "lua_cocos2dx_coco_studio_manual.hpp"
#include "lua_cocos2dx_controller_manual.hpp"
#include "lua_cocos2dx_deprecated.h"
#include "lua_cocos2dx_experimental_manual.hpp"
#include "lua_cocos2dx_experimental_video_manual.hpp"
#include "lua_cocos2dx_extension_manual.h"
#include "lua_cocos2dx_manual.hpp"
#include "lua_cocos2dx_physics_manual.hpp"
#include "lua_cocos2dx_spine_manual.hpp"
#include "lua_cocos2dx_ui_manual.hpp"
#include "lua_cocos2dx_custom.hpp"

#include "LuaBasicConversions.h"
#include <ui/CocosGUI.h>
#include "purelib/NXNodesLayout.h"

// 不影响lua栈
int call_lua_func(lua_State* lua, const char* func, const char* fmt, ...)
{
	va_list vl;
	int narg, nres;
	int len;

	len = lua_gettop(lua); // 保存栈

	va_start(vl, fmt);

	lua_getglobal(lua, func);

	if (!lua_isfunction(lua, -1))
	{
		CCLOG("call_lua_func:The LUA function:%s not exist!", func);
		goto err_exit;
	}


	narg = 0;

	while (*fmt)
	{
		switch (*fmt++)
		{
		case 'd':
			lua_pushinteger(lua, va_arg(vl, int));
			break;

		case 'f':
			lua_pushnumber(lua, va_arg(vl, double));
			break;

		case 's':
			lua_pushstring(lua, va_arg(vl, char*));
			break;

		case ';':
			goto endwhile;

		default:
			CCLOG("call_lua_func: Unknown argument format");
			goto err_exit;
		}

		narg++;

		//栈上至少有一个空余,否则增长
		if (!lua_checkstack(lua, 1))
		{
			CCLOG("call_lua_func: LUA stack space insufficient!\n");
			goto err_exit;
		}
	}

endwhile:

	nres = strlen(fmt);

	if (lua_pcall(lua, narg, nres, 0) != 0)
	{
		CCLOG("call_lua_func: error occured --- [%s]\n", lua_tostring(lua, -1));
		goto err_exit;
	}

	nres = -nres;

	while (*fmt)
	{
		switch (*fmt++)
		{
		case 'd':
			if (!lua_isnumber(lua, nres))
			{
				CCLOG("%s", "Invalid result format!");
				goto err_exit;
			}

			*va_arg(vl, int*) = (int)lua_tointeger(lua, nres);
			break;

		case 'f':
			if (!lua_isnumber(lua, nres))
			{
				CCLOG("%s", "Invalid result format!");
				goto err_exit;
			}

			*va_arg(vl, double*) = (double)lua_tonumber(lua, nres);
			break;

		case 's':
			if (!lua_isstring(lua, nres))
			{
				CCLOG("%s", "Invalid result format!");
				goto err_exit;
			}

			strcpy(va_arg(vl, char*), lua_tostring(lua, nres));
			break;

		default:
			CCLOG("%s", "Unknown result format!");
			goto err_exit;
		}

		nres++;
	}

	va_end(vl);
	lua_settop(lua, len); // 恢复栈

	return 0;

err_exit:
	va_end(vl);
	lua_settop(lua, len); // 恢复栈
	return -1;
}

void lua_table_setvalue(lua_State* L, const char* tableName, const char* tableItem, int value)
{
    lua_getglobal(L, tableName);
    lua_pushnumber(L, value);
    lua_setfield(L, -2, tableItem);
    lua_pop(L, 1);
}

void lua_table_setvalue(lua_State* L, const char* tableName, const char* tableItem, const char* value, int len)
{
    lua_getglobal(L, tableName);
    lua_pushlstring(L, value, len);
    lua_setfield(L, -2, tableItem);
    lua_pop(L, 1);
}

void lua_dump_stack(lua_State *L) {
	auto top = lua_gettop(L);
	for (auto i = 1; i <= top; i++) { /* repeat for each level */
		int t = lua_type(L, i);
		switch (t) {
		case LUA_TSTRING: /* strings */
			CCLOG("`%s'", lua_tostring(L, i));
			break;
		case LUA_TBOOLEAN: /* booleans */
			CCLOG(lua_toboolean(L, i) ? "true" : "false");
			break;
		case LUA_TNUMBER: /* numbers */
			CCLOG("%g", lua_tonumber(L, i));
			break;
		default: /* other values */
			CCLOG("%s", lua_typename(L, t));
			break;
		}
		CCLOG(" "); /* put a separator */
	}
	CCLOG("\n"); /* end the listing */
}

#if 0
void lua_test(lua_State* L)
{
	//// lua_gettop(L);
	lua_getglobal(L, "view_config");
	lua_getfield(L, -1, "width");
	int checkVal = 0;
	if (lua_isnumber(L, -1))
	{
		checkVal = lua_tonumber(L, -1);
	}

	int len = lua_gettop(L);
	lua_pop(L, 1); // pop view_config.width

	call_lua_func(L, "hello", "s", "before modify ===> ");

	/*lua_getglobal(L, "view_config");*/
	lua_pushnumber(L, 20);
	lua_setfield(L, -2, "width");
	/*lua_pushnumber(L, 550);
	lua_setfield(L, -2, "height");*/

	call_lua_func(L, "hello", "s", "after modify ===> ");
	/*if (engine->executeScriptFile("D:\\develop\\TDProject\\trunk\\TDGame\\Resources\\scripts\\src\\main.lua")) {
		return false;
		}*/

	lua_pushnumber(L, 550);
	
	len = lua_gettop(L);
	lua_setfield(L, 1, "height"); // -( 2 - 1 + 1 )

	// lua_pop(L, 2);
	   
	call_lua_func(L, "hello", "s", "after modify2 ===> ");

	lua_pop(L, 1); // pop table view_config


	lua_pushnumber(L, 100);
	lua_pushnumber(L, 10);

	checkVal = lua_tonumber(L, -1);

	len = lua_gettop(L);
	lua_pushvalue(L, -2);
	len = lua_gettop(L);

	checkVal = lua_tonumber(L, -1);

	lua_dump_stack(L);
}
#endif

static void impl_setNodeRT(Node* pNode, const cocos2d::Point& p)
{ // right top
    CC_ASSERT(pNode);
    auto scrSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
    cocos2d::Point delta = cocos2d::Vec2(scrSize.width, scrSize.height) - VisibleRect::rightTop();
    Size size = pNode->getContentSize() * nodes_utility::getScale2D(pNode);
    cocos2d::Point achorPoint = Vec2::ZERO;
    if (!pNode->isIgnoreAnchorPointForPosition())
    {
        achorPoint = pNode->getAnchorPoint();
    }

    auto screenP = cocos2d::Vec2(
        adjust_coord_neg(scrSize.width, size.width, achorPoint.x, p.x) - delta.x,
        adjust_coord_neg(scrSize.height, size.height, achorPoint.y, p.y) - delta.y);

    auto clientP = PointApplyTransform(screenP, pNode->getParent()->getWorldToNodeTransform()); // PointApplyAffineTransform(screenP, pNode->getParent()->getWorldToNodeAffineTransform());
    
    pNode->setPosition(clientP);
}

static int lua_vis_setNodeRT(lua_State* L)
{
    if (lua_gettop(L) >= 3) {
        cocos2d::Node* target = nullptr;
        auto ok = luaval_to_object<cocos2d::Node>(L, 1, "cc.Node", &target);
        auto r = lua_tonumber(L, 2);
        auto t = lua_tonumber(L, 3);

        impl_setNodeRT(target, Vec2(r, t));

        // target->getParent()->setScale(0.5f);
    }
    return 0;
}

static int lua_vscr_setPositionTop(lua_State* L)
{
    return 0;
}

static luaL_Reg layoutFuncs[] = {
    { "setNodeRT", lua_vis_setNodeRT},
        {NULL, NULL}
    };

void register_all_modules(lua_State *L)
{
    luaL_register(L, "layoutv", layoutFuncs);
    // lua_register(L, "vis_setNodeRT", lua_vis_setNodeRT);

	register_all_cocos2dx_ui_manual(L);
	//extension module must be registed after ui module
	register_all_cocos2dx_extension_manual(L);
	register_all_cocos2dx_coco_studio_manual(L);
	register_all_cocos2dx_spine_manual(L);
	register_all_cocos2dx_manual(L);
	register_all_cocos2dx_custom(L);
}
