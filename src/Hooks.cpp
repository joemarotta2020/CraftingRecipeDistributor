#include "Hooks.h"

#include "Distributor.h"

namespace CRAFT::Hooks
{
	namespace
	{
		struct StoryItemCraft
		{
			RE::ObjectRefHandle objectHandle;
			RE::BGSLocation*    location;
			RE::TESForm*        form;
		};
		static_assert(sizeof(StoryItemCraft) == 0x18);

		using CraftFunc_t = StoryItemCraft* (*)(StoryItemCraft*, RE::TESObjectREFR*, RE::BGSLocation*, RE::TESForm*);

		RE::BGSConstructibleObject* GetSelectedConstructible()
		{
			const auto ui = RE::UI::GetSingleton();
			if (!ui) {
				return nullptr;
			}

			const auto craftingMenu = ui->GetMenu<RE::CraftingMenu>();
			if (!craftingMenu || !craftingMenu->subMenu) {
				return nullptr;
			}

			const auto constructibleMenu = skyrim_cast<RE::CraftingSubMenus::ConstructibleObjectMenu*>(craftingMenu->subMenu);
			if (!constructibleMenu || constructibleMenu->currentIndex >= constructibleMenu->recipes.size()) {
				return nullptr;
			}

			return constructibleMenu->recipes[constructibleMenu->currentIndex].constructibleObject;
		}

		struct CraftHook
		{
			static StoryItemCraft* Thunk(StoryItemCraft* a_event, RE::TESObjectREFR* a_bench, RE::BGSLocation* a_location, RE::TESForm* a_form)
			{
				const auto selectedRecipe = GetSelectedConstructible();
				const auto result = _Original(a_event, a_bench, a_location, a_form);
				Manager::GetSingleton()->HandleCraftedItem(selectedRecipe, a_form);
				return result;
			}

			static void Install()
			{
#ifdef SKYRIM_SUPPORT_AE
				const auto offset = REL::Module::IsAE() ? 0x227 : 0x11E;
				REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(50476, 51369), offset };
#else
				REL::Relocation<std::uintptr_t> target{ REL::ID(50476), 0x11E };
#endif

				auto& trampoline = SKSE::GetTrampoline();
				_Original = trampoline.write_call<5>(target.address(), Thunk);
				REX::INFO("Installed CRD breakdown craft hook at {:X}", target.address());
			}

			static inline REL::Relocation<CraftFunc_t> _Original;
		};
	}

	void Install()
	{
#ifndef SKYRIMVR
		SKSE::AllocTrampoline(14);
		CraftHook::Install();
#else
		REX::WARN("Breakdown failure hook is not installed for Skyrim VR");
#endif
	}
}
