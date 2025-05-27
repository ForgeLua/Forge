/*
 * Copyright (C) 2010 - 2024 Forge Lua Engine <https://forgeluaengine.github.io/>
 * This program is free software licensed under GPL version 3
 * Please see the included DOCS/LICENSE.md for more information
 */

#include "ForgeInstanceAI.h"
#include "ForgeUtility.h"
#include "lmarshal.h"


#if !defined FORGE_TRINITY
void ForgeInstanceAI::Initialize()
{
    ASSERT(!instance->GetForge()->HasInstanceData(instance));

    // Create a new table for instance data.
    lua_State* L = instance->GetForge()->L;
    lua_newtable(L);
    instance->GetForge()->CreateInstanceData(instance);

    instance->GetForge()->OnInitialize(this);
}
#endif

void ForgeInstanceAI::Load(const char* data)
{
    // If we get passed NULL (i.e. `Reload` was called) then use
    //   the last known save data (or maybe just an empty string).
    if (!data)
    {
        data = lastSaveData.c_str();
    }
    else // Otherwise, copy the new data into our buffer.
    {
        lastSaveData.assign(data);
    }

    if (data[0] == '\0')
    {
        ASSERT(!instance->GetForge()->HasInstanceData(instance));

        // Create a new table for instance data.
        lua_State* L = instance->GetForge()->L;
        lua_newtable(L);
        instance->GetForge()->CreateInstanceData(instance);

        instance->GetForge()->OnLoad(this);
        // Stack: (empty)
        return;
    }

    size_t decodedLength;
    const unsigned char* decodedData = ForgeUtil::DecodeData(data, &decodedLength);
    lua_State* L = instance->GetForge()->L;

    if (decodedData)
    {
        // Stack: (empty)

        lua_pushcfunction(L, mar_decode);
        lua_pushlstring(L, (const char*)decodedData, decodedLength);
        // Stack: mar_decode, decoded_data

        // Call `mar_decode` and check for success.
        if (lua_pcall(L, 1, 1, 0) == 0)
        {
            // Stack: data
            // Only use the data if it's a table.
            if (lua_istable(L, -1))
            {
                instance->GetForge()->CreateInstanceData(instance);
                // Stack: (empty)
                instance->GetForge()->OnLoad(this);
                // WARNING! lastSaveData might be different after `OnLoad` if the Lua code saved data.
            }
            else
            {
                FORGE_LOG_ERROR("Error while loading instance data: Expected data to be a table (type 5), got type %d instead", lua_type(L, -1));
                lua_pop(L, 1);
                // Stack: (empty)

#if !defined FORGE_TRINITY
                Initialize();
#endif
            }
        }
        else
        {
            // Stack: error_message
            FORGE_LOG_ERROR("Error while parsing instance data with lua-marshal: %s", lua_tostring(L, -1));
            lua_pop(L, 1);
            // Stack: (empty)

#if !defined FORGE_TRINITY
            Initialize();
#endif
        }

        delete[] decodedData;
    }
    else
    {
        FORGE_LOG_ERROR("Error while decoding instance data: Data is not valid base-64");

#if !defined FORGE_TRINITY
        Initialize();
#endif
    }
}

const char* ForgeInstanceAI::Save() const
{
    lua_State* L = instance->GetForge()->L;
    // Stack: (empty)

    /*
     * Need to cheat because this method actually does modify this instance,
     *   even though it's declared as `const`.
     *
     * Declaring virtual methods as `const` is BAD!
     * Don't dictate to children that their methods must be pure.
     */
    ForgeInstanceAI* self = const_cast<ForgeInstanceAI*>(this);

    lua_pushcfunction(L, mar_encode);
    instance->GetForge()->PushInstanceData(self, false);
    // Stack: mar_encode, instance_data

    if (lua_pcall(L, 1, 1, 0) != 0)
    {
        // Stack: error_message
        FORGE_LOG_ERROR("Error while saving: %s", lua_tostring(L, -1));
        lua_pop(L, 1);
        return NULL;
    }

    // Stack: data
    size_t dataLength;
    const unsigned char* data = (const unsigned char*)lua_tolstring(L, -1, &dataLength);
    ForgeUtil::EncodeData(data, dataLength, self->lastSaveData);

    lua_pop(L, 1);
    // Stack: (empty)

    return lastSaveData.c_str();
}

uint32 ForgeInstanceAI::GetData(uint32 key) const
{
    Forge* F = instance->GetForge();
    lua_State* L = F->L;
    // Stack: (empty)

    F->PushInstanceData(const_cast<ForgeInstanceAI*>(this), false);
    // Stack: instance_data

    F->Push(key);
    // Stack: instance_data, key

    lua_gettable(L, -2);
    // Stack: instance_data, value

    uint32 value = F->CHECKVAL<uint32>(-1, 0);
    lua_pop(L, 2);
    // Stack: (empty)

    return value;
}

void ForgeInstanceAI::SetData(uint32 key, uint32 value)
{
    Forge* F = instance->GetForge();
    lua_State* L = F->L;
    // Stack: (empty)

    F->PushInstanceData(this, false);
    // Stack: instance_data

    F->Push(key);
    F->Push(value);
    // Stack: instance_data, key, value

    lua_settable(L, -3);
    // Stack: instance_data

    lua_pop(L, 1);
    // Stack: (empty)
}

uint64 ForgeInstanceAI::GetData64(uint32 key) const
{
    Forge* F = instance->GetForge();
    lua_State* L = F->L;
    // Stack: (empty)

    F->PushInstanceData(const_cast<ForgeInstanceAI*>(this), false);
    // Stack: instance_data

    F->Push(key);
    // Stack: instance_data, key

    lua_gettable(L, -2);
    // Stack: instance_data, value

    uint64 value = F->CHECKVAL<uint64>(-1, 0);
    lua_pop(L, 2);
    // Stack: (empty)

    return value;
}

void ForgeInstanceAI::SetData64(uint32 key, uint64 value)
{
    Forge* F = instance->GetForge();
    lua_State* L = F->L;
    // Stack: (empty)

    F->PushInstanceData(this, false);
    // Stack: instance_data

    F->Push(key);
    F->Push(value);
    // Stack: instance_data, key, value

    lua_settable(L, -3);
    // Stack: instance_data

    lua_pop(L, 1);
    // Stack: (empty)
}
