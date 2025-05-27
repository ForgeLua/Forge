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
#include "ForgeInstanceAI.h"

using namespace Hooks;

#define START_HOOK(EVENT, AI) \
    auto mapKey = EntryKey<InstanceEvents>(EVENT, AI->instance->GetId());\
    auto instanceKey = EntryKey<InstanceEvents>(EVENT, AI->instance->GetInstanceId());\
    if (!MapEventBindings->HasBindingsFor(mapKey) && !InstanceEventBindings->HasBindingsFor(instanceKey))\
        return;\
    PushInstanceData(AI);\
    HookPush<Map>(AI->instance)

#define START_HOOK_WITH_RETVAL(EVENT, AI, RETVAL) \
    auto mapKey = EntryKey<InstanceEvents>(EVENT, AI->instance->GetId());\
    auto instanceKey = EntryKey<InstanceEvents>(EVENT, AI->instance->GetInstanceId());\
    if (!MapEventBindings->HasBindingsFor(mapKey) && !InstanceEventBindings->HasBindingsFor(instanceKey))\
        return RETVAL;\
    PushInstanceData(AI);\
    HookPush<Map>(AI->instance)

void Forge::OnInitialize(ForgeInstanceAI* ai)
{
    START_HOOK(INSTANCE_EVENT_ON_INITIALIZE, ai);
    CallAllFunctions(MapEventBindings, InstanceEventBindings, mapKey, instanceKey);
}

void Forge::OnLoad(ForgeInstanceAI* ai)
{
    START_HOOK(INSTANCE_EVENT_ON_LOAD, ai);
    CallAllFunctions(MapEventBindings, InstanceEventBindings, mapKey, instanceKey);
}

void Forge::OnUpdateInstance(ForgeInstanceAI* ai, uint32 diff)
{
    START_HOOK(INSTANCE_EVENT_ON_UPDATE, ai);
    HookPush(diff);
    CallAllFunctions(MapEventBindings, InstanceEventBindings, mapKey, instanceKey);
}

void Forge::OnPlayerEnterInstance(ForgeInstanceAI* ai, Player* player)
{
    START_HOOK(INSTANCE_EVENT_ON_PLAYER_ENTER, ai);
    HookPush(player);
    CallAllFunctions(MapEventBindings, InstanceEventBindings, mapKey, instanceKey);
}

void Forge::OnCreatureCreate(ForgeInstanceAI* ai, Creature* creature)
{
    START_HOOK(INSTANCE_EVENT_ON_CREATURE_CREATE, ai);
    HookPush(creature);
    CallAllFunctions(MapEventBindings, InstanceEventBindings, mapKey, instanceKey);
}

void Forge::OnGameObjectCreate(ForgeInstanceAI* ai, GameObject* gameobject)
{
    START_HOOK(INSTANCE_EVENT_ON_GAMEOBJECT_CREATE, ai);
    HookPush(gameobject);
    CallAllFunctions(MapEventBindings, InstanceEventBindings, mapKey, instanceKey);
}

bool Forge::OnCheckEncounterInProgress(ForgeInstanceAI* ai)
{
    START_HOOK_WITH_RETVAL(INSTANCE_EVENT_ON_CHECK_ENCOUNTER_IN_PROGRESS, ai, false);
    return CallAllFunctionsBool(MapEventBindings, InstanceEventBindings, mapKey, instanceKey);
}
