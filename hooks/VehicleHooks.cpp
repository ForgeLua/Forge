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

#if FORGE_EXPANSION >= EXP_WOTLK

using namespace Hooks;

#define START_HOOK(EVENT) \
    auto key = EventKey<VehicleEvents>(EVENT);\
    if (!VehicleEventBindings->HasBindingsFor(key))\
        return;

void Forge::OnInstall(Vehicle* vehicle)
{
    START_HOOK(VEHICLE_EVENT_ON_INSTALL);
    HookPush(vehicle);
    CallAllFunctions(VehicleEventBindings, key);
}

void Forge::OnUninstall(Vehicle* vehicle)
{
    START_HOOK(VEHICLE_EVENT_ON_UNINSTALL);
    HookPush(vehicle);
    CallAllFunctions(VehicleEventBindings, key);
}

void Forge::OnInstallAccessory(Vehicle* vehicle, Creature* accessory)
{
    START_HOOK(VEHICLE_EVENT_ON_INSTALL_ACCESSORY);
    HookPush(vehicle);
    HookPush(accessory);
    CallAllFunctions(VehicleEventBindings, key);
}

void Forge::OnAddPassenger(Vehicle* vehicle, Unit* passenger, int8 seatId)
{
    START_HOOK(VEHICLE_EVENT_ON_ADD_PASSENGER);
    HookPush(vehicle);
    HookPush(passenger);
    HookPush(seatId);
    CallAllFunctions(VehicleEventBindings, key);
}

void Forge::OnRemovePassenger(Vehicle* vehicle, Unit* passenger)
{
    START_HOOK(VEHICLE_EVENT_ON_REMOVE_PASSENGER);
    HookPush(vehicle);
    HookPush(passenger);
    CallAllFunctions(VehicleEventBindings, key);
}

#endif
