#include "Smelt.h"

#include "Distributor.h"

#include <cmath>
#include <limits>

namespace CRAFT
{
	namespace
	{
		static constexpr std::array<std::pair<std::string_view, std::string_view>, 11> scrapMap = {
			std::pair{ "IngotIron"sv, "DRIT_ScrapIron"sv },
			std::pair{ "IngotSteel"sv, "DRIT_ScrapSteel"sv },
			std::pair{ "ingotSilver"sv, "DRIT_ScrapSilver"sv },
			std::pair{ "IngotGold"sv, "DRIT_ScrapGold"sv },
			std::pair{ "IngotCorundum"sv, "DRIT_ScrapCorundum"sv },
			std::pair{ "IngotDwarven"sv, "DRIT_ScrapDwarven"sv },
			std::pair{ "IngotIMoonstone"sv, "DRIT_ScrapMoonstone"sv },
			std::pair{ "IngotQuicksilver"sv, "DRIT_ScrapQuicksilver"sv },
			std::pair{ "IngotOrichalcum"sv, "DRIT_ScrapOrichalcum"sv },
			std::pair{ "IngotMalachite"sv, "DRIT_ScrapMalachite"sv },
			std::pair{ "IngotEbony"sv, "DRIT_ScrapEbony"sv }
		};
	}

	void SMELT::InitData()
	{
		forgeKywd = RE::TESForm::LookupByID<RE::BGSKeyword>(0x00088105);        //CraftingSmithingForge
		smeltKywd = RE::TESForm::LookupByID<RE::BGSKeyword>(0x000A5CCE);        //CraftingSmithingSmelter
		tanningRackKywd = RE::TESForm::LookupByID<RE::BGSKeyword>(0x0007866A);  //CraftingSmithingTanningRack

		CraftingBase::InitData(rawMap);
		InitScrapData();
	}

	void SMELT::InitScrapData()
	{
		scrapOutputs.clear();

		if (!scrapEnabled.GetValue()) {
			REX::INFO("SCRAP conversion disabled");
			return;
		}

		for (const auto& [materialEDID, scrapEDID] : scrapMap) {
			const auto material = RE::TESForm::LookupByEditorID<RE::TESBoundObject>(materialEDID);
			const auto scrap = RE::TESForm::LookupByEditorID<RE::TESBoundObject>(scrapEDID);
			if (material && scrap) {
				scrapOutputs.insert_or_assign(material->GetFormID(), scrap);
			} else {
				REX::WARN("SCRAP mapping unavailable : {} -> {}", materialEDID, scrapEDID);
			}
		}

		REX::INFO("SCRAP mappings available : {}", scrapOutputs.size());
	}

	RE::TESBoundObject* SMELT::GetScrapOutput(RE::TESForm* a_material) const
	{
		if (!a_material) {
			return nullptr;
		}

		if (const auto it = scrapOutputs.find(a_material->GetFormID()); it != scrapOutputs.end()) {
			return it->second;
		}

		return nullptr;
	}

	float SMELT::GetMaterialDifficulty(RE::TESBoundObject* a_item, RE::TESForm* a_recoveryMaterial) const
	{
		if (a_item) {
			const auto has = [&](std::string_view a_keyword) {
				return a_item->HasKeywordString(a_keyword);
			};

			if (has("ArmorMaterialDaedric"sv) || has("WeapMaterialDaedric"sv)) {
				return difficultyDaedric.GetValue();
			}
			if (has("ArmorMaterialDragonPlate"sv) || has("ArmorMaterialDragonscale"sv) || has("DLC1WeapMaterialDragonbone"sv)) {
				return difficultyDragon.GetValue();
			}
			if (has("DLC2ArmorMaterialStalhrimHeavy"sv) || has("DLC2ArmorMaterialStalhrimLight"sv) || has("DLC2WeaponMaterialStalhrim"sv)) {
				return difficultyStalhrim.GetValue();
			}
			if (has("ArmorMaterialEbony"sv) || has("WeapMaterialEbony"sv)) {
				return difficultyEbony.GetValue();
			}
			if (has("ArmorMaterialGlass"sv) || has("WeapMaterialGlass"sv)) {
				return difficultyGlass.GetValue();
			}
			if (has("ArmorMaterialOrcish"sv) || has("WeapMaterialOrcish"sv)) {
				return difficultyOrcish.GetValue();
			}
			if (has("DLC2ArmorMaterialChitinHeavy"sv) || has("DLC2ArmorMaterialChitinLight"sv) ||
				has("ArmorMaterialFalmer"sv) || has("DLC1ArmorMaterialFalmerHardened"sv) ||
				has("WeapMaterialFalmer"sv) || has("WeapMaterialFalmerHoned"sv)) {
				return difficultyChaurus.GetValue();
			}
			if (has("DLC2ArmorMaterialBonemoldHeavy"sv)) {
				return difficultyBonemold.GetValue();
			}
			if (has("ArmorMaterialSteelPlate"sv) || has("DLC2ArmorMaterialNordicHeavy"sv) || has("DLC2WeaponMaterialNordic"sv)) {
				return difficultyAdvancedSteel.GetValue();
			}
			if (has("ArmorMaterialDwarven"sv) || has("WeapMaterialDwarven"sv) || has("DLC1LD_CraftingMaterialAetherium"sv)) {
				return difficultyDwarven.GetValue();
			}
			if (has("ArmorMaterialElven"sv) || has("ArmorMaterialElvenGilded"sv) || has("WeapMaterialElven"sv)) {
				return difficultyElven.GetValue();
			}
			if (has("WeapMaterialSilver"sv)) {
				return difficultySilver.GetValue();
			}
			if (has("ArmorMaterialBlades"sv) || has("ArmorMaterialImperialHeavy"sv) || has("ArmorMaterialScaled"sv) ||
				has("ArmorMaterialSteel"sv) || has("DLC1ArmorMaterialDawnguard"sv) || has("DLC1ArmorMaterialHunter"sv) ||
				has("WeapMaterialDraugr"sv) || has("WeapMaterialDraugrHoned"sv) || has("WeapMaterialImperial"sv) ||
				has("WeapMaterialSteel"sv)) {
				return difficultySteel.GetValue();
			}
			if (has("ArmorMaterialIron"sv) || has("ArmorMaterialIronBanded"sv) || has("ArmorMaterialStudded"sv) || has("WeapMaterialIron"sv)) {
				return difficultyIron.GetValue();
			}
			if (has("WeapMaterialWood"sv)) {
				return difficultyWood.GetValue();
			}
			if (has("ArmorMaterialBearStormcloak"sv) || has("ArmorMaterialForsworn"sv) || has("ArmorMaterialHide"sv) ||
				has("ArmorMaterialImperialLight"sv) || has("ArmorMaterialImperialStudded"sv) || has("ArmorMaterialLeather"sv) ||
				has("ArmorMaterialMS02Forsworn"sv) || has("ArmorMaterialPenitus"sv) || has("ArmorMaterialStormcloak"sv) ||
				has("ArmorMaterialThievesGuild"sv) || has("ArmorMaterialThievesGuildLeader"sv) || has("DLC1ArmorMaterialVampire"sv) ||
				has("DLC2ArmorMaterialMoragTong"sv) || has("USKPArmorMaterialLinwe"sv) || has("USLEEPArmorMaterialBlackguard"sv)) {
				return difficultyLeather.GetValue();
			}
		}

		const std::string_view recoveryEDID = a_recoveryMaterial ? a_recoveryMaterial->GetFormEditorID() : "";
		if (recoveryEDID == "IngotIron"sv) {
			return difficultyIron.GetValue();
		}
		if (recoveryEDID == "IngotSteel"sv) {
			return difficultySteel.GetValue();
		}
		if (recoveryEDID == "ingotSilver"sv) {
			return difficultySilver.GetValue();
		}
		if (recoveryEDID == "IngotGold"sv) {
			return difficultyGold.GetValue();
		}
		if (recoveryEDID == "IngotCorundum"sv) {
			return difficultyCorundum.GetValue();
		}
		if (recoveryEDID == "IngotQuicksilver"sv) {
			return difficultyQuicksilver.GetValue();
		}
		if (recoveryEDID == "IngotDwarven"sv) {
			return difficultyDwarven.GetValue();
		}
		if (recoveryEDID == "IngotIMoonstone"sv) {
			return difficultyElven.GetValue();
		}
		if (recoveryEDID == "IngotOrichalcum"sv) {
			return difficultyOrcish.GetValue();
		}
		if (recoveryEDID == "IngotMalachite"sv) {
			return difficultyGlass.GetValue();
		}
		if (recoveryEDID == "IngotEbony"sv) {
			return difficultyEbony.GetValue();
		}
		if (recoveryEDID == "DLC2OreStalhrim"sv) {
			return difficultyStalhrim.GetValue();
		}
		if (recoveryEDID == "DragonBone"sv || recoveryEDID == "DragonScales"sv) {
			return difficultyDragon.GetValue();
		}
		if (recoveryEDID == "ChaurusChitin"sv || recoveryEDID == "DLC2ChitinPlate"sv) {
			return difficultyChaurus.GetValue();
		}
		if (recoveryEDID == "BoneMeal"sv) {
			return difficultyBonemold.GetValue();
		}
		if (recoveryEDID == "LeatherStrips"sv) {
			return difficultyLeather.GetValue();
		}
		if (recoveryEDID == "Firewood01"sv) {
			return difficultyWood.GetValue();
		}

		return difficultyUnknown.GetValue();
	}

	float SMELT::CalculateFailureChance(const BreakdownRecipeInfo& a_info, float a_smithingSkill) const
	{
		if (!failureEnabled.GetValue()) {
			return 0.0f;
		}

		const auto base = a_info.tanning ? tanningBaseFailure.GetValue() : smeltingBaseFailure.GetValue();
		const auto factor = a_info.tanning ? tanningSkillFactor.GetValue() : smeltingSkillFactor.GetValue();

		auto chance = base + ((a_info.difficulty - a_smithingSkill) * factor);
		const auto minimum = std::clamp(std::min(minimumFailure.GetValue(), maximumFailure.GetValue()), 0.0f, 100.0f);
		const auto maximum = std::clamp(std::max(minimumFailure.GetValue(), maximumFailure.GetValue()), 0.0f, 100.0f);
		return std::clamp(chance, minimum, maximum);
	}

	bool SMELT::CreateRecipe(TYPE a_type, RE::TESBoundObject* a_item, std::int32_t a_numRequired)
	{
		auto formID = a_item->GetFormID();

		auto it = formidMap.find(formID);
		if (it == formidMap.end()) {
			return false;
		}

		return CreateRecipe(a_type, a_item, it->second.form, it->second.count, a_numRequired);
	}

	bool SMELT::CreateRecipe(TYPE a_type, RE::TESBoundObject* a_item, RE::TESForm* a_ingot, std::uint16_t a_numConstructed, std::int32_t a_numRequired)
	{
		if (!a_ingot || IsBlacklisted(a_item)) {
			return false;
		}

		const auto originalMaterial = a_ingot;
		const auto isTanning = originalMaterial->GetFormID() == tanningRackMat;

		const auto factory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::BGSConstructibleObject>();

		if (auto constructibleObj = factory ? factory->Create() : nullptr) {
			constructibleObj->benchKeyword = isTanning ? tanningRackKywd : smeltKywd;
			constructibleObj->requiredItems.AddObjectToContainer(a_item, a_numRequired, nullptr);

			RE::TESConditionItem* equippedNode = nullptr;
			if (a_type != TYPE::kClutter) {
				equippedNode = new RE::TESConditionItem;
				equippedNode->next = nullptr;
				equippedNode->data.flags.isOR = true;
				equippedNode->data.comparisonValue.f = 0.0f;
				equippedNode->data.functionData.function = RE::FUNCTION_DATA::FunctionID::kGetEquipped;
				equippedNode->data.functionData.params[0] = a_item;
			}
			auto itemCountNode = new RE::TESConditionItem;
			itemCountNode->next = equippedNode;
			itemCountNode->data.comparisonValue.f = static_cast<float>(a_numRequired);
			itemCountNode->data.flags.opCode = RE::CONDITION_ITEM_DATA::OpCode::kGreaterThanOrEqualTo;
			itemCountNode->data.functionData.function = RE::FUNCTION_DATA::FunctionID::kGetItemCount;
			itemCountNode->data.functionData.params[0] = a_item;

			constructibleObj->conditions.head = itemCountNode;

			std::uint16_t numConstructed = a_numConstructed;
			if (numConstructed == 0) {
				RE::TESBoundObject* item = a_item;
				if (auto armor = item->As<RE::TESObjectARMO>(); armor && armor->templateArmor) {
					while (armor && armor->templateArmor) {
						armor = armor->templateArmor;
					}
					item = armor;
				} else if (auto weap = item->As<RE::TESObjectWEAP>(); weap && weap->templateWeapon) {
					while (weap && weap->templateWeapon) {
						weap = weap->templateWeapon;
					}
					item = weap;
				}

				if (const auto cobj = Manager::GetSingleton()->FindConstructible(forgeKywd, item)) {
					numConstructed = static_cast<std::uint16_t>(cobj->requiredItems.GetObjectCount(static_cast<RE::TESBoundObject*>(originalMaterial)));
				}

				if (numConstructed == 0) {
					auto itemGold = item->GetGoldValue();
					auto ingotGold = originalMaterial->GetGoldValue();

					auto itemWeight = item->GetWeight();
					auto ingotWeight = originalMaterial->GetWeight();

					if (itemGold > 0 && ingotGold > 0 && itemWeight > 0.0f && ingotWeight > 0.0f) {
						numConstructed = static_cast<std::uint16_t>(std::min(itemWeight / ingotWeight, itemGold * 0.5f / ingotGold));
					} else if (itemWeight > 0.0f && ingotWeight > 0.0f) {
						numConstructed = static_cast<std::uint16_t>(itemWeight / ingotWeight);
					} else if (itemGold > 0 && ingotGold > 0) {
						numConstructed = static_cast<std::uint16_t>(itemGold * 0.5f / ingotGold);
					}
				}
			}

			std::uint16_t cap = 0;
			switch (a_type) {
			case TYPE::kArmor:
				cap = maxArmorAmount;
				break;
			case TYPE::kWeap:
				cap = maxWeapAmount;
				break;
			case TYPE::kJewel:
				cap = maxJewelryAmount;
				break;
			case TYPE::kClutter:
				cap = maxClutterAmount;
				break;
			default:
				std::unreachable();
			}
			if (cap > 0) {
				numConstructed = std::clamp(numConstructed, static_cast<std::uint16_t>(1), cap);
			} else if (numConstructed == 0) {
				numConstructed = 1;
			}

			auto output = originalMaterial->As<RE::TESBoundObject>();
			auto finalCount = numConstructed;

			if (!isTanning && scrapEnabled.GetValue()) {
				if (auto scrap = GetScrapOutput(originalMaterial)) {
					output = scrap;
					const auto multiplier = std::max(0.0f, scrapYieldMultiplier.GetValue());
					const auto scaled = std::floor(static_cast<float>(numConstructed) * 2.0f * multiplier);
					const auto scrapCount = std::clamp<std::uint32_t>(
						static_cast<std::uint32_t>(std::max(1.0f, scaled)),
						1,
						std::numeric_limits<std::uint16_t>::max());
					finalCount = static_cast<std::uint16_t>(scrapCount);
				}
			}

			if (!output) {
				return false;
			}

			constructibleObj->createdItem = output;
			constructibleObj->data.numConstructed = finalCount;

			const BreakdownRecipeInfo breakdownInfo{
				.tanning = isTanning,
				.difficulty = GetMaterialDifficulty(a_item, originalMaterial)
			};
			Manager::GetSingleton()->AddGeneratedConstructible(constructibleObj, breakdownInfo);

			switch (a_type) {
			case TYPE::kArmor:
				armorCount++;
				break;
			case TYPE::kWeap:
				weapCount++;
				break;
			case TYPE::kJewel:
				jewelryCount++;
				break;
			case TYPE::kClutter:
				miscObjCount++;
				break;
			default:
				std::unreachable();
			}

			return true;
		}

		return false;
	}
}
