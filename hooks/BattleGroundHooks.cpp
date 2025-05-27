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
#include "ForgeTemplate.h"

using namespace Hooks;

#define START_HOOK(EVENT) \
    auto key = EventKey<BGEvents>(EVENT);\
    if (!BGEventBindings->HasBindingsFor(key))\
        return;

void Forge::OnBGStart(BattleGround* bg, BattleGroundTypeId bgId, uint32 instanceId)
{
    START_HOOK(BG_EVENT_ON_START);
    HookPush(bg);
    HookPush(bgId);
    HookPush(instanceId);
    CallAllFunctions(BGEventBindings, key);
}

void Forge::OnBGEnd(BattleGround* bg, BattleGroundTypeId bgId, uint32 instanceId, Team winner)
{
    START_HOOK(BG_EVENT_ON_END);
    HookPush(bg);
    HookPush(bgId);
    HookPush(instanceId);
    HookPush(winner);
    CallAllFunctions(BGEventBindings, key);
}

void Forge::OnBGCreate(BattleGround* bg, BattleGroundTypeId bgId, uint32 instanceId)
{
    START_HOOK(BG_EVENT_ON_CREATE);
    HookPush(bg);
    HookPush(bgId);
    HookPush(instanceId);
    CallAllFunctions(BGEventBindings, key);
}

void Forge::OnBGDestroy(BattleGround* bg, BattleGroundTypeId bgId, uint32 instanceId)
{
    START_HOOK(BG_EVENT_ON_PRE_DESTROY);
    HookPush(bg);
    HookPush(bgId);
    HookPush(instanceId);
    CallAllFunctions(BGEventBindings, key);
}
