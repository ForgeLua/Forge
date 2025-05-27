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

#include "Hooks.h"
#include "HookHelpers.h"
#include "LuaEngine.h"
#include "BindingMap.h"
#include "ForgeIncludes.h"
#include "ForgeTemplate.h"

using namespace Hooks;

#define START_HOOK(BINDINGS, EVENT, ENTRY) \
    auto key = EntryKey<GossipEvents>(EVENT, ENTRY);\
    if (!BINDINGS->HasBindingsFor(key))\
        return;

#define START_HOOK_WITH_RETVAL(BINDINGS, EVENT, ENTRY, RETVAL) \
    auto key = EntryKey<GossipEvents>(EVENT, ENTRY);\
    if (!BINDINGS->HasBindingsFor(key))\
        return RETVAL;

bool Forge::OnGossipHello(Player* pPlayer, GameObject* pGameObject)
{
    START_HOOK_WITH_RETVAL(GameObjectGossipBindings, GOSSIP_EVENT_ON_HELLO, pGameObject->GetEntry(), false);
#if defined FORGE_CMANGOS && FORGE_EXPANSION < EXP_CATA
    pPlayer->GetPlayerMenu()->ClearMenus();
#else
    pPlayer->PlayerTalkClass->ClearMenus();
#endif
    HookPush(pPlayer);
    HookPush(pGameObject);
    return CallAllFunctionsBool(GameObjectGossipBindings, key, true);
}

bool Forge::OnGossipSelect(Player* pPlayer, GameObject* pGameObject, uint32 sender, uint32 action)
{
    START_HOOK_WITH_RETVAL(GameObjectGossipBindings, GOSSIP_EVENT_ON_SELECT, pGameObject->GetEntry(), false);
#if defined FORGE_CMANGOS && FORGE_EXPANSION < EXP_CATA
    pPlayer->GetPlayerMenu()->ClearMenus();
#else
    pPlayer->PlayerTalkClass->ClearMenus();
#endif
    HookPush(pPlayer);
    HookPush(pGameObject);
    HookPush(sender);
    HookPush(action);
    return CallAllFunctionsBool(GameObjectGossipBindings, key, true);
}

bool Forge::OnGossipSelectCode(Player* pPlayer, GameObject* pGameObject, uint32 sender, uint32 action, const char* code)
{
    START_HOOK_WITH_RETVAL(GameObjectGossipBindings, GOSSIP_EVENT_ON_SELECT, pGameObject->GetEntry(), false);
#if defined FORGE_CMANGOS && FORGE_EXPANSION < EXP_CATA
    pPlayer->GetPlayerMenu()->ClearMenus();
#else
    pPlayer->PlayerTalkClass->ClearMenus();
#endif
    HookPush(pPlayer);
    HookPush(pGameObject);
    HookPush(sender);
    HookPush(action);
    HookPush(code);
    return CallAllFunctionsBool(GameObjectGossipBindings, key, true);
}

void Forge::HandleGossipSelectOption(Player* pPlayer, uint32 menuId, uint32 sender, uint32 action, const std::string& code)
{
    START_HOOK(PlayerGossipBindings, GOSSIP_EVENT_ON_SELECT, menuId);
#if defined FORGE_CMANGOS && FORGE_EXPANSION < EXP_CATA
    pPlayer->GetPlayerMenu()->ClearMenus();
#else
    pPlayer->PlayerTalkClass->ClearMenus();
#endif

    HookPush(pPlayer); // receiver
    HookPush(pPlayer); // sender, just not to mess up the amount of args.
    HookPush(sender);
    HookPush(action);
    if (code.empty())
        HookPush();
    else
        HookPush(code);

    CallAllFunctions(PlayerGossipBindings, key);
}

bool Forge::OnItemGossip(Player* pPlayer, Item* pItem, SpellCastTargets const& /*targets*/)
{
    START_HOOK_WITH_RETVAL(ItemGossipBindings, GOSSIP_EVENT_ON_HELLO, pItem->GetEntry(), true);
#if defined FORGE_CMANGOS && FORGE_EXPANSION < EXP_CATA
    pPlayer->GetPlayerMenu()->ClearMenus();
#else
    pPlayer->PlayerTalkClass->ClearMenus();
#endif
    HookPush(pPlayer);
    HookPush(pItem);
    return CallAllFunctionsBool(ItemGossipBindings, key, true);
}

void Forge::HandleGossipSelectOption(Player* pPlayer, Item* pItem, uint32 sender, uint32 action, const std::string& code)
{
    START_HOOK(ItemGossipBindings, GOSSIP_EVENT_ON_SELECT, pItem->GetEntry());
#if defined FORGE_CMANGOS && FORGE_EXPANSION < EXP_CATA
    pPlayer->GetPlayerMenu()->ClearMenus();
#else
    pPlayer->PlayerTalkClass->ClearMenus();
#endif

    HookPush(pPlayer);
    HookPush(pItem);
    HookPush(sender);
    HookPush(action);
    if (code.empty())
        HookPush();
    else
        HookPush(code);

    CallAllFunctions(ItemGossipBindings, key);
}

bool Forge::OnGossipHello(Player* pPlayer, Creature* pCreature)
{
    START_HOOK_WITH_RETVAL(CreatureGossipBindings, GOSSIP_EVENT_ON_HELLO, pCreature->GetEntry(), false);
#if defined FORGE_CMANGOS && FORGE_EXPANSION < EXP_CATA
    pPlayer->GetPlayerMenu()->ClearMenus();
#else
    pPlayer->PlayerTalkClass->ClearMenus();
#endif
    HookPush(pPlayer);
    HookPush(pCreature);
    return CallAllFunctionsBool(CreatureGossipBindings, key, true);
}

bool Forge::OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action)
{
    START_HOOK_WITH_RETVAL(CreatureGossipBindings, GOSSIP_EVENT_ON_SELECT, pCreature->GetEntry(), false);
#if defined FORGE_CMANGOS && FORGE_EXPANSION < EXP_CATA
    auto original_menu = *pPlayer->GetPlayerMenu();
    pPlayer->GetPlayerMenu()->ClearMenus();
#else
    auto original_menu = *pPlayer->PlayerTalkClass;
    pPlayer->PlayerTalkClass->ClearMenus();
#endif
    HookPush(pPlayer);
    HookPush(pCreature);
    HookPush(sender);
    HookPush(action);
    auto preventDefault = CallAllFunctionsBool(CreatureGossipBindings, key, true);
    if (!preventDefault) {
#if defined FORGE_CMANGOS && FORGE_EXPANSION < EXP_CATA
        *pPlayer->GetPlayerMenu() = original_menu;
#else
        *pPlayer->PlayerTalkClass = original_menu;
#endif
    }
    return preventDefault;
}

bool Forge::OnGossipSelectCode(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action, const char* code)
{
    START_HOOK_WITH_RETVAL(CreatureGossipBindings, GOSSIP_EVENT_ON_SELECT, pCreature->GetEntry(), false);
#if defined FORGE_CMANGOS && FORGE_EXPANSION < EXP_CATA
    auto original_menu = *pPlayer->GetPlayerMenu();
    pPlayer->GetPlayerMenu()->ClearMenus();
#else
    auto original_menu = *pPlayer->PlayerTalkClass;
    pPlayer->PlayerTalkClass->ClearMenus();
#endif
    HookPush(pPlayer);
    HookPush(pCreature);
    HookPush(sender);
    HookPush(action);
    HookPush(code);
    auto preventDefault = CallAllFunctionsBool(CreatureGossipBindings, key, true);
    if (!preventDefault) {
#if defined FORGE_CMANGOS && FORGE_EXPANSION < EXP_CATA
        *pPlayer->GetPlayerMenu() = original_menu;
#else
        *pPlayer->PlayerTalkClass = original_menu;
#endif
    }
    return preventDefault;
}
