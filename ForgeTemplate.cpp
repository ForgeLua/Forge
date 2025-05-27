/*
 * Part of Forge <https://github.com/iThorgrim/Forge>, a standalone fork of Eluna Lua Engine.
 * 
 * Copyright (C) Forge contributors
 * Based on Eluna <https://elunaluaengine.github.io/>
 * Copyright (C) Eluna Lua Engine contributors
 * 
 * Licensed under the GNU GPL v3 only.
 * See LICENSE file or <https://www.gnu.org/licenses/>.
 */

// Forge
#include "LuaEngine.h"
#include "ForgeIncludes.h"
#include "ForgeTemplate.h"
#include "ForgeUtility.h"

template<> inline int ForgeTemplate<unsigned long long>::Add(lua_State* L) { return ForgeTemplateHelper<unsigned long long>::PerformOp(L, std::plus()); }
template<> inline int ForgeTemplate<unsigned long long>::Subtract(lua_State* L) { return ForgeTemplateHelper<unsigned long long>::PerformOp(L, std::minus()); }
template<> inline int ForgeTemplate<unsigned long long>::Multiply(lua_State* L) { return ForgeTemplateHelper<unsigned long long>::PerformOp(L, std::multiplies()); }
template<> inline int ForgeTemplate<unsigned long long>::Divide(lua_State* L) { return ForgeTemplateHelper<unsigned long long>::PerformOp(L, std::divides()); }
template<> inline int ForgeTemplate<unsigned long long>::Mod(lua_State* L) { return ForgeTemplateHelper<unsigned long long>::PerformOp(L, std::modulus()); }
template<> inline int ForgeTemplate<unsigned long long>::Equal(lua_State* L) { return ForgeTemplateHelper<unsigned long long>::PerformOp(L, std::equal_to()); }
template<> inline int ForgeTemplate<unsigned long long>::Less(lua_State* L) { return ForgeTemplateHelper<unsigned long long>::PerformOp(L, std::less()); }
template<> inline int ForgeTemplate<unsigned long long>::LessOrEqual(lua_State* L) { return ForgeTemplateHelper<unsigned long long>::PerformOp(L, std::less_equal()); }
template<> inline int ForgeTemplate<unsigned long long>::ToString(lua_State* L) { return ForgeTemplateHelper<unsigned long long>::ToString(L); }
template<> inline int ForgeTemplate<unsigned long long>::Pow(lua_State* L) { return ForgeTemplateHelper<unsigned long long>::Pow(L); }

template<> inline int ForgeTemplate<long long>::Add(lua_State* L) { return ForgeTemplateHelper<long long>::PerformOp(L, std::plus()); }
template<> inline int ForgeTemplate<long long>::Subtract(lua_State* L) { return ForgeTemplateHelper<long long>::PerformOp(L, std::minus()); }
template<> inline int ForgeTemplate<long long>::Multiply(lua_State* L) { return ForgeTemplateHelper<long long>::PerformOp(L, std::multiplies()); }
template<> inline int ForgeTemplate<long long>::Divide(lua_State* L) { return ForgeTemplateHelper<long long>::PerformOp(L, std::divides()); }
template<> inline int ForgeTemplate<long long>::Mod(lua_State* L) { return ForgeTemplateHelper<long long>::PerformOp(L, std::modulus()); }
template<> inline int ForgeTemplate<long long>::UnaryMinus(lua_State* L) { return ForgeTemplateHelper<long long>::PerformOp(L, std::negate()); }
template<> inline int ForgeTemplate<long long>::Equal(lua_State* L) { return ForgeTemplateHelper<long long>::PerformOp(L, std::equal_to()); }
template<> inline int ForgeTemplate<long long>::Less(lua_State* L) { return ForgeTemplateHelper<long long>::PerformOp(L, std::less()); }
template<> inline int ForgeTemplate<long long>::LessOrEqual(lua_State* L) { return ForgeTemplateHelper<long long>::PerformOp(L, std::less_equal()); }
template<> inline int ForgeTemplate<long long>::ToString(lua_State* L) { return ForgeTemplateHelper<long long>::ToString(L); }
template<> inline int ForgeTemplate<long long>::Pow(lua_State* L) { return ForgeTemplateHelper<long long>::Pow(L); }

template<> inline int ForgeTemplate<ObjectGuid>::Equal(lua_State* L) { Forge* F = Forge::GetForge(L); F->Push(F->CHECKVAL<ObjectGuid>(1) == F->CHECKVAL<ObjectGuid>(2)); return 1; }
template<> inline int ForgeTemplate<ObjectGuid>::ToString(lua_State* L)
{
    Forge* F = Forge::GetForge(L);
    F->Push(F->CHECKVAL<ObjectGuid>(1).ToString());
    return 1;
}
