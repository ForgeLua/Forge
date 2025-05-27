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

#ifndef WORLDPACKETMETHODS_H
#define WORLDPACKETMETHODS_H

/***
 * A packet used to pass messages between the server and a client.
 *
 * Each packet has an opcode that determines the type of message being sent,
 *   e.g. if a CMSG_LOGOUT_REQUEST packet is sent to the server,
 *   the client has sent a message that its [Player] wants to logout.
 *
 * The packet can contain further data, the format of which depends on the opcode.
 *
 * Inherits all methods from: none
 */
namespace LuaPacket
{
    /**
     * Returns the opcode of the [WorldPacket].
     *
     * @return uint16 opcode
     */
    int GetOpcode(Forge* F, WorldPacket* packet)
    {
        F->Push(packet->GetOpcode());
        return 1;
    }

    /**
     * Returns the size of the [WorldPacket].
     *
     * @return uint32 size
     */
    int GetSize(Forge* F, WorldPacket* packet)
    {
        F->Push(packet->size());
        return 1;
    }

    /**
     * Sets the opcode of the [WorldPacket] to the specified opcode.
     *
     * @param [Opcodes] opcode : see Opcodes.h for all known opcodes
     */
    int SetOpcode(Forge* F, WorldPacket* packet)
    {
        uint32 opcode = F->CHECKVAL<uint32>(2);
        if (opcode >= NUM_MSG_TYPES)
            return luaL_argerror(F->L, 2, "valid opcode expected");

        packet->SetOpcode((OpcodesList)opcode);
        return 0;
    }

    /**
     * Reads and returns a signed 8-bit integer value from the [WorldPacket].
     *
     * @return int8 value
     */
    int ReadByte(Forge* F, WorldPacket* packet)
    {
        int8 _byte;
        (*packet) >> _byte;
        F->Push(_byte);
        return 1;
    }

    /**
     * Reads and returns an unsigned 8-bit integer value from the [WorldPacket].
     *
     * @return uint8 value
     */
    int ReadUByte(Forge* F, WorldPacket* packet)
    {
        uint8 _ubyte;
        (*packet) >> _ubyte;
        F->Push(_ubyte);
        return 1;
    }

    /**
     * Reads and returns a signed 16-bit integer value from the [WorldPacket].
     *
     * @return int16 value
     */
    int ReadShort(Forge* F, WorldPacket* packet)
    {
        int16 _short;
        (*packet) >> _short;
        F->Push(_short);
        return 1;
    }

    /**
     * Reads and returns an unsigned 16-bit integer value from the [WorldPacket].
     *
     * @return uint16 value
     */
    int ReadUShort(Forge* F, WorldPacket* packet)
    {
        uint16 _ushort;
        (*packet) >> _ushort;
        F->Push(_ushort);
        return 1;
    }

    /**
     * Reads and returns a signed 32-bit integer value from the [WorldPacket].
     *
     * @return int32 value
     */
    int ReadLong(Forge* F, WorldPacket* packet)
    {
        int32 _long;
        (*packet) >> _long;
        F->Push(_long);
        return 1;
    }

    /**
     * Reads and returns an unsigned 32-bit integer value from the [WorldPacket].
     *
     * @return uint32 value
     */
    int ReadULong(Forge* F, WorldPacket* packet)
    {
        uint32 _ulong;
        (*packet) >> _ulong;
        F->Push(_ulong);
        return 1;
    }

    /**
     * Reads and returns a single-precision floating-point value from the [WorldPacket].
     *
     * @return float value
     */
    int ReadFloat(Forge* F, WorldPacket* packet)
    {
        float _val;
        (*packet) >> _val;
        F->Push(_val);
        return 1;
    }

    /**
     * Reads and returns a double-precision floating-point value from the [WorldPacket].
     *
     * @return double value
     */
    int ReadDouble(Forge* F, WorldPacket* packet)
    {
        double _val;
        (*packet) >> _val;
        F->Push(_val);
        return 1;
    }

    /**
     * Reads and returns an unsigned 64-bit integer value from the [WorldPacket].
     *
     * @return ObjectGuid value : value returned as string
     */
    int ReadGUID(Forge* F, WorldPacket* packet)
    {
        ObjectGuid guid;
        (*packet) >> guid;
        F->Push(guid);
        return 1;
    }

    /**
     * Reads and returns a string value from the [WorldPacket].
     *
     * @return string value
     */
    int ReadString(Forge* F, WorldPacket* packet)
    {
        std::string _val;
        (*packet) >> _val;
        F->Push(_val);
        return 1;
    }

    /**
     * Writes an unsigned 64-bit integer value to the [WorldPacket].
     *
     * @param ObjectGuid value : the value to be written to the [WorldPacket]
     */
    int WriteGUID(Forge* F, WorldPacket* packet)
    {
        ObjectGuid guid = F->CHECKVAL<ObjectGuid>(2);
        (*packet) << guid;
        return 0;
    }

    /**
     * Writes a string to the [WorldPacket].
     *
     * @param string value : the string to be written to the [WorldPacket]
     */
    int WriteString(Forge* F, WorldPacket* packet)
    {
        std::string _val = F->CHECKVAL<std::string>(2);
        (*packet) << _val;
        return 0;
    }

    /**
     * Writes a signed 8-bit integer value to the [WorldPacket].
     *
     * @param int8 value : the int8 value to be written to the [WorldPacket]
     */
    int WriteByte(Forge* F, WorldPacket* packet)
    {
        int8 byte = F->CHECKVAL<int8>(2);
        (*packet) << byte;
        return 0;
    }

    /**
     * Writes an unsigned 8-bit integer value to the [WorldPacket].
     *
     * @param uint8 value : the uint8 value to be written to the [WorldPacket]
     */
    int WriteUByte(Forge* F, WorldPacket* packet)
    {
        uint8 byte = F->CHECKVAL<uint8>(2);
        (*packet) << byte;
        return 0;
    }

    /**
     * Writes a signed 16-bit integer value to the [WorldPacket].
     *
     * @param int16 value : the int16 value to be written to the [WorldPacket]
     */
    int WriteShort(Forge* F, WorldPacket* packet)
    {
        int16 _short = F->CHECKVAL<int16>(2);
        (*packet) << _short;
        return 0;
    }

    /**
     * Writes an unsigned 16-bit integer value to the [WorldPacket].
     *
     * @param uint16 value : the uint16 value to be written to the [WorldPacket]
     */
    int WriteUShort(Forge* F, WorldPacket* packet)
    {
        uint16 _ushort = F->CHECKVAL<uint16>(2);
        (*packet) << _ushort;
        return 0;
    }

    /**
     * Writes a signed 32-bit integer value to the [WorldPacket].
     *
     * @param int32 value : the int32 value to be written to the [WorldPacket]
     */
    int WriteLong(Forge* F, WorldPacket* packet)
    {
        int32 _long = F->CHECKVAL<int32>(2);
        (*packet) << _long;
        return 0;
    }

    /**
     * Writes an unsigned 32-bit integer value to the [WorldPacket].
     *
     * @param uint32 value : the uint32 value to be written to the [WorldPacket]
     */
    int WriteULong(Forge* F, WorldPacket* packet)
    {
        uint32 _ulong = F->CHECKVAL<uint32>(2);
        (*packet) << _ulong;
        return 0;
    }

    /**
     * Writes a 32-bit floating-point value to the [WorldPacket].
     *
     * @param float value : the float value to be written to the [WorldPacket]
     */
    int WriteFloat(Forge* F, WorldPacket* packet)
    {
        float _val = F->CHECKVAL<float>(2);
        (*packet) << _val;
        return 0;
    }

    /**
     * Writes a 64-bit floating-point value to the [WorldPacket].
     *
     * @param double value : the double value to be written to the [WorldPacket]
     */
    int WriteDouble(Forge* F, WorldPacket* packet)
    {
        double _val = F->CHECKVAL<double>(2);
        (*packet) << _val;
        return 0;
    }
    
    ForgeRegister<WorldPacket> PacketMethods[] =
    {
        // Getters
        { "GetOpcode", &LuaPacket::GetOpcode },
        { "GetSize", &LuaPacket::GetSize },

        // Setters
        { "SetOpcode", &LuaPacket::SetOpcode },

        // Readers
        { "ReadByte", &LuaPacket::ReadByte },
        { "ReadUByte", &LuaPacket::ReadUByte },
        { "ReadShort", &LuaPacket::ReadShort },
        { "ReadUShort", &LuaPacket::ReadUShort },
        { "ReadLong", &LuaPacket::ReadLong },
        { "ReadULong", &LuaPacket::ReadULong },
        { "ReadGUID", &LuaPacket::ReadGUID },
        { "ReadString", &LuaPacket::ReadString },
        { "ReadFloat", &LuaPacket::ReadFloat },
        { "ReadDouble", &LuaPacket::ReadDouble },

        // Writers
        { "WriteByte", &LuaPacket::WriteByte },
        { "WriteUByte", &LuaPacket::WriteUByte },
        { "WriteShort", &LuaPacket::WriteShort },
        { "WriteUShort", &LuaPacket::WriteUShort },
        { "WriteLong", &LuaPacket::WriteLong },
        { "WriteULong", &LuaPacket::WriteULong },
        { "WriteGUID", &LuaPacket::WriteGUID },
        { "WriteString", &LuaPacket::WriteString },
        { "WriteFloat", &LuaPacket::WriteFloat },
        { "WriteDouble", &LuaPacket::WriteDouble }
    };
};

#endif
