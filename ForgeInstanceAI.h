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

#ifndef _FORGE_INSTANCE_DATA_H
#define _FORGE_INSTANCE_DATA_H

#include "LuaEngine.h"
#include "InstanceScript.h"
#include "Map.h"

/*
 * This class is a small wrapper around `InstanceData`,
 *   allowing instances to be scripted with Forge.
 *
 *
 * Note 1
 * ======
 *
 * Instances of `ForgeInstanceAI` are owned by the core, so they
 *   are not deleted when Forge is reloaded. Thus `Load` is only called
 *   by the core once, no matter how many times Forge is reloaded.
 *
 * However, when Forge reloads, all instance data in Forge is lost.
 * So the solution is as follows:
 *
 *   1. Store the last save data in the member var `lastSaveData`.
 *
 *      At first this is just the data given to us by the core when it calls `Load`,
 *        but later on once we start saving new data this is from Forge.
 *
 *   2. When retrieving instance data from Forge, check if it's missing.
 *
 *      The data will be missing if Forge is reloaded, since a new Lua state is created.
 *
 *   3. If it *is* missing, call `Reload`.
 *
 *      This reloads the last known instance save data into Forge, and calls the appropriate hooks.
 *
 *
 * Note 2
 * ======
 *
 * CMaNGOS expects some of these methods to be `const`. However, any of these
 *   methods are free to call `Save`, resulting in mutation of `lastSaveData`.
 *
 * Therefore, none of the hooks are `const`-safe, and `const_cast` is used
 *   to escape from these restrictions.
 */
class ForgeInstanceAI : public InstanceData
{
private:
    // The last save data to pass through this class,
    //   either through `Load` or `Save`.
    std::string lastSaveData;

public:
    ForgeInstanceAI(Map* map) : InstanceData(map->ToInstanceMap())
    {
    }

    /*
     * These are responsible for serializing/deserializing the instance's
     *   data table to/from the core.
     */
    void Load(const char* data) override;

    // Simply calls Save, since the functions are a bit different in name and data types on different cores
    std::string GetSaveData() override
    {
        return Save();
    }
    const char* Save() const;

    /*
     * Calls `Load` with the last save data that was passed to
     * or from Forge.
     *
     * See: big documentation blurb at the top of this class.
     */
    void Reload()
    {
        Load(NULL);
    }

    /*
     * These methods allow non-Lua scripts (e.g. DB, C++) to get/set instance data.
     */
    uint32 GetData(uint32 key) const override;
    void SetData(uint32 key, uint32 value) override;

    uint64 GetData64(uint32 key) const override;
    void SetData64(uint32 key, uint64 value) override;

    /*
     * These methods are just thin wrappers around Forge.
     */
    void Update(uint32 diff) override
    {
        // If Forge is reloaded, it will be missing our instance data.
        // Reload here instead of waiting for the next hook call (possibly never).
        // This avoids having to have an empty Update hook handler just to trigger the reload.
        if (!instance->GetForge()->HasInstanceData(instance))
            Reload();

        instance->GetForge()->OnUpdateInstance(this, diff);
    }

    bool IsEncounterInProgress() const override
    {
        return instance->GetForge()->OnCheckEncounterInProgress(const_cast<ForgeInstanceAI*>(this));
    }

    void OnPlayerEnter(Player* player) override
    {
        instance->GetForge()->OnPlayerEnterInstance(this, player);
    }

    void OnGameObjectCreate(GameObject* gameobject) override
    {
        instance->GetForge()->OnGameObjectCreate(this, gameobject);
    }

    void OnCreatureCreate(Creature* creature) override
    {
        instance->GetForge()->OnCreatureCreate(this, creature);
    }
};

#endif // _FORGE_INSTANCE_DATA_H
