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

#ifndef _FORGE_INCLUDES_H
#define _FORGE_INCLUDES_H

// Required
#include "AccountMgr.h"
#include "AuctionHouseMgr.h"
#include "Bag.h"
#include "Cell.h"
#include "CellImpl.h"
#include "Channel.h"
#include "Chat.h"
#include "DBCStores.h"
#include "GameEventMgr.h"
#include "GossipDef.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Group.h"
#include "Guild.h"
#include "GuildMgr.h"
#include "Language.h"
#include "Mail.h"
#include "MapManager.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "Pet.h"
#include "Player.h"
#include "ReputationMgr.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "TemporarySummon.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Battleground.h"
#include "Config.h"
#include "DatabaseEnv.h"
#include "GitRevision.h"
#include "GroupMgr.h"
#include "MiscPackets.h"
#include "MotionMaster.h"
#include "ScriptedCreature.h"
#include "SpellHistory.h"
#include "SpellInfo.h"
#include "WeatherMgr.h"
#include "ArenaTeam.h"
#include "Vehicle.h"
#include "World.h"


#include "GitRevision.h"
#include "revision_data.h"

typedef Opcodes OpcodesList;

/*
 * Note: if you add or change a CORE_NAME or CORE_VERSION #define,
 *   please update LuaGlobalFunctions::GetCoreName or LuaGlobalFunctions::GetCoreVersion documentation example string.
 */
#define CORE_NAME               "TrinityCore"
#define REGEN_TIME_FULL

#define CORE_VERSION            (GitRevision::GetFullVersion())
#define eWorld                  (sWorld)
#define eMapMgr                 (sMapMgr)
#define eGuildMgr               (sGuildMgr)
#define eObjectMgr              (sObjectMgr)
#define eAccountMgr             (sAccountMgr)
#define eAuctionMgr             (sAuctionMgr)
#define eGameEventMgr           (sGameEventMgr)
#define eObjectAccessor()       ObjectAccessor::

#endif // _FORGE_INCLUDES_H
