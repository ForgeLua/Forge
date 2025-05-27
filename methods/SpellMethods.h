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

#ifndef SPELLMETHODS_H
#define SPELLMETHODS_H

/***
 * An instance of a spell, created when the spell is cast by a [Unit].
 *
 * Inherits all methods from: none
 */
namespace LuaSpell
{
    /**
     * Returns `true` if the [Spell] is automatically repeating, `false` otherwise.
     *
     * @return bool isAutoRepeating
     */
    int IsAutoRepeat(Forge* F, Spell* spell)
    {
        F->Push(spell->IsAutoRepeat());
        return 1;
    }

    /**
     * Returns the [Unit] that casted the [Spell].
     *
     * @return [Unit] caster
     */
    int GetCaster(Forge* F, Spell* spell)
    {
        F->Push(spell->GetCaster());
        return 1;
    }

    /**
     * Returns the cast time of the [Spell].
     *
     * @return int32 castTime
     */
    int GetCastTime(Forge* F, Spell* spell)
    {
        F->Push(spell->GetCastTime());
        return 1;
    }

    /**
     * Returns the entry ID of the [Spell].
     *
     * @return uint32 entryId
     */
    int GetEntry(Forge* F, Spell* spell)
    {
        F->Push(spell->m_spellInfo->Id);
        return 1;
    }

    /**
     * Returns the power cost of the [Spell].
     *
     * @return uint32 powerCost
     */
    int GetPowerCost(Forge* F, Spell* spell)
    {
        F->Push(spell->GetPowerCost());
        return 1;
    }

    /**
     * Returns the spell duration of the [Spell].
     *
     * @return int32 duration
     */
    int GetDuration(Forge* F, Spell* spell)
    {
        F->Push(spell->GetSpellInfo()->GetDuration());
        return 1;
    }

    /**
     * Returns the target destination coordinates of the [Spell].
     *
     * @return float x : x coordinate of the [Spell]
     * @return float y : y coordinate of the [Spell]
     * @return float z : z coordinate of the [Spell]
     */
    int GetTargetDest(Forge* F, Spell* spell)
    {
        if (!spell->m_targets.HasDst())
            return 3;

        float x, y, z;
        spell->m_targets.GetDstPos()->GetPosition(x, y, z);

        F->Push(x);
        F->Push(y);
        F->Push(z);
        return 3;
    }

    /**
     * Returns the target [Object] of the [Spell].
     *
     * The target can be any of the following [Object] types:
     * - [Player]
     * - [Creature]
     * - [GameObject]
     * - [Item]
     * - [Corpse]
     *
     * @return [Object] target
     */
    int GetTarget(Forge* F, Spell* spell)
    {
        if (GameObject* target = spell->m_targets.GetGOTarget())
            F->Push(target);
        else if (Item* target = spell->m_targets.GetItemTarget())
            F->Push(target);
        else if (Corpse* target = spell->m_targets.GetCorpseTarget())
            F->Push(target);
        else if (Unit* target = spell->m_targets.GetUnitTarget())
            F->Push(target);
        else if (WorldObject* target = spell->m_targets.GetObjectTarget())
            F->Push(target);
        return 1;
    }

    /**
     * Sets the [Spell] to automatically repeat.
     *
     * @param bool repeat : set variable to 'true' for spell to automatically repeat
     */
    int SetAutoRepeat(Forge* F, Spell* spell)
    {
        bool repeat = F->CHECKVAL<bool>(2);
        spell->SetAutoRepeat(repeat);
        return 0;
    }

    /**
     * Casts the [Spell].
     *
     * @param bool skipCheck = false : skips initial checks to see if the [Spell] can be casted or not, this is optional
     */
    int Cast(Forge* F, Spell* spell)
    {
        bool skipCheck = F->CHECKVAL<bool>(2, false);
        spell->cast(skipCheck);
        return 0;
    }

    /**
     * Cancels the [Spell].
     */
    int Cancel(Forge* /*E*/, Spell* spell)
    {
        spell->cancel();
        return 0;
    }

    /**
     * Finishes the [Spell].
     */
    int Finish(Forge* /*E*/, Spell* spell)
    {
        spell->finish();
        return 0;
    }
    
    ForgeRegister<Spell> SpellMethods[] =
    {
        // Getters
        { "GetCaster", &LuaSpell::GetCaster },
        { "GetCastTime", &LuaSpell::GetCastTime },
        { "GetEntry", &LuaSpell::GetEntry },
        { "GetDuration", &LuaSpell::GetDuration },
        { "GetPowerCost", &LuaSpell::GetPowerCost },
        { "GetTargetDest", &LuaSpell::GetTargetDest },
        { "GetTarget", &LuaSpell::GetTarget },

        // Setters
        { "SetAutoRepeat", &LuaSpell::SetAutoRepeat },

        // Boolean
        { "IsAutoRepeat", &LuaSpell::IsAutoRepeat },

        // Other
        { "Cancel", &LuaSpell::Cancel },
        { "Cast", &LuaSpell::Cast },
        { "Finish", &LuaSpell::Finish }
    };
};
#endif
