#pragma once

#include "Common.h"

namespace CRAFT
{
	struct BreakdownRecipeInfo
	{
		bool  tanning{ false };
		float difficulty{ 0.0f };
	};

	class SMELT : public CraftingBase
	{
	public:
		void InitData();
		bool CreateRecipe(TYPE a_type, RE::TESBoundObject* a_item, std::int32_t a_numRequired = 1);
		bool CreateRecipe(TYPE a_type, RE::TESBoundObject* a_item, RE::TESForm* a_ingot, std::uint16_t a_numConstructed, std::int32_t a_numRequired = 1);

		float CalculateFailureChance(const BreakdownRecipeInfo& a_info, float a_smithingSkill) const;

		// members
		RE::BGSKeyword* smeltKywd{};
		RE::BGSKeyword* tanningRackKywd{};
		RE::BGSKeyword* forgeKywd{};

		REX::TIniSetting<std::uint16_t> maxWeapAmount{ "SMELT", "Weapon cap", 0 };
		REX::TIniSetting<std::uint16_t> maxArmorAmount{ "SMELT", "Armor cap", 0 };
		REX::TIniSetting<std::uint16_t> maxJewelryAmount{ "SMELT", "Jewelry cap", 0 };
		REX::TIniSetting<std::uint16_t> maxClutterAmount{ "SMELT", "Clutter cap", 0 };

		REX::TIniSetting<bool>  scrapEnabled{ "SCRAP", "Enabled", true };
		REX::TIniSetting<float> scrapYieldMultiplier{ "SCRAP", "YieldMultiplier", 1.0f };

		REX::TIniSetting<bool>  failureEnabled{ "BREAKDOWN_FAILURE", "Enabled", true };
		REX::TIniSetting<float> smeltingBaseFailure{ "BREAKDOWN_FAILURE", "SmeltingBaseFailure", 15.0f };
		REX::TIniSetting<float> smeltingSkillFactor{ "BREAKDOWN_FAILURE", "SmeltingSkillFactor", 1.0f };
		REX::TIniSetting<float> tanningBaseFailure{ "BREAKDOWN_FAILURE", "TanningBaseFailure", 10.0f };
		REX::TIniSetting<float> tanningSkillFactor{ "BREAKDOWN_FAILURE", "TanningSkillFactor", 0.75f };
		REX::TIniSetting<float> minimumFailure{ "BREAKDOWN_FAILURE", "MinimumFailure", 0.0f };
		REX::TIniSetting<float> maximumFailure{ "BREAKDOWN_FAILURE", "MaximumFailure", 75.0f };
		REX::TIniSetting<bool>  notifyFailure{ "BREAKDOWN_FAILURE", "NotifyFailure", true };
		REX::TIniSetting<bool>  debugFailure{ "BREAKDOWN_FAILURE", "DebugLog", false };

		REX::TIniSetting<float> difficultyIron{ "MATERIAL_DIFFICULTY", "Iron", 0.0f };
		REX::TIniSetting<float> difficultySteel{ "MATERIAL_DIFFICULTY", "Steel", 20.0f };
		REX::TIniSetting<float> difficultyAdvancedSteel{ "MATERIAL_DIFFICULTY", "AdvancedSteel", 50.0f };
		REX::TIniSetting<float> difficultySilver{ "MATERIAL_DIFFICULTY", "Silver", 20.0f };
		REX::TIniSetting<float> difficultyGold{ "MATERIAL_DIFFICULTY", "Gold", 20.0f };
		REX::TIniSetting<float> difficultyCorundum{ "MATERIAL_DIFFICULTY", "Corundum", 20.0f };
		REX::TIniSetting<float> difficultyQuicksilver{ "MATERIAL_DIFFICULTY", "Quicksilver", 30.0f };
		REX::TIniSetting<float> difficultyDwarven{ "MATERIAL_DIFFICULTY", "Dwarven", 30.0f };
		REX::TIniSetting<float> difficultyElven{ "MATERIAL_DIFFICULTY", "Elven", 30.0f };
		REX::TIniSetting<float> difficultyChaurus{ "MATERIAL_DIFFICULTY", "Chaurus", 40.0f };
		REX::TIniSetting<float> difficultyBonemold{ "MATERIAL_DIFFICULTY", "Bonemold", 30.0f };
		REX::TIniSetting<float> difficultyOrcish{ "MATERIAL_DIFFICULTY", "Orcish", 50.0f };
		REX::TIniSetting<float> difficultyGlass{ "MATERIAL_DIFFICULTY", "Glass", 70.0f };
		REX::TIniSetting<float> difficultyEbony{ "MATERIAL_DIFFICULTY", "Ebony", 80.0f };
		REX::TIniSetting<float> difficultyStalhrim{ "MATERIAL_DIFFICULTY", "Stalhrim", 80.0f };
		REX::TIniSetting<float> difficultyDaedric{ "MATERIAL_DIFFICULTY", "Daedric", 90.0f };
		REX::TIniSetting<float> difficultyDragon{ "MATERIAL_DIFFICULTY", "Dragon", 100.0f };
		REX::TIniSetting<float> difficultyLeather{ "MATERIAL_DIFFICULTY", "Leather", 0.0f };
		REX::TIniSetting<float> difficultyWood{ "MATERIAL_DIFFICULTY", "Wood", 0.0f };
		REX::TIniSetting<float> difficultyUnknown{ "MATERIAL_DIFFICULTY", "Unknown", 25.0f };

		std::uint32_t weapCount{ 0 };
		std::uint32_t armorCount{ 0 };
		std::uint32_t jewelryCount{ 0 };
		std::uint32_t miscObjCount{ 0 };

		static constexpr RawMap rawMap = {
			{ "ArmorMaterialBearStormcloak"sv, "LeatherStrips"sv },
			{ "ArmorMaterialBlades"sv, "IngotSteel"sv },
			{ "ArmorMaterialDaedric"sv, "IngotEbony"sv },
			{ "ArmorMaterialDragonPlate"sv, "DragonBone"sv },
			{ "ArmorMaterialDragonscale"sv, "DragonScales"sv },
			{ "ArmorMaterialDwarven"sv, "IngotDwarven"sv },
			{ "ArmorMaterialEbony"sv, "IngotEbony"sv },
			{ "ArmorMaterialElven"sv, "IngotIMoonstone"sv },
			{ "ArmorMaterialElvenGilded"sv, "IngotIMoonstone"sv },
			{ "ArmorMaterialFalmer"sv, "ChaurusChitin"sv },
			{ "ArmorMaterialForsworn"sv, "LeatherStrips"sv },
			{ "ArmorMaterialGlass"sv, "IngotMalachite"sv },
			{ "ArmorMaterialHide"sv, "LeatherStrips"sv },
			{ "ArmorMaterialImperialHeavy"sv, "IngotSteel"sv },
			{ "ArmorMaterialImperialLight"sv, "LeatherStrips"sv },
			{ "ArmorMaterialImperialStudded"sv, "LeatherStrips"sv },
			{ "ArmorMaterialIron"sv, "IngotIron"sv },
			{ "ArmorMaterialIronBanded"sv, "IngotIron"sv },
			{ "ArmorMaterialLeather"sv, "LeatherStrips"sv },
			{ "ArmorMaterialMS02Forsworn"sv, "LeatherStrips"sv },
			{ "ArmorMaterialOrcish"sv, "IngotOrichalcum"sv },
			{ "ArmorMaterialPenitus"sv, "LeatherStrips"sv },
			{ "ArmorMaterialScaled"sv, "IngotSteel"sv },
			{ "ArmorMaterialSteel"sv, "IngotSteel"sv },
			{ "ArmorMaterialSteelPlate"sv, "IngotSteel"sv },
			{ "ArmorMaterialStormcloak"sv, "LeatherStrips"sv },
			{ "ArmorMaterialStudded"sv, "IngotIron"sv },
			{ "ArmorMaterialThievesGuild"sv, "LeatherStrips"sv },
			{ "ArmorMaterialThievesGuildLeader"sv, "LeatherStrips"sv },
			{ "DLC1ArmorMaterialDawnguard"sv, "IngotSteel"sv },
			{ "DLC1ArmorMaterialFalmerHardened"sv, "ChaurusChitin"sv },
			{ "DLC1ArmorMaterialHunter"sv, "IngotSteel"sv },
			{ "DLC1ArmorMaterialVampire"sv, "LeatherStrips"sv },
			{ "DLC1LD_CraftingMaterialAetherium"sv, "IngotDwarven"sv },
			{ "DLC1WeapMaterialDragonbone"sv, "DragonBone"sv },
			{ "DLC2ArmorMaterialBonemoldHeavy"sv, "BoneMeal"sv },
			{ "DLC2ArmorMaterialChitinHeavy"sv, "DLC2ChitinPlate"sv },
			{ "DLC2ArmorMaterialChitinLight"sv, "DLC2ChitinPlate"sv },
			{ "DLC2ArmorMaterialMoragTong"sv, "LeatherStrips"sv },
			{ "DLC2ArmorMaterialNordicHeavy"sv, "IngotSteel"sv },
			{ "DLC2ArmorMaterialStalhrimHeavy"sv, "DLC2OreStalhrim"sv },
			{ "DLC2ArmorMaterialStalhrimLight"sv, "DLC2OreStalhrim"sv },
			{ "DLC2WeaponMaterialNordic"sv, "IngotSteel"sv },
			{ "DLC2WeaponMaterialStalhrim"sv, "DLC2OreStalhrim"sv },
			{ "USKPArmorMaterialLinwe"sv, "LeatherStrips"sv },
			{ "USLEEPArmorMaterialBlackguard"sv, "LeatherStrips"sv },
			{ "WeapMaterialDaedric"sv, "IngotEbony"sv },
			{ "WeapMaterialDraugr"sv, "IngotSteel"sv },
			{ "WeapMaterialDraugrHoned"sv, "IngotSteel"sv },
			{ "WeapMaterialDwarven"sv, "IngotDwarven"sv },
			{ "WeapMaterialEbony"sv, "IngotEbony"sv },
			{ "WeapMaterialElven"sv, "IngotIMoonstone"sv },
			{ "WeapMaterialFalmer"sv, "ChaurusChitin"sv },
			{ "WeapMaterialFalmerHoned"sv, "ChaurusChitin"sv },
			{ "WeapMaterialGlass"sv, "IngotMalachite"sv },
			{ "WeapMaterialImperial"sv, "IngotSteel"sv },
			{ "WeapMaterialIron"sv, "IngotIron"sv },
			{ "WeapMaterialOrcish"sv, "IngotOrichalcum"sv },
			{ "WeapMaterialSilver"sv, "ingotSilver"sv },
			{ "WeapMaterialSteel"sv, "IngotSteel"sv },
			{ "WeapMaterialWood"sv, "Firewood01"sv }
		};
		static constexpr RE::FormID tanningRackMat = 0x800E4;

	private:
		void InitScrapData();
		RE::TESBoundObject* GetScrapOutput(RE::TESForm* a_material) const;
		float GetMaterialDifficulty(RE::TESBoundObject* a_item, RE::TESForm* a_recoveryMaterial) const;

		Map<RE::FormID, RE::TESBoundObject*> scrapOutputs{};
	};
}
