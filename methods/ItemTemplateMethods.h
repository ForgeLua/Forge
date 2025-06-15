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

#ifndef ITEMTEMPLATEMETHODS_H
#define ITEMTEMPLATEMETHODS_H

namespace LuaItemTemplate
{
    int GetEntry(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->ItemId);
        return 1;
    }

    int GetClass(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->Class);
        return 1;
    }

    int GetSubClass(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->SubClass);
        return 1;
    }

    int GetSoundOverride(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->SoundOverrideSubclass);
        return 1;
    }

    int GetName(Forge* F, ItemTemplate* item_template)
    {
        Player* player = F->CHECKOBJ<Player>(2);

        std::string item_name = item_template->Name1;
        LocaleConstant loc_idx = player->GetSession()->GetSessionDbLocaleIndex();
        if (ItemLocale const* il = eObjectMgr->GetItemLocale(item_template->ItemId))
            ObjectMgr::GetLocaleString(il->Name, loc_idx, item_name);

        F->Push(item_name);
        return 1;
    }

    int GetDisplayID(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->DisplayInfoID);
        return 1;
    }

    int GetQuality(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->Quality);
        return 1;
    }

    int GetFlags(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->Flags);
        return 1;
    }

    int GetFlags2(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->Flags2);
        return 1;
    }

    int GetBuyPrice(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->BuyPrice);
        return 1;
    }

    int GetSellPrice(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->SellPrice);
        return 1;
    }

    int GetInventoryType(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->InventoryType);
        return 1;
    }

    int GetAllowableClass(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->AllowableClass);
        return 1;
    }

    int GetAllowableRace(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->AllowableRace);
        return 1;
    }

    int GetItemLevel(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->ItemLevel);
        return 1;
    }

    int GetRequiredLevel(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->RequiredLevel);
        return 1;
    }

    int GetRequiredSkill(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->RequiredSkill);
        return 1;
    }

    int GetRequiredSkillRank(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->RequiredSkillRank);
        return 1;
    }

    int GetRequiredSpell(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->RequiredSpell);
        return 1;
    }

    int GetRequiredHonorRank(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->RequiredHonorRank);
        return 1;
    }

    int GetRequiredCityRank(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->RequiredCityRank);
        return 1;
    }

    int GetRequiredReputationFaction(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->RequiredReputationFaction);
        return 1;
    }

    int GetRequiredReputationRank(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->RequiredReputationRank);
        return 1;
    }

    int GetMaxCount(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->MaxCount);
        return 1;
    }

    int GetStackable(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->Stackable);
        return 1;
    }

    int GetContainerSlots(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->ContainerSlots);
        return 1;
    }

    int GetStatsCount(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->StatsCount);
        return 1;
    }

    int GetScalingStatDistribution(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->ScalingStatDistribution);
        return 1;
    }

    int GetScalingStatValue(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->ScalingStatValue);
        return 1;
    }

    int GetArmor(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->Armor);
        return 1;
    }

    int GetHolyRes(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->HolyRes);
        return 1;
    }

    int GetFireRes(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->FireRes);
        return 1;
    }

    int GetNatureRes(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->NatureRes);
        return 1;
    }

    int GetFrostRes(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->FrostRes);
        return 1;
    }

    int GetShadowRes(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->ShadowRes);
        return 1;
    }

    int GetArcaneRes(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->ArcaneRes);
        return 1;
    }

    int GetDelay(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->Delay);
        return 1;
    }

    int GetAmmoType(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->AmmoType);
        return 1;
    }

    int GetRangedModRange(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->RangedModRange);
        return 1;
    }

    int GetBonding(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->Bonding);
        return 1;
    }

    int GetDescription(Forge* F, ItemTemplate* item_template)
    {
        Player* player = F->CHECKOBJ<Player>(2);

        std::string item_desc = item_template->Description;
        LocaleConstant loc_idx = player->GetSession()->GetSessionDbLocaleIndex();
        if (ItemLocale const* il = eObjectMgr->GetItemLocale(item_template->ItemId))
            ObjectMgr::GetLocaleString(il->Description, loc_idx, item_desc);

        F->Push(item_desc);
        return 1;
    }

    int GetPageText(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->PageText);
        return 1;
    }

    int GetLanguageID(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->LanguageID);
        return 1;
    }

    int GetPageMaterial(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->PageMaterial);
        return 1;
    }

    int GetStartQuest(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->StartQuest);
        return 1;
    }

    int GetLockID(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->LockID);
        return 1;
    }

    int GetMaterial(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->Material);
        return 1;
    }

    int GetSheath(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->Sheath);
        return 1;
    }

    int GetRandomProperty(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->RandomProperty);
        return 1;
    }

    int GetRandomSuffix(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->RandomSuffix);
        return 1;
    }

    int GetBlock(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->Block);
        return 1;
    }

    int GetItemSet(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->ItemSet);
        return 1;
    }

    int GetMaxDurability(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->MaxDurability);
        return 1;
    }

    int GetArea(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->Area);
        return 1;
    }

    int GetMap(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->Map);
        return 1;
    }

    int GetBagFamily(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->BagFamily);
        return 1;
    }

    int GetTotemCategory(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->TotemCategory);
        return 1;
    }

    int GetSocketBonus(Forge* F, ItemTemplate* item_template)
    {

        F->Push(item_template->socketBonus);
        return 1;
    }

    int GetGemProperties(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->GemProperties);
        return 1;
    }

    int GetRequiredDisenchantSkill(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->RequiredDisenchantSkill);
        return 1;
    }

    int GetArmorDamageModifier(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->ArmorDamageModifier);
        return 1;
    }

    int GetDuration(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->Duration);
        return 1;
    }

    int GetItemLimitCategory(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->ItemLimitCategory);
        return 1;
    }

    int GetHolidayID(Forge* F, ItemTemplate* item_template)
    {
        F->Push(item_template->HolidayId);
        return 1;
    }

    int SetStat(Forge* F, ItemTemplate* item_template)
    {
        uint32 index = F->CHECKVAL<uint32>(2);
        if (index >= MAX_ITEM_PROTO_STATS)
            return 0;

        _ItemStat& stat = item_template->ItemStat[index];
        stat.ItemStatType = F->CHECKVAL<uint32>(3);
        stat.ItemStatValue = F->CHECKVAL<int32>(4);

        return 0;
    }

    ForgeRegister<ItemTemplate> ItemTemplateMethods[] =
    {
        { "GetEntry", &LuaItemTemplate::GetEntry },
        { "GetClass", &LuaItemTemplate::GetClass },
        { "GetSubClass", &LuaItemTemplate::GetSubClass },
        { "GetSoundOverride", &LuaItemTemplate::GetSoundOverride },
        { "GetName", &LuaItemTemplate::GetName },
        { "GetDisplayID", &LuaItemTemplate::GetDisplayID },
        { "GetQuality", &LuaItemTemplate::GetQuality },
        { "GetFlags", &LuaItemTemplate::GetFlags },
        { "GetFlags2", &LuaItemTemplate::GetFlags2 },
        { "GetBuyPrice", &LuaItemTemplate::GetBuyPrice },
        { "GetSellPrice", &LuaItemTemplate::GetSellPrice },
        { "GetInventoryType", &LuaItemTemplate::GetInventoryType },
        { "GetAllowableClass", &LuaItemTemplate::GetAllowableClass },
        { "GetAllowableRace", &LuaItemTemplate::GetAllowableRace },
        { "GetItemLevel", &LuaItemTemplate::GetItemLevel },
        { "GetRequiredLevel", &LuaItemTemplate::GetRequiredLevel },
        { "GetRequiredSkill", &LuaItemTemplate::GetRequiredSkill },
        { "GetRequiredSkillRank", &LuaItemTemplate::GetRequiredSkillRank },
        { "GetRequiredSpell", &LuaItemTemplate::GetRequiredSpell },
        { "GetRequiredHonorRank", &LuaItemTemplate::GetRequiredHonorRank },
        { "GetRequiredCityRank", &LuaItemTemplate::GetRequiredCityRank },
        { "GetRequiredReputationFaction", &LuaItemTemplate::GetRequiredReputationFaction },
        { "GetRequiredReputationRank", &LuaItemTemplate::GetRequiredReputationRank },
        { "GetMaxCount", &LuaItemTemplate::GetMaxCount },
        { "GetStackable", &LuaItemTemplate::GetStackable },
        { "GetContainerSlots", &LuaItemTemplate::GetContainerSlots },
        { "GetStatsCount", &LuaItemTemplate::GetStatsCount },
        { "GetScalingStatDistribution", &LuaItemTemplate::GetScalingStatDistribution },
        { "GetScalingStatValue", &LuaItemTemplate::GetScalingStatValue },
        { "GetArmor", &LuaItemTemplate::GetArmor },
        { "GetHolyRes", &LuaItemTemplate::GetHolyRes },
        { "GetFireRes", &LuaItemTemplate::GetFireRes },
        { "GetNatureRes", &LuaItemTemplate::GetNatureRes },
        { "GetFrostRes", &LuaItemTemplate::GetFrostRes },
        { "GetShadowRes", &LuaItemTemplate::GetShadowRes },
        { "GetArcaneRes", &LuaItemTemplate::GetArcaneRes },
        { "GetDelay", &LuaItemTemplate::GetDelay },
        { "GetAmmoType", &LuaItemTemplate::GetAmmoType },
        { "GetRangedModRange", &LuaItemTemplate::GetRangedModRange },
        { "GetBonding", &LuaItemTemplate::GetBonding },
        { "GetDescription", &LuaItemTemplate::GetDescription },
        { "GetPageText", &LuaItemTemplate::GetPageText },
        { "GetLanguageID", &LuaItemTemplate::GetLanguageID },
        { "GetPageMaterial", &LuaItemTemplate::GetPageMaterial },
        { "GetStartQuest", &LuaItemTemplate::GetStartQuest },
        { "GetLockID", &LuaItemTemplate::GetLockID },
        { "GetMaterial", &LuaItemTemplate::GetMaterial },
        { "GetSheath", &LuaItemTemplate::GetSheath },
        { "GetRandomProperty", &LuaItemTemplate::GetRandomProperty },
        { "GetRandomSuffix", &LuaItemTemplate::GetRandomSuffix },
        { "GetBlock", &LuaItemTemplate::GetBlock },
        { "GetItemSet", &LuaItemTemplate::GetItemSet },
        { "GetMaxDurability", &LuaItemTemplate::GetMaxDurability },
        { "GetArea", &LuaItemTemplate::GetArea },
        { "GetMap", &LuaItemTemplate::GetMap },
        { "GetBagFamily", &LuaItemTemplate::GetBagFamily },
        { "GetTotemCategory", &LuaItemTemplate::GetTotemCategory },
        { "GetSocketBonus", &LuaItemTemplate::GetSocketBonus },
        { "GetGemProperties", &LuaItemTemplate::GetGemProperties },
        { "GetRequiredDisenchantSkill", &LuaItemTemplate::GetRequiredDisenchantSkill },
        { "GetArmorDamageModifier", &LuaItemTemplate::GetArmorDamageModifier },
        { "GetDuration", &LuaItemTemplate::GetDuration },
        { "GetItemLimitCategory", &LuaItemTemplate::GetItemLimitCategory },
        { "GetHolidayID", &LuaItemTemplate::GetHolidayID },

        { "SetStat", &LuaItemTemplate::SetStat },
    };
}
#endif
