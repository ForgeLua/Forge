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

#if defined TRACKABLE_PTR_NAMESPACE
ForgeConstrainedObjectRef<Aura> GetWeakPtrFor(Aura const* obj)
{
#if defined FORGE_TRINITY
    Map* map = obj->GetOwner()->GetMap();
#elif defined FORGE_CMANGOS
    Map* map = obj->GetTarget()->GetMap();
#endif
    return { obj->GetWeakPtr(), map };
}
ForgeConstrainedObjectRef<BattleGround> GetWeakPtrFor(BattleGround const* obj) { return { obj->GetWeakPtr(), obj->GetBgMap() }; }
ForgeConstrainedObjectRef<Group> GetWeakPtrFor(Group const* obj) { return { obj->GetWeakPtr(), nullptr }; }
ForgeConstrainedObjectRef<Guild> GetWeakPtrFor(Guild const* obj) { return { obj->GetWeakPtr(), nullptr }; }
ForgeConstrainedObjectRef<Map> GetWeakPtrFor(Map const* obj) { return { obj->GetWeakPtr(), obj }; }
ForgeConstrainedObjectRef<Object> GetWeakPtrForObjectImpl(Object const* obj)
{
    if (obj->isType(TYPEMASK_WORLDOBJECT))
        return { obj->GetWeakPtr(), static_cast<WorldObject const*>(obj)->GetMap() };

    if (obj->GetTypeId() == TYPEID_ITEM)
        if (Player const* player = static_cast<Item const*>(obj)->GetOwner())
            return { obj->GetWeakPtr(), player->GetMap() };

    // possibly dangerous item
    return { obj->GetWeakPtr(), nullptr };
}
ForgeConstrainedObjectRef<Quest> GetWeakPtrFor(Quest const* obj) { return { obj->GetWeakPtr(), nullptr }; }
ForgeConstrainedObjectRef<Spell> GetWeakPtrFor(Spell const* obj) { return { obj->GetWeakPtr(), obj->GetCaster()->GetMap() }; }
#if FORGE_EXPANSION >= EXP_WOTLK
ForgeConstrainedObjectRef<Vehicle> GetWeakPtrFor(Vehicle const* obj)
{
#if defined FORGE_TRINITY
    Map* map = obj->GetBase()->GetMap();
#elif defined FORGE_CMANGOS
    Map* map = obj->GetOwner()->GetMap();
#endif
    return { obj->GetWeakPtr(), map };
}
#endif
#endif

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
#if defined FORGE_TRINITY
    F->Push(F->CHECKVAL<ObjectGuid>(1).ToString());
#else
    F->Push(F->CHECKVAL<ObjectGuid>(1).GetString());
#endif
    return 1;
}
