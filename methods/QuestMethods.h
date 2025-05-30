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

#ifndef QUESTMETHODS_H
#define QUESTMETHODS_H

/***
 * Inherits all methods from: none
 */
namespace LuaQuest
{
    /**
     * Returns 'true' if the [Quest] has the specified flag, false otherwise.
     * Below flags are based off of 3.3.5a. Subject to change.
     *
     * @table
     * @columns [QuestFlags, ID, Comment]
     * @values [QUEST_FLAGS_NONE, 0x0, ""]
     * @values [QUEST_FLAGS_STAY_ALIVE, 0x1, "Not used currently"]
     * @values [QUEST_FLAGS_PARTY_ACCEPT, 0x2, "Not used currently. If player in party, all players that can accept this quest will receive confirmation box to accept quest CMSG_QUEST_CONFIRM_ACCEPT/SMSG_QUEST_CONFIRM_ACCEPT"]
     * @values [QUEST_FLAGS_EXPLORATION, 0x4, "Not used currently"]
     * @values [QUEST_FLAGS_SHARABLE, 0x8, "Can be shared: Player::CanShareQuest()"]
     * @values [QUEST_FLAGS_HAS_CONDITION, 0x10, "Not used currently"]
     * @values [QUEST_FLAGS_HIDE_REWARD_POI, 0x20, "Not used currently: Unsure of content"]
     * @values [QUEST_FLAGS_RAID, 0x40, "Not used currently"]
     * @values [QUEST_FLAGS_TBC, 0x80, "Not used currently: Available if TBC expansion enabled only"]
     * @values [QUEST_FLAGS_NO_MONEY_FROM_XP, 0x100, "Not used currently: Experience is not converted to gold at max level"]
     * @values [QUEST_FLAGS_HIDDEN_REWARDS, 0x200, "Items and money rewarded only sent in SMSG_QUESTGIVER_OFFER_REWARD (not in SMSG_QUESTGIVER_QUEST_DETAILS or in client quest log(SMSG_QUEST_QUERY_RESPONSE))"]
     * @values [QUEST_FLAGS_TRACKING, 0x400, "These quests are automatically rewarded on quest complete and they will never appear in quest log client side."]
     * @values [QUEST_FLAGS_DEPRECATE_REPUTATION, 0x800, "Not used currently"]
     * @values [QUEST_FLAGS_DAILY, 0x1000, "Used to know quest is Daily one"]
     * @values [QUEST_FLAGS_FLAGS_PVP, 0x2000, "Having this quest in log forces PvP flag"]
     * @values [QUEST_FLAGS_UNAVAILABLE, 0x4000, "Used on quests that are not generically available"]
     * @values [QUEST_FLAGS_WEEKLY, 0x8000, ""]
     * @values [QUEST_FLAGS_AUTOCOMPLETE, 0x10000, "auto complete"]
     * @values [QUEST_FLAGS_DISPLAY_ITEM_IN_TRACKER, 0x20000, "Displays usable item in quest tracker"]
     * @values [QUEST_FLAGS_OBJ_TEXT, 0x40000, "use Objective text as Complete text"]
     * @values [QUEST_FLAGS_AUTO_ACCEPT, 0x80000, "The client recognizes this flag as auto-accept. However, NONE of the current quests (3.3.5a) have this flag. Maybe blizz used to use it, or will use it in the future."]
     *
     * @param [QuestFlags] flag : all available flags can be seen above
     * @return bool hasFlag
     */
    int HasFlag(Forge* F, Quest* quest)
    {
        uint32 flag = F->CHECKVAL<uint32>(2);

        F->Push(quest->HasFlag(flag));
        return 1;
    }

    /**
     * Returns 'true' if the [Quest] is a daily quest, false otherwise.
     *
     * @return bool isDaily
     */
    int IsDaily(Forge* F, Quest* quest)
    {
        F->Push(quest->IsDaily());
        return 1;
    }

    /**
     * Returns 'true' if the [Quest] is repeatable, false otherwise.
     *
     * @return bool isRepeatable
     */
    int IsRepeatable(Forge* F, Quest* quest)
    {
        F->Push(quest->IsRepeatable());
        return 1;
    }

    /**
     * Returns entry ID of the [Quest].
     *
     * @return uint32 entryId
     */
    int GetId(Forge* F, Quest* quest)
    {
        F->Push(quest->GetQuestId());
        return 1;
    }

    /**
     * Returns the [Quest]'s level.
     *
     * @return uint32 level
     */
    int GetLevel(Forge* F, Quest* quest)
    {
        F->Push(quest->GetQuestLevel());
        return 1;
    }

    /**
     * Returns the minimum level required to pick up the [Quest].
     *
     * @return uint32 minLevel
     */
    int GetMinLevel(Forge* F, Quest* quest)
    {
        F->Push(quest->GetMinLevel());
        return 1;
    }

    /**
     * Returns the next [Quest] entry ID.
     *
     * @return int32 entryId
     */
    int GetNextQuestId(Forge* F, Quest* quest)
    {
        F->Push(quest->GetNextQuestId());
        return 1;
    }

    /**
     * Returns the previous [Quest] entry ID.
     *
     * @return int32 entryId
     */
    int GetPrevQuestId(Forge* F, Quest* quest)
    {
        F->Push(quest->GetPrevQuestId());
        return 1;
    }

    /**
     * Returns the next [Quest] entry ID in the specific [Quest] chain.
     *
     * @return int32 entryId
     */
    int GetNextQuestInChain(Forge* F, Quest* quest)
    {
        F->Push(quest->GetNextQuestInChain());
        return 1;
    }

    /**
     * Returns the [Quest]'s flags.
     *
     * @return [QuestFlags] flags
     */
    int GetFlags(Forge* F, Quest* quest)
    {
        F->Push(quest->GetFlags());
        return 1;
    }

    /**
     * Returns the [Quest]'s type.
     *
     * TODO: Document types available.
     *
     * @return uint32 type
     */
    int GetType(Forge* F, Quest* quest)
    {
        F->Push(quest->GetType());
        return 1;
    }

    /**
     * Returns the maximum level where the [Quest] can still be picked up.
     *
     * @return uint32 maxLevel
     */
    int GetMaxLevel(Forge* F, Quest* quest)
    {
        F->Push(quest->GetMaxLevel());
        return 1;
    }
    
    ForgeRegister<Quest> QuestMethods[] =
    {
        // Getters
        { "GetId", &LuaQuest::GetId },
        { "GetLevel", &LuaQuest::GetLevel },
        { "GetMaxLevel", &LuaQuest::GetMaxLevel },
        { "GetMinLevel", &LuaQuest::GetMinLevel },
        { "GetNextQuestId", &LuaQuest::GetNextQuestId },
        { "GetPrevQuestId", &LuaQuest::GetPrevQuestId },
        { "GetNextQuestInChain", &LuaQuest::GetNextQuestInChain },
        { "GetFlags", &LuaQuest::GetFlags },
        { "GetType", &LuaQuest::GetType },

        // Boolean
        { "HasFlag", &LuaQuest::HasFlag },
        { "IsDaily", &LuaQuest::IsDaily },
        { "IsRepeatable", &LuaQuest::IsRepeatable }
    };
};
#endif
