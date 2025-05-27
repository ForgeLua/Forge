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

#ifndef GUILDMETHODS_H
#define GUILDMETHODS_H

/***
 * Inherits all methods from: none
 */
namespace LuaGuild
{
    /**
     * Returns a table with the [Player]s in this [Guild]
     *
     * Only the players that are online and on some map.
     *
     * In multistate, this method is only available in the WORLD state
     *
     * @return table guildPlayers : table of [Player]s
     */
    int GetMembers(Forge* F, Guild* guild)
    {
        lua_newtable(F->L);
        int tbl = lua_gettop(F->L);
        uint32 i = 0;

        std::shared_lock<std::shared_mutex> lock(*HashMapHolder<Player>::GetLock());
        const HashMapHolder<Player>::MapType& m = eObjectAccessor()GetPlayers();
        for (HashMapHolder<Player>::MapType::const_iterator it = m.begin(); it != m.end(); ++it)
        {
            if (Player* player = it->second)
            {
                if (player->IsInWorld() && player->GetGuildId() == guild->GetId())
                {
                    F->Push(player);
                    lua_rawseti(F->L, tbl, ++i);
                }
            }
        }

        lua_settop(F->L, tbl); // push table to top of stack
        return 1;
    }

    /**
     * Returns the member count of this [Guild]
     *
     * @return uint32 memberCount
     */
    int GetMemberCount(Forge* F, Guild* guild)
    {
        F->Push(guild->GetMemberCount());
        return 1;
    }

    /**
     * Finds and returns the [Guild] leader by their GUID if logged in
     *
     * In multistate, this method is only available in the WORLD state
     *
     * @return [Player] leader
     */
    int GetLeader(Forge* F, Guild* guild)
    {
        F->Push(eObjectAccessor()FindPlayer(guild->GetLeaderGUID()));
        return 1;
    }

    /**
     * Returns [Guild] leader GUID
     *
     * @return ObjectGuid leaderGUID
     */
    int GetLeaderGUID(Forge* F, Guild* guild)
    {
        F->Push(guild->GetLeaderGUID());
        return 1;
    }

    /**
     * Returns the [Guild]s entry ID
     *
     * @return uint32 entryId
     */
    int GetId(Forge* F, Guild* guild)
    {
        F->Push(guild->GetId());
        return 1;
    }

    /**
     * Returns the [Guild]s name
     *
     * @return string guildName
     */
    int GetName(Forge* F, Guild* guild)
    {
        F->Push(guild->GetName());
        return 1;
    }

    /**
     * Returns the [Guild]s current Message Of The Day
     *
     * @return string guildMOTD
     */
    int GetMOTD(Forge* F, Guild* guild)
    {
        F->Push(guild->GetMOTD());
        return 1;
    }

    /**
     * Returns the [Guild]s current info
     *
     * @return string guildInfo
     */
    int GetInfo(Forge* F, Guild* guild)
    {
        F->Push(guild->GetInfo());
        return 1;
    }

    /**
     * Sets the leader of this [Guild]
     *
     * In multistate, this method is only available in the WORLD state
     *
     * @param [Player] leader : the [Player] leader to change
     */
    int SetLeader(Forge* F, Guild* guild)
    {
        Player* player = F->CHECKOBJ<Player>(2);

        guild->HandleSetLeader(player->GetSession(), player->GetName());
        return 0;
    }

    /**
     * Sets the information of the bank tab specified
     *
     * In multistate, this method is only available in the WORLD state
     *
     * @param uint8 tabId : the ID of the tab specified
     * @param string info : the information to be set to the bank tab
     */
    int SetBankTabText(Forge* F, Guild* guild)
    {
        uint8 tabId = F->CHECKVAL<uint8>(2);
        const char* text = F->CHECKVAL<const char*>(3);

        guild->SetBankTabText(tabId, text);
        return 0;
    }

    // SendPacketToGuild(packet)
    /**
     * Sends a [WorldPacket] to all the [Player]s in the [Guild]
     *
     * @param [WorldPacket] packet : the [WorldPacket] to be sent to the [Player]s
     */
    int SendPacket(Forge* F, Guild* guild)
    {
        WorldPacket* data = F->CHECKOBJ<WorldPacket>(2);

        guild->BroadcastPacket(data);
        return 0;
    }

    // SendPacketToRankedInGuild(packet, rankId)
    /**
     * Sends a [WorldPacket] to all the [Player]s at the specified rank in the [Guild]
     *
     * @param [WorldPacket] packet : the [WorldPacket] to be sent to the [Player]s
     * @param uint8 rankId : the rank ID
     */
    int SendPacketToRanked(Forge* F, Guild* guild)
    {
        WorldPacket* data = F->CHECKOBJ<WorldPacket>(2);
        uint8 ranked = F->CHECKVAL<uint8>(3);

        guild->BroadcastPacketToRank(data, ranked);
        return 0;
    }

    /**
     * Disbands the [Guild]
     *
     * In multistate, this method is only available in the WORLD state
     *
     */
    int Disband(Forge* /*E*/, Guild* guild)
    {
        guild->Disband();
        return 0;
    }

    /**
     * Adds the specified [Player] to the [Guild] at the specified rank.
     *
     * If no rank is specified, defaults to none.
     *
     * In multistate, this method is only available in the WORLD state
     *
     * @param [Player] player : the [Player] to be added to the guild
     * @param uint8 rankId : the rank ID
     */
    int AddMember(Forge* F, Guild* guild)
    {
        Player* player = F->CHECKOBJ<Player>(2);
        uint8 rankId = F->CHECKVAL<uint8>(3, GUILD_RANK_NONE);

        CharacterDatabaseTransaction trans(nullptr);

        guild->AddMember(trans, player->GET_GUID(), rankId);
        return 0;
    }

    /**
     * Removes the specified [Player] from the [Guild].
     *
     * In multistate, this method is only available in the WORLD state
     *
     * @param [Player] player : the [Player] to be removed from the guild
     * @param bool isDisbanding : default 'false', should only be set to 'true' if the guild is triggered to disband
     */
    int DeleteMember(Forge* F, Guild* guild)
    {
        Player* player = F->CHECKOBJ<Player>(2);
        bool isDisbanding = F->CHECKVAL<bool>(3, false);

        CharacterDatabaseTransaction trans(nullptr);

        guild->DeleteMember(trans, player->GET_GUID(), isDisbanding);
        return 0;
    }

    /**
     * Promotes/demotes the [Player] to the specified rank.
     *
     * In multistate, this method is only available in the WORLD state
     *
     * @param [Player] player : the [Player] to be promoted/demoted
     * @param uint8 rankId : the rank ID
     */
    int SetMemberRank(Forge* F, Guild* guild)
    {
        Player* player = F->CHECKOBJ<Player>(2);
        uint8 newRank = F->CHECKVAL<uint8>(3);

        CharacterDatabaseTransaction trans(nullptr);

        guild->ChangeMemberRank(trans, player->GET_GUID(), newRank);
        return 0;
    }
    
    ForgeRegister<Guild> GuildMethods[] =
    {
        // Getters
        { "GetMembers", &LuaGuild::GetMembers, METHOD_REG_WORLD }, // World state method only in multistate
        { "GetLeader", &LuaGuild::GetLeader, METHOD_REG_WORLD }, // World state method only in multistate
        { "GetLeaderGUID", &LuaGuild::GetLeaderGUID },
        { "GetId", &LuaGuild::GetId },
        { "GetName", &LuaGuild::GetName },
        { "GetMOTD", &LuaGuild::GetMOTD },
        { "GetInfo", &LuaGuild::GetInfo },
        { "GetMemberCount", &LuaGuild::GetMemberCount },

        // Setters
        { "SetBankTabText", &LuaGuild::SetBankTabText, METHOD_REG_WORLD }, // World state method only in multistate
        { "SetMemberRank", &LuaGuild::SetMemberRank, METHOD_REG_WORLD }, // World state method only in multistate
        { "SetLeader", &LuaGuild::SetLeader, METHOD_REG_WORLD }, // World state method only in multistate

        // Other
        { "SendPacket", &LuaGuild::SendPacket },
        { "SendPacketToRanked", &LuaGuild::SendPacketToRanked },
        { "Disband", &LuaGuild::Disband, METHOD_REG_WORLD }, // World state method only in multistate
        { "AddMember", &LuaGuild::AddMember, METHOD_REG_WORLD }, // World state method only in multistate
        { "DeleteMember", &LuaGuild::DeleteMember, METHOD_REG_WORLD } // World state method only in multistate
    };
};
#endif
