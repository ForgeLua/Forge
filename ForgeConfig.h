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

#ifndef _FORGECONFIG_H
#define _FORGECONFIG_H

#include "ForgeUtility.h"

enum ForgeConfigBoolValues
{
    CONFIG_FORGE_ENABLED,
    CONFIG_FORGE_COMPATIBILITY_MODE,
    CONFIG_FORGE_TRACEBACK,
    CONFIG_FORGE_SCRIPT_RELOADER,
    CONFIG_FORGE_BOOL_COUNT
};

enum ForgeConfigStringValues
{
    CONFIG_FORGE_SCRIPT_PATH,
    CONFIG_FORGE_ONLY_ON_MAPS,
    CONFIG_FORGE_REQUIRE_PATH_EXTRA,
    CONFIG_FORGE_REQUIRE_CPATH_EXTRA,
    CONFIG_FORGE_STRING_COUNT
};

class ForgeConfig
{
private:
    ForgeConfig();
    ~ForgeConfig();
    ForgeConfig(ForgeConfig const&) = delete;
    ForgeConfig& operator=(ForgeConfig const&) = delete;

public:
    static ForgeConfig* instance();

    void Initialize();

    bool GetConfig(ForgeConfigBoolValues index) const { return _configBoolValues[index]; }
    const std::string& GetConfig(ForgeConfigStringValues index) const { return _configStringValues[index]; }
    void SetConfig(ForgeConfigBoolValues index, bool value) { _configBoolValues[index] = value; }
    void SetConfig(ForgeConfigStringValues index, std::string value) { _configStringValues[index] = value; }

    bool IsForgeEnabled();
    bool IsForgeCompatibilityMode();
    bool ShouldMapLoadForge(uint32 mapId);

private:
    bool _configBoolValues[CONFIG_FORGE_BOOL_COUNT];
    std::string _configStringValues[CONFIG_FORGE_STRING_COUNT];

    void SetConfig(ForgeConfigBoolValues index, char const* fieldname, bool defvalue);
    void SetConfig(ForgeConfigStringValues index, char const* fieldname, std::string defvalue);

    void TokenizeAllowedMaps();

    std::unordered_set<uint32> m_allowedMaps;
};

#define sForgeConfig ForgeConfig::instance()

#endif //_FORGECONFIG_H
