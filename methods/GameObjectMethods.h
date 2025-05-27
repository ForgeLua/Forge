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

#ifndef GAMEOBJECTMETHODS_H
#define GAMEOBJECTMETHODS_H

/***
 * Inherits all methods from: [Object], [WorldObject]
 */
namespace LuaGameObject
{
    /**
     * Returns 'true' if the [GameObject] can give the specified [Quest]
     *
     * @param uint32 questId : quest entry Id to check
     * @return bool hasQuest
     */
    int HasQuest(Forge* F, GameObject* go)
    {
        uint32 questId = F->CHECKVAL<uint32>(2);

        F->Push(go->hasQuest(questId));
        return 1;
    }

    /**
     * Returns 'true' if the [GameObject] is spawned
     *
     * @return bool isSpawned
     */
    int IsSpawned(Forge* F, GameObject* go)
    {
        F->Push(go->isSpawned());
        return 1;
    }

    /**
     * Returns 'true' if the [GameObject] is a transport
     *
     * @return bool isTransport
     */
    int IsTransport(Forge* F, GameObject* go)
    {
        F->Push(go->IsTransport());
        return 1;
    }

    /**
     * Returns 'true' if the [GameObject] is active
     *
     * @return bool isActive
     */
    int IsActive(Forge* F, GameObject* go)
    {
        F->Push(go->isActiveObject());
        return 1;
    }

    /**
     * Returns true if the [GameObject] is a destructible, false otherwise.
     *
     * @return bool isDestructible
     */
    int IsDestructible(Forge* F, GameObject* go)
    {
        F->Push(go->IsDestructibleBuilding());
        return 1;
    }

    /**
     * Returns display ID of the [GameObject]
     *
     * @return uint32 displayId
     */
    int GetDisplayId(Forge* F, GameObject* go)
    {
        F->Push(go->GetDisplayId());
        return 1;
    }

    /**
     * Returns the state of a [GameObject]
     * Below are client side [GOState]s off of 3.3.5a
     *
     * @table
     * @columns [GOState, ID, Comment]
     * @values [GO_STATE_ACTIVE, 0, "show in world as used and not reset (closed door open)"]
     * @values [GO_STATE_READY, 1, "show in world as ready (closed door close)"]
     * @values [GO_STATE_ACTIVE_ALTERNATIVE, 2, "show in world as used in alt way and not reset (closed door open by cannon fire)"]
     *
     * @return [GOState] goState
     */
    int GetGoState(Forge* F, GameObject* go)
    {
        F->Push(go->GetGoState());
        return 1;
    }

    /**
     * Returns the [LootState] of a [GameObject]
     * Below are [LootState]s off of 3.3.5a
     *
     * @table
     * @columns [LootState, ID, Comment]
     * @values [GO_NOT_READY, 0, ""]
     * @values [GO_READY, 1, "can be ready but despawned, and then not possible activate until spawn"]
     * @values [GO_ACTIVATED, 2, ""]
     * @values [GO_JUST_DEACTIVATED, 3, ""]
     *
     * @return [LootState] lootState
     */
    int GetLootState(Forge* F, GameObject* go)
    {
        F->Push(go->getLootState());
        return 1;
    }

    /**
     * Returns the [Player] that can loot the [GameObject]
     *
     * Not the original looter and may be nil.
     *
     * @return [Player] player
     */
    int GetLootRecipient(Forge* F, GameObject* go)
    {
        F->Push(go->GetLootRecipient());
        return 1;
    }

    /**
     * Returns the [Group] that can loot the [GameObject]
     *
     * Not the original looter and may be nil.
     *
     * @return [Group] group
     */
    int GetLootRecipientGroup(Forge* F, GameObject* go)
    {
        F->Push(go->GetLootRecipientGroup());
        return 1;
    }

    /**
     * Returns the guid of the [GameObject] that is used as the ID in the database
     *
     * @return uint32 dbguid
     */
    int GetDBTableGUIDLow(Forge* F, GameObject* go)
    {
        F->Push(go->GetSpawnId());
        return 1;
    }

    /**
     * Sets the state of a [GameObject]
     *
     * @table
     * @columns [GOState, ID, Comment]
     * @values [GO_STATE_ACTIVE, 0, "show in world as used and not reset (closed door open)"]
     * @values [GO_STATE_READY, 1, "show in world as ready (closed door close)"]
     * @values [GO_STATE_ACTIVE_ALTERNATIVE, 2, "show in world as used in alt way and not reset (closed door open by cannon fire)"]
     *
     * @param [GOState] state : all available go states can be seen above
     */
    int SetGoState(Forge* F, GameObject* go)
    {
        uint32 state = F->CHECKVAL<uint32>(2, 0);

        if (state == 0)
            go->SetGoState(GO_STATE_ACTIVE);
        else if (state == 1)
            go->SetGoState(GO_STATE_READY);
        else if (state == 2)
            go->SetGoState(GO_STATE_DESTROYED);

        return 0;
    }

    /**
     * Sets the [LootState] of a [GameObject]
     * Below are [LootState]s off of 3.3.5a
     *
     * @table
     * @columns [LootState, ID, Comment]
     * @values [GO_NOT_READY, 0, ""]
     * @values [GO_READY, 1, "can be ready but despawned, and then not possible activate until spawn"]
     * @values [GO_ACTIVATED, 2, ""]
     * @values [GO_JUST_DEACTIVATED, 3, ""]
     *
     * @param [LootState] state : all available loot states can be seen above
     */
    int SetLootState(Forge* F, GameObject* go)
    {
        uint32 state = F->CHECKVAL<uint32>(2, 0);

        if (state == 0)
            go->SetLootState(GO_NOT_READY);
        else if (state == 1)
            go->SetLootState(GO_READY);
        else if (state == 2)
            go->SetLootState(GO_ACTIVATED);
        else if (state == 3)
            go->SetLootState(GO_JUST_DEACTIVATED);

        return 0;
    }

    /**
     * Saves [GameObject] to the database
     *
     */
    int SaveToDB(Forge* /*E*/, GameObject* go)
    {
        go->SaveToDB();
        return 0;
    }

    /**
     * Removes [GameObject] from the world
     *
     * The object is no longer reachable after this and it is not respawned.
     *
     * @param bool deleteFromDB : if true, it will delete the [GameObject] from the database
     */
    int RemoveFromWorld(Forge* F, GameObject* go)
    {
        bool deldb = F->CHECKVAL<bool>(2, false);

        // cs_gobject.cpp copy paste
        ObjectGuid ownerGuid = go->GetOwnerGUID();
        if (ownerGuid)
        {
            Unit* owner = eObjectAccessor()GetUnit(*go, ownerGuid);
            if (!owner || !ownerGuid.IsPlayer())
                return 0;

            owner->RemoveGameObject(go, false);
        }

        if (deldb)
            GameObject::DeleteFromDB(go->GetSpawnId());

        go->SetRespawnTime(0);
        go->Delete();

        return 0;
    }

    /**
     * Activates a door or a button/lever
     *
     * @param uint32 delay = 0 : cooldown time in seconds to restore the [GameObject] back to normal. 0 for infinite duration
     */
    int UseDoorOrButton(Forge* F, GameObject* go)
    {
        uint32 delay = F->CHECKVAL<uint32>(2, 0);

        go->UseDoorOrButton(delay);
        return 0;
    }

    /**
     * Despawns a [GameObject]
     *
     * The gameobject may be automatically respawned by the core
     */
    int Despawn(Forge* /*E*/, GameObject* go)
    {
        go->SetLootState(GO_JUST_DEACTIVATED);
        return 0;
    }

    /**
     * Respawns a [GameObject]
     */
    int Respawn(Forge* /*E*/, GameObject* go)
    {
        go->Respawn();
        return 0;
    }

    /**
     * Sets the respawn or despawn time for the gameobject.
     *
     * Respawn time is also used as despawn time depending on gameobject settings
     *
     * @param int32 delay = 0 : cooldown time in seconds to respawn or despawn the object. 0 means never
     */
    int SetRespawnTime(Forge* F, GameObject* go)
    {
        int32 respawn = F->CHECKVAL<int32>(2);

        go->SetRespawnTime(respawn);
        return 0;
    }

    /**
     * Sets whether or not the [GameObject] will be spawned by default
     *
     * Primarily used for temporary spawns.
     *
     * @param bool spawn = true : if false, it will prevent the [GameObject] from respawning by default
     */
    int SetSpawnedByDefault(Forge* F, GameObject* go)
    {
        bool spawn = F->CHECKVAL<bool>(2, true);

        go->SetSpawnedByDefault(spawn);
        return 0;
    }

    ForgeRegister<GameObject> GameObjectMethods[] =
    {
        // Getters
        { "GetDisplayId", &LuaGameObject::GetDisplayId },
        { "GetGoState", &LuaGameObject::GetGoState },
        { "GetLootState", &LuaGameObject::GetLootState },
        { "GetLootRecipient", &LuaGameObject::GetLootRecipient },
        { "GetLootRecipientGroup", &LuaGameObject::GetLootRecipientGroup },
        { "GetDBTableGUIDLow", &LuaGameObject::GetDBTableGUIDLow },

        // Setters
        { "SetGoState", &LuaGameObject::SetGoState },
        { "SetLootState", &LuaGameObject::SetLootState },
        { "SetRespawnTime", &LuaGameObject::SetRespawnTime },
        { "SetSpawnedByDefault", &LuaGameObject::SetSpawnedByDefault },

        // Boolean
        { "IsTransport", &LuaGameObject::IsTransport },
        { "IsDestructible", &LuaGameObject::IsDestructible },
        { "IsActive", &LuaGameObject::IsActive },
        { "HasQuest", &LuaGameObject::HasQuest },
        { "IsSpawned", &LuaGameObject::IsSpawned },

        // Other
        { "RemoveFromWorld", &LuaGameObject::RemoveFromWorld },
        { "UseDoorOrButton", &LuaGameObject::UseDoorOrButton },
        { "Despawn", &LuaGameObject::Despawn },
        { "Respawn", &LuaGameObject::Respawn },
        { "SaveToDB", &LuaGameObject::SaveToDB }
    };
};
#endif
