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

#ifndef _FORGE_UTIL_H
#define _FORGE_UTIL_H

#include "Common.h"

#define EXP_CLASSIC 0
#define EXP_TBC 1
#define EXP_WOTLK 2
#define EXP_CATA 3

#include "SharedDefines.h"
#include "ObjectGuid.h"
#include "Log.h"
#include "QueryResult.h"

#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <memory>

#define USING_BOOST

#if TRINITY_PLATFORM == TRINITY_PLATFORM_WINDOWS
#define FORGE_WINDOWS
#endif

typedef QueryResult ForgeQuery;
#define GET_GUID                GetGUID
#define HIGHGUID_PLAYER         HighGuid::Player
#define HIGHGUID_UNIT           HighGuid::Unit
#define HIGHGUID_ITEM           HighGuid::Item
#define HIGHGUID_GAMEOBJECT     HighGuid::GameObject
#define HIGHGUID_PET            HighGuid::Pet
#define HIGHGUID_TRANSPORT      HighGuid::Transport
#define HIGHGUID_VEHICLE        HighGuid::Vehicle
#define HIGHGUID_CONTAINER      HighGuid::Container
#define HIGHGUID_DYNAMICOBJECT  HighGuid::DynamicObject
#define HIGHGUID_CORPSE         HighGuid::Corpse
#define HIGHGUID_MO_TRANSPORT   HighGuid::Mo_Transport
#define HIGHGUID_INSTANCE       HighGuid::Instance
#define HIGHGUID_GROUP          HighGuid::Group

#include "fmt/printf.h"
#define FORGE_LOG_TC_FMT(TC_LOG_MACRO, ...) \
    try { \
        std::string message = fmt::sprintf(__VA_ARGS__); \
        TC_LOG_MACRO("forge", "{}", message); \
    } catch (const std::exception& e) { \
        TC_LOG_MACRO("forge", "Failed to format log message: {}", e.what()); \
    }
#define FORGE_LOG_INFO(...)     FORGE_LOG_TC_FMT(TC_LOG_INFO, __VA_ARGS__);
#define FORGE_LOG_ERROR(...)    FORGE_LOG_TC_FMT(TC_LOG_ERROR, __VA_ARGS__);
#define FORGE_LOG_DEBUG(...)    FORGE_LOG_TC_FMT(TC_LOG_DEBUG, __VA_ARGS__);

#if !defined MAKE_NEW_GUID
#define MAKE_NEW_GUID(l, e, h)  ObjectGuid(h, e, l)
#endif

#if !defined GUID_ENPART
#define GUID_ENPART(guid)       ObjectGuid(guid).GetEntry()
#endif

#if !defined GUID_LOPART
#define GUID_LOPART(guid)       ObjectGuid(guid).GetCounter()
#endif

#if !defined GUID_HIPART
#define GUID_HIPART(guid)       ObjectGuid(guid).GetHigh()
#endif

typedef std::vector<uint8> BytecodeBuffer;

class Unit;
class WorldObject;
struct FactionTemplateEntry;

namespace ForgeUtil
{
    uint32 GetCurrTime();

    uint32 GetTimeDiff(uint32 oldMSTime);

    class ObjectGUIDCheck
    {
    public:
        ObjectGUIDCheck(ObjectGuid guid);
        bool operator()(WorldObject* object);

        ObjectGuid _guid;
    };

    // Binary predicate to sort WorldObjects based on the distance to a reference WorldObject
    class ObjectDistanceOrderPred
    {
    public:
        ObjectDistanceOrderPred(WorldObject const* pRefObj, bool ascending = true);
        bool operator()(WorldObject const* pLeft, WorldObject const* pRight) const;

        WorldObject const* m_refObj;
        const bool m_ascending;
    };

    // Doesn't get self
    class WorldObjectInRangeCheck
    {
    public:
        WorldObjectInRangeCheck(bool nearest, WorldObject const* obj, float range,
            uint16 typeMask = 0, uint32 entry = 0, uint32 hostile = 0, uint32 dead = 0);
        WorldObject const& GetFocusObject() const;
        bool operator()(WorldObject* u);

        WorldObject const* const i_obj;
        Unit const* i_obj_unit;
        FactionTemplateEntry const* i_obj_fact;
        uint32 const i_hostile; // 0 both, 1 hostile, 2 friendly
        uint32 const i_entry;
        float i_range;
        uint16 const i_typeMask;
        uint32 const i_dead; // 0 both, 1 alive, 2 dead
        bool const i_nearest;
    };

    /*
     * Encodes `data` in Base-64 and store the result in `output`.
     */
    void EncodeData(const unsigned char* data, size_t input_length, std::string& output);

    /*
     * Decodes `data` from Base-64 and returns a pointer to the result, or `NULL` on error.
     *
     * The returned result buffer must be `delete[]`ed by the caller.
     */
    unsigned char* DecodeData(const char* data, size_t *output_length);
};

#endif
