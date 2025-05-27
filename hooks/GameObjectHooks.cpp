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
#include "ForgeEventMgr.h"
#include "ForgeTemplate.h"

using namespace Hooks;

#define START_HOOK(EVENT, ENTRY) \
    auto key = EntryKey<GameObjectEvents>(EVENT, ENTRY);\
    if (!GameObjectEventBindings->HasBindingsFor(key))\
        return;

#define START_HOOK_WITH_RETVAL(EVENT, ENTRY, RETVAL) \
    auto key = EntryKey<GameObjectEvents>(EVENT, ENTRY);\
    if (!GameObjectEventBindings->HasBindingsFor(key))\
        return RETVAL;

void Forge::OnDummyEffect(WorldObject* pCaster, uint32 spellId, SpellEffIndex effIndex, GameObject* pTarget)
{
    START_HOOK(GAMEOBJECT_EVENT_ON_DUMMY_EFFECT, pTarget->GetEntry());
    HookPush(pCaster);
    HookPush(spellId);
    HookPush(effIndex);
    HookPush(pTarget);
    CallAllFunctions(GameObjectEventBindings, key);
}

void Forge::UpdateAI(GameObject* pGameObject, uint32 diff)
{
    START_HOOK(GAMEOBJECT_EVENT_ON_AIUPDATE, pGameObject->GetEntry());
    HookPush(pGameObject);
    HookPush(diff);
    CallAllFunctions(GameObjectEventBindings, key);
}

bool Forge::OnQuestAccept(Player* pPlayer, GameObject* pGameObject, Quest const* pQuest)
{
    START_HOOK_WITH_RETVAL(GAMEOBJECT_EVENT_ON_QUEST_ACCEPT, pGameObject->GetEntry(), false);
    HookPush(pPlayer);
    HookPush(pGameObject);
    HookPush(pQuest);
    return CallAllFunctionsBool(GameObjectEventBindings, key);
}

bool Forge::OnQuestReward(Player* pPlayer, GameObject* pGameObject, Quest const* pQuest, uint32 opt)
{
    START_HOOK_WITH_RETVAL(GAMEOBJECT_EVENT_ON_QUEST_REWARD, pGameObject->GetEntry(), false);
    HookPush(pPlayer);
    HookPush(pGameObject);
    HookPush(pQuest);
    HookPush(opt);
    return CallAllFunctionsBool(GameObjectEventBindings, key);
}

void Forge::GetDialogStatus(const Player* pPlayer, const GameObject* pGameObject)
{
    START_HOOK(GAMEOBJECT_EVENT_ON_DIALOG_STATUS, pGameObject->GetEntry());
    HookPush(pPlayer);
    HookPush(pGameObject);
    CallAllFunctions(GameObjectEventBindings, key);
}

#if FORGE_EXPANSION >= EXP_WOTLK
void Forge::OnDestroyed(GameObject* pGameObject, WorldObject* attacker)
{
    START_HOOK(GAMEOBJECT_EVENT_ON_DESTROYED, pGameObject->GetEntry());
    HookPush(pGameObject);
    HookPush(attacker);
    CallAllFunctions(GameObjectEventBindings, key);
}

void Forge::OnDamaged(GameObject* pGameObject, WorldObject* attacker)
{
    START_HOOK(GAMEOBJECT_EVENT_ON_DAMAGED, pGameObject->GetEntry());
    HookPush(pGameObject);
    HookPush(attacker);
    CallAllFunctions(GameObjectEventBindings, key);
}
#endif

void Forge::OnLootStateChanged(GameObject* pGameObject, uint32 state)
{
    START_HOOK(GAMEOBJECT_EVENT_ON_LOOT_STATE_CHANGE, pGameObject->GetEntry());
    HookPush(pGameObject);
    HookPush(state);
    CallAllFunctions(GameObjectEventBindings, key);
}

void Forge::OnGameObjectStateChanged(GameObject* pGameObject, uint32 state)
{
    START_HOOK(GAMEOBJECT_EVENT_ON_GO_STATE_CHANGED, pGameObject->GetEntry());
    HookPush(pGameObject);
    HookPush(state);
    CallAllFunctions(GameObjectEventBindings, key);
}

void Forge::OnSpawn(GameObject* pGameObject)
{
    START_HOOK(GAMEOBJECT_EVENT_ON_SPAWN, pGameObject->GetEntry());
    HookPush(pGameObject);
    CallAllFunctions(GameObjectEventBindings, key);
}

void Forge::OnAddToWorld(GameObject* pGameObject)
{
    START_HOOK(GAMEOBJECT_EVENT_ON_ADD, pGameObject->GetEntry());
    HookPush(pGameObject);
    CallAllFunctions(GameObjectEventBindings, key);
}

void Forge::OnRemoveFromWorld(GameObject* pGameObject)
{
    START_HOOK(GAMEOBJECT_EVENT_ON_REMOVE, pGameObject->GetEntry());
    HookPush(pGameObject);
    CallAllFunctions(GameObjectEventBindings, key);
}

bool Forge::OnGameObjectUse(Player* pPlayer, GameObject* pGameObject)
{
    START_HOOK_WITH_RETVAL(GAMEOBJECT_EVENT_ON_USE, pGameObject->GetEntry(), false);
    HookPush(pGameObject);
    HookPush(pPlayer);
    return CallAllFunctionsBool(GameObjectEventBindings, key);
}
