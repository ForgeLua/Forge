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
#include "ForgeEventMgr.h"
#include "ForgeIncludes.h"
#include "ForgeTemplate.h"
#include "ForgeUtility.h"

// Method includes
#include "GlobalMethods.h"
#include "ObjectMethods.h"
#include "WorldObjectMethods.h"
#include "UnitMethods.h"
#include "PlayerMethods.h"
#include "CreatureMethods.h"
#include "GroupMethods.h"
#include "GuildMethods.h"
#include "GameObjectMethods.h"
#include "ForgeQueryMethods.h"
#include "AuraMethods.h"
#include "ItemMethods.h"
#include "WorldPacketMethods.h"
#include "SpellMethods.h"
#include "QuestMethods.h"
#include "MapMethods.h"
#include "CorpseMethods.h"
#include "VehicleMethods.h"
#include "BattleGroundMethods.h"

// Custom methods
#include "CustomMethods.h"

void RegisterMethods(Forge* F)
{
    ForgeTemplate<>::SetMethods(F, LuaGlobalFunctions::GlobalMethods);

    ForgeTemplate<Object>::Register(F, "Object");
    ForgeTemplate<Object>::SetMethods(F, LuaObject::ObjectMethods);

    ForgeTemplate<WorldObject>::Register(F, "WorldObject");
    ForgeTemplate<WorldObject>::SetMethods(F, LuaObject::ObjectMethods);
    ForgeTemplate<WorldObject>::SetMethods(F, LuaWorldObject::WorldObjectMethods);

    ForgeTemplate<Unit>::Register(F, "Unit");
    ForgeTemplate<Unit>::SetMethods(F, LuaObject::ObjectMethods);
    ForgeTemplate<Unit>::SetMethods(F, LuaWorldObject::WorldObjectMethods);
    ForgeTemplate<Unit>::SetMethods(F, LuaUnit::UnitMethods);

    ForgeTemplate<Player>::Register(F, "Player");
    ForgeTemplate<Player>::SetMethods(F, LuaObject::ObjectMethods);
    ForgeTemplate<Player>::SetMethods(F, LuaWorldObject::WorldObjectMethods);
    ForgeTemplate<Player>::SetMethods(F, LuaUnit::UnitMethods);
    ForgeTemplate<Player>::SetMethods(F, LuaPlayer::PlayerMethods);

    ForgeTemplate<Creature>::Register(F, "Creature");
    ForgeTemplate<Creature>::SetMethods(F, LuaObject::ObjectMethods);
    ForgeTemplate<Creature>::SetMethods(F, LuaWorldObject::WorldObjectMethods);
    ForgeTemplate<Creature>::SetMethods(F, LuaUnit::UnitMethods);
    ForgeTemplate<Creature>::SetMethods(F, LuaCreature::CreatureMethods);

    ForgeTemplate<GameObject>::Register(F, "GameObject");
    ForgeTemplate<GameObject>::SetMethods(F, LuaObject::ObjectMethods);
    ForgeTemplate<GameObject>::SetMethods(F, LuaWorldObject::WorldObjectMethods);
    ForgeTemplate<GameObject>::SetMethods(F, LuaGameObject::GameObjectMethods);

    ForgeTemplate<Corpse>::Register(F, "Corpse");
    ForgeTemplate<Corpse>::SetMethods(F, LuaObject::ObjectMethods);
    ForgeTemplate<Corpse>::SetMethods(F, LuaWorldObject::WorldObjectMethods);
    ForgeTemplate<Corpse>::SetMethods(F, LuaCorpse::CorpseMethods);

    ForgeTemplate<Item>::Register(F, "Item");
    ForgeTemplate<Item>::SetMethods(F, LuaObject::ObjectMethods);
    ForgeTemplate<Item>::SetMethods(F, LuaItem::ItemMethods);

#if FORGE_EXPANSION >= EXP_WOTLK
    ForgeTemplate<Vehicle>::Register(F, "Vehicle");
    ForgeTemplate<Vehicle>::SetMethods(F, LuaVehicle::VehicleMethods);
#endif

    ForgeTemplate<Group>::Register(F, "Group");
    ForgeTemplate<Group>::SetMethods(F, LuaGroup::GroupMethods);

    ForgeTemplate<Guild>::Register(F, "Guild");
    ForgeTemplate<Guild>::SetMethods(F, LuaGuild::GuildMethods);

    ForgeTemplate<Aura>::Register(F, "Aura");
    ForgeTemplate<Aura>::SetMethods(F, LuaAura::AuraMethods);

    ForgeTemplate<Spell>::Register(F, "Spell");
    ForgeTemplate<Spell>::SetMethods(F, LuaSpell::SpellMethods);

    ForgeTemplate<Quest>::Register(F, "Quest");
    ForgeTemplate<Quest>::SetMethods(F, LuaQuest::QuestMethods);

    ForgeTemplate<Map>::Register(F, "Map");
    ForgeTemplate<Map>::SetMethods(F, LuaMap::MapMethods);

    ForgeTemplate<BattleGround>::Register(F, "BattleGround");
    ForgeTemplate<BattleGround>::SetMethods(F, LuaBattleGround::BattleGroundMethods);

    ForgeTemplate<WorldPacket>::Register(F, "WorldPacket");
    ForgeTemplate<WorldPacket>::SetMethods(F, LuaPacket::PacketMethods);

    ForgeTemplate<ForgeQuery>::Register(F, "ForgeQuery");
    ForgeTemplate<ForgeQuery>::SetMethods(F, LuaQuery::QueryMethods);

    ForgeTemplate<long long>::Register(F, "long long");

    ForgeTemplate<unsigned long long>::Register(F, "unsigned long long");

    ForgeTemplate<ObjectGuid>::Register(F, "ObjectGuid");

    // Register custom methods
    LuaCustom::RegisterCustomMethods(F);

    LuaVal::Register(F->L);
}
