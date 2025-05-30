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

#include "ForgeUtility.h"
#include "World.h"
#include "Object.h"
#include "Unit.h"
#include "GameObject.h"
#include "DBCStores.h"

uint32 ForgeUtil::GetCurrTime()
{
    return getMSTime();
}

uint32 ForgeUtil::GetTimeDiff(uint32 oldMSTime)
{
    return GetMSTimeDiffToNow(oldMSTime);
}

ForgeUtil::ObjectGUIDCheck::ObjectGUIDCheck(ObjectGuid guid) : _guid(guid)
{
}

bool ForgeUtil::ObjectGUIDCheck::operator()(WorldObject* object)
{
    return object->GET_GUID() == _guid;
}

ForgeUtil::ObjectDistanceOrderPred::ObjectDistanceOrderPred(WorldObject const* pRefObj, bool ascending) : m_refObj(pRefObj), m_ascending(ascending)
{
}
bool ForgeUtil::ObjectDistanceOrderPred::operator()(WorldObject const* pLeft, WorldObject const* pRight) const
{
    return m_ascending ? m_refObj->GetDistanceOrder(pLeft, pRight) : !m_refObj->GetDistanceOrder(pLeft, pRight);
}

ForgeUtil::WorldObjectInRangeCheck::WorldObjectInRangeCheck(bool nearest, WorldObject const* obj, float range,
    uint16 typeMask, uint32 entry, uint32 hostile, uint32 dead) :
    i_obj(obj), i_obj_unit(nullptr), i_obj_fact(nullptr), i_hostile(hostile), i_entry(entry), i_range(range), i_typeMask(typeMask), i_dead(dead), i_nearest(nearest)
{
    i_obj_unit = i_obj->ToUnit();
    if (!i_obj_unit)
        if (GameObject const* go = i_obj->ToGameObject())
            i_obj_unit = go->GetOwner();
    if (!i_obj_unit)
        i_obj_fact = sFactionTemplateStore.LookupEntry(14);
}
WorldObject const& ForgeUtil::WorldObjectInRangeCheck::GetFocusObject() const
{
    return *i_obj;
}
bool ForgeUtil::WorldObjectInRangeCheck::operator()(WorldObject* u)
{
    if (i_typeMask && !u->isType(TypeMask(i_typeMask)))
        return false;
    if (i_entry && u->GetEntry() != i_entry)
        return false;
    if (i_obj->GET_GUID() == u->GET_GUID())
        return false;
    if (!i_obj->IsWithinDistInMap(u, i_range))
        return false;
    Unit const* target = u->ToUnit();
    if (!target)
        if (GameObject const* go = u->ToGameObject())
            target = go->GetOwner();
    if (target)
    {
        if (i_dead && (i_dead == 1) != target->IsAlive())
            return false;
        if (i_hostile)
        {
            if (!i_obj_unit)
            {
                if (i_obj_fact)
                {
                    if ((i_obj_fact->IsHostileTo(*target->GetFactionTemplateEntry())) != (i_hostile == 1))
                        return false;
                }
                else if (i_hostile == 1)
                    return false;
            }
            else if ((i_hostile == 1) != i_obj_unit->IsHostileTo(target))
                return false;
        }
    }
    if (i_nearest)
        i_range = i_obj->GetDistance(u);
    return true;
}

static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};
static char decoding_table[256];
static int mod_table[] = {0, 2, 1};

static void build_decoding_table()
{
    for (int i = 0; i < 64; i++)
        decoding_table[(unsigned char)encoding_table[i]] = i;
}

void ForgeUtil::EncodeData(const unsigned char* data, size_t input_length, std::string& output)
{
    size_t output_length = 4 * ((input_length + 2) / 3);
    char* buffer = new char[output_length];

    for (size_t i = 0, j = 0; i < input_length;)
    {
        uint32 octet_a = i < input_length ? (unsigned char)data[i++] : 0;
        uint32 octet_b = i < input_length ? (unsigned char)data[i++] : 0;
        uint32 octet_c = i < input_length ? (unsigned char)data[i++] : 0;

        uint32 triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        buffer[j++] = encoding_table[(triple >> (3 * 6)) & 0x3F];
        buffer[j++] = encoding_table[(triple >> (2 * 6)) & 0x3F];
        buffer[j++] = encoding_table[(triple >> (1 * 6)) & 0x3F];
        buffer[j++] = encoding_table[(triple >> (0 * 6)) & 0x3F];
    }

    for (int i = 0; i < mod_table[input_length % 3]; i++)
        buffer[output_length - 1 - i] = '=';

    output.assign(buffer, output_length); // Need length because `buffer` is not terminated!
    delete[] buffer;
}

unsigned char* ForgeUtil::DecodeData(const char *data, size_t *output_length)
{
    if (decoding_table[(unsigned char)'B'] == 0)
        build_decoding_table();

    size_t input_length = strlen(data);

    if (input_length % 4 != 0)
        return NULL;

    // Make sure there's no invalid characters in the data.
    for (size_t i = 0; i < input_length; ++i)
    {
        unsigned char byte = data[i];

        if (byte == '=')
            continue;

        // Every invalid character (and 'A') will map to 0 (due to `calloc`).
        if (decoding_table[byte] == 0 && byte != 'A')
            return NULL;
    }

    *output_length = input_length / 4 * 3;
    if (data[input_length - 1] == '=') (*output_length)--;
    if (data[input_length - 2] == '=') (*output_length)--;

    unsigned char *decoded_data = new unsigned char[*output_length];
    if (!decoded_data)
        return NULL;

    for (size_t i = 0, j = 0; i < input_length;)
    {
        uint32 sextet_a = data[i] == '=' ? 0 & i++ : decoding_table[(unsigned char)data[i++]];
        uint32 sextet_b = data[i] == '=' ? 0 & i++ : decoding_table[(unsigned char)data[i++]];
        uint32 sextet_c = data[i] == '=' ? 0 & i++ : decoding_table[(unsigned char)data[i++]];
        uint32 sextet_d = data[i] == '=' ? 0 & i++ : decoding_table[(unsigned char)data[i++]];

        uint32 triple = (sextet_a << (3 * 6))
        + (sextet_b << (2 * 6))
        + (sextet_c << (1 * 6))
        + (sextet_d << (0 * 6));

        if (j < *output_length) decoded_data[j++] = (triple >> (2 * 8)) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> (1 * 8)) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> (0 * 8)) & 0xFF;
    }

    return decoded_data;
}
