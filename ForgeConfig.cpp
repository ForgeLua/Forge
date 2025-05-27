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

#if defined FORGE_TRINITY
#include "Config.h"
#else
#include "Config/Config.h"
#endif
#include "ForgeConfig.h"

ForgeConfig::ForgeConfig()
{
}

ForgeConfig* ForgeConfig::instance()
{
    static ForgeConfig instance;
    return &instance;
}

ForgeConfig::~ForgeConfig()
{
}

void ForgeConfig::Initialize()
{
    // Load bools
    SetConfig(CONFIG_FORGE_ENABLED, "Forge.Enabled", true);
    SetConfig(CONFIG_FORGE_COMPATIBILITY_MODE, "Forge.CompatibilityMode", false);
    SetConfig(CONFIG_FORGE_TRACEBACK, "Forge.TraceBack", false);
    SetConfig(CONFIG_FORGE_SCRIPT_RELOADER, "Forge.ScriptReloader", false);

    // Load strings
    SetConfig(CONFIG_FORGE_SCRIPT_PATH, "Forge.ScriptPath", "lua_scripts");
    SetConfig(CONFIG_FORGE_ONLY_ON_MAPS, "Forge.OnlyOnMaps", "");
    SetConfig(CONFIG_FORGE_REQUIRE_PATH_EXTRA, "Forge.RequirePaths", "");
    SetConfig(CONFIG_FORGE_REQUIRE_CPATH_EXTRA, "Forge.RequireCPaths", "");

    // Call extra functions
    TokenizeAllowedMaps();
}

void ForgeConfig::SetConfig(ForgeConfigBoolValues index, char const* fieldname, bool defvalue)
{
#if defined FORGE_TRINITY
    SetConfig(index, sConfigMgr->GetBoolDefault(fieldname, defvalue));
#else
    SetConfig(index, sConfig.GetBoolDefault(fieldname, defvalue));
#endif
}

void ForgeConfig::SetConfig(ForgeConfigStringValues index, char const* fieldname, std::string defvalue)
{
#if defined FORGE_TRINITY
    SetConfig(index, sConfigMgr->GetStringDefault(fieldname, defvalue));
#elif defined FORGE_CMANGOS
    SetConfig(index, sConfig.GetStringDefault(fieldname, defvalue));
#else
    SetConfig(index, sConfig.GetStringDefault(fieldname, defvalue.c_str()));
#endif
}

bool ForgeConfig::IsForgeEnabled()
{
    return GetConfig(CONFIG_FORGE_ENABLED);
}

bool ForgeConfig::IsForgeCompatibilityMode()
{
    return GetConfig(CONFIG_FORGE_COMPATIBILITY_MODE);
}

bool ForgeConfig::ShouldMapLoadForge(uint32 id)
{
    // if the set is empty (all maps), return true
    if (m_allowedMaps.empty())
        return true;

    // Check if the map ID is in the set
    return (m_allowedMaps.find(id) != m_allowedMaps.end());
}

void ForgeConfig::TokenizeAllowedMaps()
{
    // clear allowed maps
    m_allowedMaps.clear();

    // read the configuration value into stringstream
    std::istringstream maps(GetConfig(CONFIG_FORGE_ONLY_ON_MAPS));

    // tokenize maps and add to allowed maps
    std::string mapIdStr;
    while (std::getline(maps, mapIdStr, ','))
    {
        // remove spaces
        mapIdStr.erase(std::remove_if(mapIdStr.begin(), mapIdStr.end(), [](char c) {
            return std::isspace(static_cast<unsigned char>(c));
            }), mapIdStr.end());

        try {
            uint32 mapId = std::stoul(mapIdStr);
            m_allowedMaps.emplace(mapId);
        }
        catch (std::exception&) {
            FORGE_LOG_ERROR("[Forge]: Error tokenizing Forge.OnlyOnMaps, invalid config value '%s'", mapIdStr.c_str());
        }
    }
}
