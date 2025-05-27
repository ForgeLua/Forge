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

#ifndef CORPSEMETHODS_H
#define CORPSEMETHODS_H

/***
 * The remains of a [Player] that has died.
 *
 * Inherits all methods from: [Object], [WorldObject]
 */
namespace LuaCorpse
{
    /**
     * Returns the GUID of the [Player] that left the [Corpse] behind.
     *
     * @return ObjectGuid ownerGUID
     */
    int GetOwnerGUID(Forge* F, Corpse* corpse)
    {
        F->Push(corpse->GetOwnerGUID());
        return 1;
    }

    /**
     * Returns the time when the [Player] became a ghost and spawned this [Corpse].
     *
     * @return uint32 ghostTime
     */
    int GetGhostTime(Forge* F, Corpse* corpse)
    {
        F->Push(corpse->GetGhostTime());
        return 1;
    }

    /**
     * Returns the [CorpseType] of a [Corpse].
     *
     * @table
     * @columns [CorpseType, ID]
     * @values [CORPSE_BONES, 0]
     * @values [CORPSE_RESURRECTABLE_PVE, 1]
     * @values [CORPSE_RESURRECTABLE_PVP, 2]
     *
     * @return [CorpseType] corpseType
     */
    int GetType(Forge* F, Corpse* corpse)
    {
        F->Push(corpse->GetType());
        return 1;
    }

    /**
     * Sets the "ghost time" to the current time.
     *
     * See [Corpse:GetGhostTime].
     */
    int ResetGhostTime(Forge* /*E*/, Corpse* corpse)
    {
        corpse->ResetGhostTime();
        return 0;
    }

    /**
     * Saves the [Corpse] to the database.
     */
    int SaveToDB(Forge* /*E*/, Corpse* corpse)
    {
        corpse->SaveToDB();
        return 0;
    }
    
    ForgeRegister<Corpse> CorpseMethods[] =
    {
        // Getters
        { "GetOwnerGUID", &LuaCorpse::GetOwnerGUID },
        { "GetGhostTime", &LuaCorpse::GetGhostTime },
        { "GetType", &LuaCorpse::GetType },

        // Other
        { "ResetGhostTime", &LuaCorpse::ResetGhostTime },
        { "SaveToDB", &LuaCorpse::SaveToDB }
    };
};
#endif
