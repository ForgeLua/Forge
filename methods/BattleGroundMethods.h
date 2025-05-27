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

#ifndef BATTLEGROUNDMETHODS_H
#define BATTLEGROUNDMETHODS_H

/***
 * Contains the state of a battleground, e.g. Warsong Gulch, Arathi Basin, etc.
 *
 * Inherits all methods from: none
 */
namespace LuaBattleGround
{
    /**
     * Returns the name of the [BattleGround].
     *
     * @return string name
     */
    int GetName(Forge* F, BattleGround* bg)
    {
        F->Push(bg->GetName());
        return 1;
    }

    /**
     * Returns the amount of alive players in the [BattleGround] by the team ID.
     *
     * @param [Team] team : team ID
     * @return uint32 count
     */
    int GetAlivePlayersCountByTeam(Forge* F, BattleGround* bg)
    {
        uint32 team = F->CHECKVAL<uint32>(2);

        F->Push(bg->GetAlivePlayersCountByTeam((Team)team));
        return 1;
    }

    /**
     * Returns the [Map] of the [BattleGround].
     *
     * @return [Map] map
     */
    int GetMap(Forge* F, BattleGround* bg)
    {
        F->Push(static_cast<Map*>(bg->GetBgMap()));
        return 1;
    }

    /**
     * Returns the bonus honor given by amount of kills in the specific [BattleGround].
     *
     * @param uint32 kills : amount of kills
     * @return uint32 bonusHonor
     */
    int GetBonusHonorFromKillCount(Forge* F, BattleGround* bg)
    {
        uint32 kills = F->CHECKVAL<uint32>(2);

        F->Push(bg->GetBonusHonorFromKill(kills));
        return 1;
    }

    /**
     * Returns the bracket ID of the specific [BattleGround].
     *
     * @return [BattleGroundBracketId] bracketId
     */
    int GetBracketId(Forge* F, BattleGround* bg)
    {
        F->Push(bg->GetBracketId());
        return 1;
    }

    /**
     * Returns the end time of the [BattleGround].
     *
     * @return uint32 endTime
     */
    int GetEndTime(Forge* F, BattleGround* bg)
    {
        F->Push(bg->GetEndTime());
        return 1;
    }

    /**
     * Returns the amount of free slots for the selected team in the specific [BattleGround].
     *
     * @param [Team] team : team ID
     * @return uint32 freeSlots
     */
    int GetFreeSlotsForTeam(Forge* F, BattleGround* bg)
    {
        uint32 team = F->CHECKVAL<uint32>(2);

        F->Push(bg->GetFreeSlotsForTeam((Team)team));
        return 1;
    }

    /**
     * Returns the instance ID of the [BattleGround].
     *
     * @return uint32 instanceId
     */
    int GetInstanceId(Forge* F, BattleGround* bg)
    {
        F->Push(bg->GetInstanceID());
        return 1;
    }

    /**
     * Returns the map ID of the [BattleGround].
     *
     * @return uint32 mapId
     */
    int GetMapId(Forge* F, BattleGround* bg)
    {
        F->Push(bg->GetMapId());
        return 1;
    }

    /**
     * Returns the type ID of the [BattleGround].
     *
     * @return [BattleGroundTypeId] typeId
     */
    int GetTypeId(Forge* F, BattleGround* bg)
    {
        F->Push(bg->GetTypeID());
        return 1;
    }

    /**
     * Returns the max allowed [Player] level of the specific [BattleGround].
     *
     * @return uint32 maxLevel
     */
    int GetMaxLevel(Forge* F, BattleGround* bg)
    {
        F->Push(bg->GetMaxLevel());
        return 1;
    }

    /**
     * Returns the minimum allowed [Player] level of the specific [BattleGround].
     *
     * @return uint32 minLevel
     */
    int GetMinLevel(Forge* F, BattleGround* bg)
    {
        F->Push(bg->GetMinLevel());
        return 1;
    }

    /**
     * Returns the maximum allowed [Player] count of the specific [BattleGround].
     *
     * @return uint32 maxPlayerCount
     */
    int GetMaxPlayers(Forge* F, BattleGround* bg)
    {
        F->Push(bg->GetMaxPlayers());
        return 1;
    }

    /**
     * Returns the minimum allowed [Player] count of the specific [BattleGround].
     *
     * @return uint32 minPlayerCount
     */
    int GetMinPlayers(Forge* F, BattleGround* bg)
    {
        F->Push(bg->GetMinPlayers());
        return 1;
    }

    /**
     * Returns the maximum allowed [Player] count per team of the specific [BattleGround].
     *
     * @return uint32 maxTeamPlayerCount
     */
    int GetMaxPlayersPerTeam(Forge* F, BattleGround* bg)
    {
        F->Push(bg->GetMaxPlayersPerTeam());
        return 1;
    }

    /**
     * Returns the minimum allowed [Player] count per team of the specific [BattleGround].
     *
     * @return uint32 minTeamPlayerCount
     */
    int GetMinPlayersPerTeam(Forge* F, BattleGround* bg)
    {
        F->Push(bg->GetMinPlayersPerTeam());
        return 1;
    }

    /**
     * Returns the winning team of the specific [BattleGround].
     *
     * @return [Team] team
     */
    int GetWinner(Forge* F, BattleGround* bg)
    {
        F->Push(bg->GetWinner());
        return 1;
    }

    /**
     * Returns the status of the specific [BattleGround].
     *
     * @return [BattleGroundStatus] status
     */
    int GetStatus(Forge* F, BattleGround* bg)
    {
        F->Push(bg->GetStatus());
        return 1;
    }
    
    ForgeRegister<BattleGround> BattleGroundMethods[] =
    {
        // Getters
        { "GetName", &LuaBattleGround::GetName },
        { "GetAlivePlayersCountByTeam", &LuaBattleGround::GetAlivePlayersCountByTeam },
        { "GetMap", &LuaBattleGround::GetMap },
        { "GetBonusHonorFromKillCount", &LuaBattleGround::GetBonusHonorFromKillCount },
        { "GetBracketId", &LuaBattleGround::GetBracketId },
        { "GetEndTime", &LuaBattleGround::GetEndTime },
        { "GetFreeSlotsForTeam", &LuaBattleGround::GetFreeSlotsForTeam },
        { "GetInstanceId", &LuaBattleGround::GetInstanceId },
        { "GetMapId", &LuaBattleGround::GetMapId },
        { "GetTypeId", &LuaBattleGround::GetTypeId },
        { "GetMaxLevel", &LuaBattleGround::GetMaxLevel },
        { "GetMinLevel", &LuaBattleGround::GetMinLevel },
        { "GetMaxPlayers", &LuaBattleGround::GetMaxPlayers },
        { "GetMinPlayers", &LuaBattleGround::GetMinPlayers },
        { "GetMaxPlayersPerTeam", &LuaBattleGround::GetMaxPlayersPerTeam },
        { "GetMinPlayersPerTeam", &LuaBattleGround::GetMinPlayersPerTeam },
        { "GetWinner", &LuaBattleGround::GetWinner },
        { "GetStatus", &LuaBattleGround::GetStatus }
    };
};
#endif
