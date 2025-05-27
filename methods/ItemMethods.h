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

#ifndef ITEMMETHODS_H
#define ITEMMETHODS_H

/***
 * Inherits all methods from: [Object]
 */
namespace LuaItem
{
    /**
     * Returns 'true' if the [Item] is soulbound, 'false' otherwise
     *
     * @return bool isSoulBound
     */
    int IsSoulBound(Forge* F, Item* item)
    {
        F->Push(item->IsSoulBound());
        return 1;
    }

    /**
     * Returns 'true' if the [Item] is account bound, 'false' otherwise
     *
     * @return bool isAccountBound
     */
    int IsBoundAccountWide(Forge* F, Item* item)
    {
        F->Push(item->IsBoundAccountWide());
        return 1;
    }

    /**
     * Returns 'true' if the [Item] is bound to a [Player] by an enchant, 'false' otehrwise
     *
     * @return bool isBoundByEnchant
     */
    int IsBoundByEnchant(Forge* F, Item* item)
    {
        F->Push(item->IsBoundByEnchant());
        return 1;
    }

    /**
     * Returns 'true' if the [Item] is not bound to the [Player] specified, 'false' otherwise
     *
     * @param [Player] player : the [Player] object to check the item against
     * @return bool isNotBound
     */
    int IsNotBoundToPlayer(Forge* F, Item* item)
    {
        Player* player = F->CHECKOBJ<Player>(2);

        F->Push(item->IsBindedNotWith(player));
        return 1;
    }

    /**
     * Returns 'true' if the [Item] is locked, 'false' otherwise
     *
     * @return bool isLocked
     */
    int IsLocked(Forge* F, Item* item)
    {
        F->Push(item->IsLocked());
        return 1;
    }

    /**
     * Returns 'true' if the [Item] is a bag, 'false' otherwise
     *
     * @return bool isBag
     */
    int IsBag(Forge* F, Item* item)
    {
        F->Push(item->IsBag());
        return 1;
    }

    /**
     * Returns 'true' if the [Item] is a currency token, 'false' otherwise
     *
     * @return bool isCurrencyToken
     */
    int IsCurrencyToken(Forge* F, Item* item)
    {
        F->Push(item->IsCurrencyToken());
        return 1;
    }

    /**
     * Returns 'true' if the [Item] is a not an empty bag, 'false' otherwise
     *
     * @return bool isNotEmptyBag
     */
    int IsNotEmptyBag(Forge* F, Item* item)
    {
        F->Push(item->IsNotEmptyBag());
        return 1;
    }

    /**
     * Returns 'true' if the [Item] is broken, 'false' otherwise
     *
     * @return bool isBroken
     */
    int IsBroken(Forge* F, Item* item)
    {
        F->Push(item->IsBroken());
        return 1;
    }

    /**
     * Returns 'true' if the [Item] can be traded, 'false' otherwise
     *
     * @return bool isTradeable
     */
    int CanBeTraded(Forge* F, Item* item)
    {
        bool mail = F->CHECKVAL<bool>(2, false);

        F->Push(item->CanBeTraded(mail));
        return 1;
    }

    /**
     * Returns 'true' if the [Item] is currently in a trade window, 'false' otherwise
     *
     * @return bool isInTrade
     */
    int IsInTrade(Forge* F, Item* item)
    {
        F->Push(item->IsInTrade());
        return 1;
    }

    /**
     * Returns 'true' if the [Item] is currently in a bag, 'false' otherwise
     *
     * @return bool isInBag
     */
    int IsInBag(Forge* F, Item* item)
    {
        F->Push(item->IsInBag());
        return 1;
    }

    /**
     * Returns 'true' if the [Item] is currently equipped, 'false' otherwise
     *
     * @return bool isEquipped
     */
    int IsEquipped(Forge* F, Item* item)
    {
        F->Push(item->IsEquipped());
        return 1;
    }

    /**
     * Returns 'true' if the [Item] has the [Quest] specified tied to it, 'false' otherwise
     *
     * @param uint32 questId : the [Quest] id to be checked
     * @return bool hasQuest
     */
    int HasQuest(Forge* F, Item* item)
    {
        uint32 quest = F->CHECKVAL<uint32>(2);

        F->Push(item->hasQuest(quest));
        return 1;
    }

    /**
     * Returns 'true' if the [Item] is a potion, 'false' otherwise
     *
     * @return bool isPotion
     */
    int IsPotion(Forge* F, Item* item)
    {
        F->Push(item->IsPotion());
        return 1;
    }

    /**
     * Returns 'true' if the [Item] is a weapon vellum, 'false' otherwise
     *
     * @return bool isWeaponVellum
     */
    int IsWeaponVellum(Forge* F, Item* item)
    {
        F->Push(item->IsWeaponVellum());
        return 1;
    }

    /**
     * Returns 'true' if the [Item] is an armor vellum, 'false' otherwise
     *
     * @return bool isArmorVellum
     */
    int IsArmorVellum(Forge* F, Item* item)
    {
        F->Push(item->IsArmorVellum());
        return 1;
    }

    /**
     * Returns 'true' if the [Item] is a conjured consumable, 'false' otherwise
     *
     * @return bool isConjuredConsumable
     */
    int IsConjuredConsumable(Forge* F, Item* item)
    {
        F->Push(item->IsConjuredConsumable());
        return 1;
    }

    /**
     * Returns 'true' if the refund period has expired for this [Item], 'false' otherwise
     *
     * @return bool isRefundExpired
     */
    int IsRefundExpired(Forge* F, Item* item)
    {
        F->Push(item->IsRefundExpired());
        return 1;
    }

    /**
     * Returns the chat link of the [Item]
     *
     * @table
     * @columns [Locale, ID]
     * @values [LOCALE_enUS, 0]
     * @values [LOCALE_koKR, 1]
     * @values [LOCALE_frFR, 2]
     * @values [LOCALE_deDE, 3]
     * @values [LOCALE_zhCN, 4]
     * @values [LOCALE_zhTW, 5]
     * @values [LOCALE_esES, 6]
     * @values [LOCALE_esMX, 7]
     * @values [LOCALE_ruRU, 8]
     *
     * @param [LocaleConstant] locale = DEFAULT_LOCALE : locale to return the [Item]'s name in
     * @return string itemLink
     */
    int GetItemLink(Forge* F, Item* item)
    {
        uint8 locale = F->CHECKVAL<uint8>(2, DEFAULT_LOCALE);
        if (locale >= TOTAL_LOCALES)
            return luaL_argerror(F->L, 2, "valid LocaleConstant expected");

        const ItemTemplate* temp = item->GetTemplate();

        std::string name = temp->Name1;
        if (ItemLocale const* il = eObjectMgr->GetItemLocale(temp->ItemId))
            ObjectMgr::GetLocaleString(il->Name, static_cast<LocaleConstant>(locale), name);

        if (int32 itemRandPropId = item->GetItemRandomPropertyId())
        {
            std::array<char const*, 16> const* suffix = NULL;
            if (itemRandPropId < 0)
            {
                const ItemRandomSuffixEntry* itemRandEntry = sItemRandomSuffixStore.LookupEntry(-item->GetItemRandomPropertyId());
                if (itemRandEntry)
                    suffix = &itemRandEntry->Name;
            }
            else
            {
                const ItemRandomPropertiesEntry* itemRandEntry = sItemRandomPropertiesStore.LookupEntry(item->GetItemRandomPropertyId());
                if (itemRandEntry)
                    suffix = &itemRandEntry->Name;
            }
            if (suffix)
            {
                name += ' ';
                name += (*suffix)[(name != temp->Name1) ? locale : uint8(DEFAULT_LOCALE)];
            }
        }

        std::ostringstream oss;
        oss << "|c" << std::hex << ItemQualityColors[temp->Quality] << std::dec <<
            "|Hitem:" << temp->ItemId << ":" <<
            item->GetEnchantmentId(PERM_ENCHANTMENT_SLOT) << ":" <<
            item->GetEnchantmentId(SOCK_ENCHANTMENT_SLOT) << ":" <<
            item->GetEnchantmentId(SOCK_ENCHANTMENT_SLOT_2) << ":" <<
            item->GetEnchantmentId(SOCK_ENCHANTMENT_SLOT_3) << ":" <<
            item->GetEnchantmentId(BONUS_ENCHANTMENT_SLOT) << ":" <<
            item->GetItemRandomPropertyId() << ":" << item->GetItemSuffixFactor() << ":" <<
            (uint32)item->GetOwner()->GetLevel() << "|h[" << name << "]|h|r";

        F->Push(oss.str());
        return 1;
    }

    /**
     * Returns GUID of the [Player] who currently owns the [Item]
     *
     * @return ObjectGuid guid : guid of the [Player] who owns the [Item]
     */
    int GetOwnerGUID(Forge* F, Item* item)
    {
        F->Push(item->GetOwnerGUID());
        return 1;
    }

    /**
     * Returns the [Player] who currently owns the [Item]
     *
     * @return [Player] player : the [Player] who owns the [Item]
     */
    int GetOwner(Forge* F, Item* item)
    {
        F->Push(item->GetOwner());
        return 1;
    }

    /**
     * Returns the [Item]s stack count
     *
     * @return uint32 count
     */
    int GetCount(Forge* F, Item* item)
    {
        F->Push(item->GetCount());
        return 1;
    }

    /**
     * Returns the [Item]s max stack count
     *
     * @return uint32 maxCount
     */
    int GetMaxStackCount(Forge* F, Item* item)
    {
        F->Push(item->GetMaxStackCount());
        return 1;
    }

    /**
     * Returns the [Item]s current slot
     *
     * @return uint8 slot
     */
    int GetSlot(Forge* F, Item* item)
    {
        F->Push(item->GetSlot());
        return 1;
    }

    /**
     * Returns the [Item]s current bag slot
     *
     * @return uint8 bagSlot
     */
    int GetBagSlot(Forge* F, Item* item)
    {
        F->Push(item->GetBagSlot());
        return 1;
    }

    /**
     * Returns the [Item]s enchantment ID by enchant slot specified
     *
     * @param [EnchantmentSlot] enchantSlot : the enchant slot specified
     * @return uint32 enchantId : the id of the enchant slot specified
     */
    int GetEnchantmentId(Forge* F, Item* item)
    {
        uint32 enchant_slot = F->CHECKVAL<uint32>(2);

        if (enchant_slot >= MAX_INSPECTED_ENCHANTMENT_SLOT)
            return luaL_argerror(F->L, 2, "valid EnchantmentSlot expected");

        F->Push(item->GetEnchantmentId(EnchantmentSlot(enchant_slot)));
        return 1;
    }

    /**
     * Returns the spell ID tied to the [Item] by spell index
     *
     * @param uint32 spellIndex : the spell index specified
     * @return uint32 spellId : the id of the spell
     */
    int GetSpellId(Forge* F, Item* item)
    {
        uint32 index = F->CHECKVAL<uint32>(2);
        if (index >= MAX_ITEM_PROTO_SPELLS)
            return luaL_argerror(F->L, 2, "valid SpellIndex expected");

        F->Push(item->GetTemplate()->Spells[index].SpellId);
        return 1;
    }

    /**
     * Returns the spell trigger tied to the [Item] by spell index
     *
     * @param uint32 spellIndex : the spell index specified
     * @return uint32 spellTrigger : the spell trigger of the specified index
     */
    int GetSpellTrigger(Forge* F, Item* item)
    {
        uint32 index = F->CHECKVAL<uint32>(2);
        if (index >= MAX_ITEM_PROTO_SPELLS)
            return luaL_argerror(F->L, 2, "valid SpellIndex expected");

        F->Push(item->GetTemplate()->Spells[index].SpellTrigger);
        return 1;
    }

    /**
     * Returns class of the [Item]
     *
     * @return uint32 class
     */
    int GetClass(Forge* F, Item* item)
    {
        F->Push(item->GetTemplate()->Class);
        return 1;
    }

    /**
     * Returns subclass of the [Item]
     *
     * @return uint32 subClass
     */
    int GetSubClass(Forge* F, Item* item)
    {
        F->Push(item->GetTemplate()->SubClass);
        return 1;
    }

    /**
    * Returns the ID of the [Item]
    *
    * @return uint32 itemId
    */
    int GetItemId(Forge* F, Item* item)
    {
        F->Push(item->GetTemplate()->ItemId);
        return 1;
    }

    /**
     * Returns the name of the [Item]
     *
     * @return string name
     */
    int GetName(Forge* F, Item* item)
    {
        F->Push(item->GetTemplate()->Name1);
        return 1;
    }

    /**
     * Returns the display ID of the [Item]
     *
     * @return uint32 displayId
     */
    int GetDisplayId(Forge* F, Item* item)
    {
        F->Push(item->GetTemplate()->DisplayInfoID);
        return 1;
    }

    /**
     * Returns the quality of the [Item]
     *
     * @return uint32 quality
     */
    int GetQuality(Forge* F, Item* item)
    {
        F->Push(item->GetTemplate()->Quality);
        return 1;
    }

    /**
    * Returns the flags of the [Item]
    *
    * @return uint32 flags
    */
    int GetFlags(Forge* F, Item* item)
    {
        F->Push(item->GetTemplate()->Flags);
        return 1;
    }

    /**
    * Returns the flags2 of the [Item]
    *
    * @return uint32 flags2
    */
    int GetFlags2(Forge* F, Item* item)
    {
        F->Push(item->GetTemplate()->Flags2);
        return 1;
    }

    /**
    * Returns the extraFlags of the [Item]
    *
    * @return uint32 extraFlags
    */
    int GetExtraFlags(Forge* F, Item* item)
    {
        F->Push(item->GetTemplate()->FlagsCu);
        return 1;
    }

    /**
     * Returns the default purchase count of the [Item]
     *
     * @return uint32 count
     */
    int GetBuyCount(Forge* F, Item* item)
    {
        F->Push(item->GetTemplate()->BuyCount);
        return 1;
    }

    /**
     * Returns the purchase price of the [Item]
     *
     * @return uint32 price
     */
    int GetBuyPrice(Forge* F, Item* item)
    {
        F->Push(item->GetTemplate()->BuyPrice);
        return 1;
    }

    /**
     * Returns the sell price of the [Item]
     *
     * @return uint32 price
     */
    int GetSellPrice(Forge* F, Item* item)
    {
        F->Push(item->GetTemplate()->SellPrice);
        return 1;
    }

    /**
     * Returns the inventory type of the [Item]
     *
     * @return uint32 inventoryType
     */
    int GetInventoryType(Forge* F, Item* item)
    {
        F->Push(item->GetTemplate()->InventoryType);
        return 1;
    }

    /**
     * Returns the [Player] classes allowed to use this [Item]
     *
     * @return uint32 allowableClass
     */
    int GetAllowableClass(Forge* F, Item* item)
    {
        F->Push(item->GetTemplate()->AllowableClass);
        return 1;
    }

    /**
     * Returns the [Player] races allowed to use this [Item]
     *
     * @return uint32 allowableRace
     */
    int GetAllowableRace(Forge* F, Item* item)
    {
        F->Push(item->GetTemplate()->AllowableRace);
        return 1;
    }

    /**
     * Returns the [Item]s level
     *
     * @return uint32 itemLevel
     */
    int GetItemLevel(Forge* F, Item* item)
    {
        F->Push(item->GetTemplate()->ItemLevel);
        return 1;
    }

    /**
     * Returns the minimum level required to use this [Item]
     *
     * @return uint32 requiredLevel
     */
    int GetRequiredLevel(Forge* F, Item* item)
    {
        F->Push(item->GetTemplate()->RequiredLevel);
        return 1;
    }

    /**
     * Returns the amount of stat values on this [Item]
     *
     * @return uint32 statsCount
     */
    int GetStatsCount(Forge* F, Item* item)
    {
        F->Push(item->GetTemplate()->StatsCount);
        return 1;
    }

    /**
     * Returns the stat info of the specified stat slot of this [Item]
     *
     * @param uint8 statSlot : the stat slot specified
     * @return int32 statValue
     * @return int32 statType
     */
    int GetStatInfo(Forge* F, Item* item)
    {
        uint8 statSlot = F->CHECKVAL<uint8>(2);
        int32 statValue = 0;
        int32 statType = 0;

        if (statSlot > 0 && statSlot <= item->GetTemplate()->StatsCount)
        {
            auto& statEntry = item->GetTemplate()->ItemStat[statSlot - 1];
            statValue = statEntry.ItemStatValue;
            statType = statEntry.ItemStatType;
        }

        F->Push(statValue);
        F->Push(statType);
        return 2;
    }

    /**
     * Returns the damage info of the specified damage slot of this [Item]
     *
     * @param uint8 damageSlot : the damage slot specified (1 or 2)
     * @return uint32 damageType
     * @return float minDamage
     * @return float maxDamage
     */
    int GetDamageInfo(Forge* F, Item* item)
    {
        uint8 damageSlot = F->CHECKVAL<uint8>(2);
        uint32 damageType = 0;
        float damageMin = 0;
        float damageMax = 0;

        if (damageSlot > 0 && damageSlot <= MAX_ITEM_PROTO_DAMAGES)
        {
            auto& damageEntry = item->GetTemplate()->Damage[damageSlot - 1];
            damageType = damageEntry.DamageType;
            damageMin = damageEntry.DamageMin;
            damageMax = damageEntry.DamageMax;
        }

        F->Push(damageType);
        F->Push(damageMin);
        F->Push(damageMax);
        return 3;
    }

    /**
     * Returns the base attack speed of this [Item]
     *
     * @return uint32 speed
     */
    int GetSpeed(Forge* F, Item* item)
    {
        F->Push(item->GetTemplate()->Delay);
        return 1;
    }

    /**
     * Returns the base armor of this [Item]
     *
     * @return uint32 armor
     */
    int GetArmor(Forge* F, Item* item)
    {
        F->Push(item->GetTemplate()->Armor);
        return 1;
    }

    /**
     * Returns the max durability of this [Item]
     *
     * @return uint32 maxDurability
     */
    int GetMaxDurability(Forge* F, Item* item)
    {
        F->Push(item->GetUInt32Value(ITEM_FIELD_MAXDURABILITY));
        return 1;
    }

    /**
     * Returns the current durability of this [Item]
     *
     * @return uint32 durabiliy
     */
    int GetDurability(Forge* F, Item* item)
    {
        F->Push(item->GetUInt32Value(ITEM_FIELD_DURABILITY));
        return 1;
    }

    /**
     * Returns the random property ID of this [Item]
     *
     * @return uint32 randomPropertyId
     */
    int GetRandomProperty(Forge* F, Item* item)
    {
        F->Push(item->GetTemplate()->RandomProperty);
        return 1;
    }

    /**
     * Returns the random suffix ID of this [Item]
     *
     * @return uint32 suffixId
     */
    int GetRandomSuffix(Forge* F, Item* item)
    {
        F->Push(item->GetTemplate()->RandomSuffix);
        return 1;
    }

    /**
     * Returns the item set ID of this [Item]
     *
     * @return uint32 itemSetId
     */
    int GetItemSet(Forge* F, Item* item)
    {
        F->Push(item->GetTemplate()->ItemSet);
        return 1;
    }

    /**
     * Returns the bag size of this [Item], 0 if [Item] is not a bag
     *
     * @return uint32 bagSize
     */
    int GetBagSize(Forge* F, Item* item)
    {
        if (Bag* bag = item->ToBag())
            F->Push(bag->GetBagSize());
        else
            F->Push(0);
        return 1;
    }

    /**
     * Sets the [Player] specified as the owner of the [Item]
     *
     * @param [Player] player : the [Player] specified
     */
    int SetOwner(Forge* F, Item* item)
    {
        Player* player = F->CHECKOBJ<Player>(2);

        item->SetOwnerGUID(player->GET_GUID());
        return 0;
    }

    /**
     * Sets the binding of the [Item] to 'true' or 'false'
     *
     * @param bool setBinding
     */
    int SetBinding(Forge* F, Item* item)
    {
        bool soulbound = F->CHECKVAL<bool>(2);

        item->SetBinding(soulbound);
        item->SetState(ITEM_CHANGED, item->GetOwner());

        return 0;
    }

    /**
     * Sets the stack count of the [Item]
     *
     * @param uint32 count
     */
    int SetCount(Forge* F, Item* item)
    {
        uint32 count = F->CHECKVAL<uint32>(2);
        item->SetCount(count);
        return 0;
    }

    /**
     * Sets the specified enchantment of the [Item] to the specified slot
     *
     * @param uint32 enchantId : the ID of the enchant to be applied
     * @param uint32 enchantSlot : the slot for the enchant to be applied to
     * @return bool enchantmentSuccess : if enchantment is successfully set to specified slot, returns 'true', otherwise 'false'
     */
    int SetEnchantment(Forge* F, Item* item)
    {
        Player* owner = item->GetOwner();
        if (!owner)
        {
            F->Push(false);
            return 1;
        }

        uint32 enchant = F->CHECKVAL<uint32>(2);
        if (!sSpellItemEnchantmentStore.LookupEntry(enchant))
        {
            F->Push(false);
            return 1;
        }

        EnchantmentSlot slot = (EnchantmentSlot)F->CHECKVAL<uint32>(3);
        if (slot >= MAX_INSPECTED_ENCHANTMENT_SLOT)
            return luaL_argerror(F->L, 2, "valid EnchantmentSlot expected");

        owner->ApplyEnchantment(item, slot, false);
        item->SetEnchantment(slot, enchant, 0, 0);
        owner->ApplyEnchantment(item, slot, true);
        F->Push(true);
        return 1;
    }

    /* OTHER */
    /**
     * Removes an enchant from the [Item] by the specified slot
     *
     * @param uint32 enchantSlot : the slot for the enchant to be removed from
     * @return bool enchantmentRemoved : if enchantment is successfully removed from specified slot, returns 'true', otherwise 'false'
     */
    int ClearEnchantment(Forge* F, Item* item)
    {
        Player* owner = item->GetOwner();
        if (!owner)
        {
            F->Push(false);
            return 1;
        }

        EnchantmentSlot slot = (EnchantmentSlot)F->CHECKVAL<uint32>(2);
        if (slot >= MAX_INSPECTED_ENCHANTMENT_SLOT)
            return luaL_argerror(F->L, 2, "valid EnchantmentSlot expected");

        if (!item->GetEnchantmentId(slot))
        {
            F->Push(false);
            return 1;
        }

        owner->ApplyEnchantment(item, slot, false);
        item->ClearEnchantment(slot);
        F->Push(true);
        return 1;
    }

    /**
     * Saves the [Item] to the database
     */
    int SaveToDB(Forge* /*E*/, Item* item)
    {
        CharacterDatabaseTransaction trans = CharacterDatabaseTransaction(nullptr);
        item->SaveToDB(trans);
        return 0;
    }
    
    ForgeRegister<Item> ItemMethods[] =
    {
        // Getters
        { "GetOwnerGUID", &LuaItem::GetOwnerGUID },
        { "GetOwner", &LuaItem::GetOwner },
        { "GetCount", &LuaItem::GetCount },
        { "GetMaxStackCount", &LuaItem::GetMaxStackCount },
        { "GetSlot", &LuaItem::GetSlot },
        { "GetBagSlot", &LuaItem::GetBagSlot },
        { "GetEnchantmentId", &LuaItem::GetEnchantmentId },
        { "GetSpellId", &LuaItem::GetSpellId },
        { "GetSpellTrigger", &LuaItem::GetSpellTrigger },
        { "GetItemLink", &LuaItem::GetItemLink },
        { "GetClass", &LuaItem::GetClass },
        { "GetSubClass", &LuaItem::GetSubClass },
        { "GetItemId", &LuaItem::GetItemId },
        { "GetName", &LuaItem::GetName },
        { "GetDisplayId", &LuaItem::GetDisplayId },
        { "GetQuality", &LuaItem::GetQuality },
        { "GetFlags", &LuaItem::GetFlags },
        { "GetFlags2", &LuaItem::GetFlags2 },
        { "GetExtraFlags", &LuaItem::GetExtraFlags },		
        { "GetBuyCount", &LuaItem::GetBuyCount },
        { "GetBuyPrice", &LuaItem::GetBuyPrice },
        { "GetSellPrice", &LuaItem::GetSellPrice },
        { "GetInventoryType", &LuaItem::GetInventoryType },
        { "GetAllowableClass", &LuaItem::GetAllowableClass },
        { "GetAllowableRace", &LuaItem::GetAllowableRace },
        { "GetItemLevel", &LuaItem::GetItemLevel },
        { "GetRequiredLevel", &LuaItem::GetRequiredLevel },
        { "GetStatsCount", &LuaItem::GetStatsCount },
        { "GetRandomProperty", &LuaItem::GetRandomProperty },
        { "GetRandomSuffix", &LuaItem::GetRandomSuffix },
        { "GetItemSet", &LuaItem::GetItemSet },
        { "GetBagSize", &LuaItem::GetBagSize },
        { "GetStatInfo", &LuaItem::GetStatInfo },
        { "GetDamageInfo", &LuaItem::GetDamageInfo },
        { "GetSpeed", &LuaItem::GetSpeed },
        { "GetArmor", &LuaItem::GetArmor },
        { "GetMaxDurability", &LuaItem::GetMaxDurability },
        { "GetDurability", &LuaItem::GetDurability },

        // Setters
        { "SetOwner", &LuaItem::SetOwner },
        { "SetBinding", &LuaItem::SetBinding },
        { "SetCount", &LuaItem::SetCount },

        // Boolean
        { "IsSoulBound", &LuaItem::IsSoulBound },
        { "IsBoundAccountWide", &LuaItem::IsBoundAccountWide },
        { "IsBoundByEnchant", &LuaItem::IsBoundByEnchant },
        { "IsNotBoundToPlayer", &LuaItem::IsNotBoundToPlayer },
        { "IsLocked", &LuaItem::IsLocked },
        { "IsBag", &LuaItem::IsBag },
        { "IsCurrencyToken", &LuaItem::IsCurrencyToken },
        { "IsNotEmptyBag", &LuaItem::IsNotEmptyBag },
        { "IsBroken", &LuaItem::IsBroken },
        { "CanBeTraded", &LuaItem::CanBeTraded },
        { "IsInTrade", &LuaItem::IsInTrade },
        { "IsInBag", &LuaItem::IsInBag },
        { "IsEquipped", &LuaItem::IsEquipped },
        { "HasQuest", &LuaItem::HasQuest },
        { "IsPotion", &LuaItem::IsPotion },
        { "IsWeaponVellum", &LuaItem::IsWeaponVellum },
        { "IsArmorVellum", &LuaItem::IsArmorVellum },
        { "IsRefundExpired", &LuaItem::IsRefundExpired },
        { "IsConjuredConsumable", &LuaItem::IsConjuredConsumable },
        { "SetEnchantment", &LuaItem::SetEnchantment },
        { "ClearEnchantment", &LuaItem::ClearEnchantment },

        // Other
        { "SaveToDB", &LuaItem::SaveToDB }
    };
};
#endif
