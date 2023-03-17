// ============================================================================
//                              Offsets.cpp
// ----------------------------------------------------------------------------
// Part of the open-source Dynamic Animation Replacer (DARGH).
// 
// Copyright (c) 2023 Nox Sidereum
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the “Software”), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is furnished
// to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
// 
// (The MIT License)
// ============================================================================
#include "RE/Offsets.h"

#include "versionlibdb.h"

#include <vector>
#include <unordered_map>

namespace RE
{
	TESDataHandler** g_dataHandler;
	SettingCollectionList** g_iniSettingCollection;
	Actor** g_thePlayer;
	Calendar** g_theCalendar;

	_SettingCollectionList_Get SettingCollectionList_Get;
	uint64_t VTBL_hkbCharacterStringData;
	uint64_t VTBL_hkbProjectData;
	uint64_t VTBL_hkbClipGenerator;
	uint64_t Unk00;
	uint64_t hkbClipGenerator_Generate;
	uint64_t g_AnimationFileManager;
	uint64_t QueuedRef_Unk;
	uint64_t TESObjectREFR_Unk;
	uint64_t WeaponAnimGraphMgrHolder_Unk;
	uint64_t SimpleAnimGraphMgrLoadingTask_Unk;

	_ShowMsgBox showMessageBox;
	_Game_GetForm Game_GetForm;
	_Runtime_DynamicCast Runtime_DynamicCast;
	uint64_t RTTI_TESForm;
	uint64_t RTTI_BGSKeywordForm;
	_BSReadWriteLock_LockForRead BSReadWriteLock_LockForRead;
	_BSReadWriteLock_UnlockRead BSReadWriteLock_UnlockRead;
	_ActorBase_GetSex ActorBase_GetSex;
	_MagicTarget_HasMagicEffect MagicTarget_HasMagicEffect;
	_MagicTarget_HasMagicEffectWithKeyword MagicTarget_HasMagicEffectWithKeyword;
	_Actor_HasPerk Actor_HasPerk;
	_Actor_HasSpell Actor_HasSpell;
	_Actor_HasShout Actor_HasShout;
	_Actor_GetLevel Actor_GetLevel;
	_Calendar_GetCurrentGameTime Calendar_GetCurrentGameTime;
	float* g_gameHoursPerGameDay;
	_Sky_GetSingleton Sky_GetSingleton;
	_Actor_IsRunning Actor_IsRunning;
	_Actor_IsOnMount Actor_IsOnMount;
	_Actor_IsInAir Actor_IsInAir;
	_TESObjectREFR_GetCurrentLocation ObjectReference_GetCurrentLocation;
	_TESObjectREFR_GetWorldspace TESObjectREFR_GetWorldSpace;
	_Actor_GetFactionRank Actor_GetFactionRank;
	_Actor_IsMoving Actor_IsMoving;
	_Actor_GetMoveDirRelToFacing Actor_GetMoveDirRelToFacing;
	_AnimationFileManager_Load AnimationFileManager_Load;

	std::vector<std::tuple<void*, uint64_t, uint64_t>> skyrim_addr
	{
		// ====================================================================================================
		// Skyrim Function / Structure Name      SKSE Addr Lib ID    Addtl Offset        1.6.659 Offset
		// ----------------------------------------------------------------------------------------------------
		{ &g_dataHandler,                               400269,         0  },     //   1  [0x01f52138]
		{ &g_iniSettingCollection,                      411155,         0  },     //   2  [0x030b25d8]
		{ &SettingCollectionList_Get,                    22787,         0  },     //   3  [0x00327140]

		// 3 hooks: ----------------------
		{ &VTBL_hkbCharacterStringData,                 226201,         0  },     //   4  [0x018044a8]
		{ &VTBL_hkbProjectData,                         226870,         0  },     //   5  [0x0180ac48]
		{ &VTBL_hkbClipGenerator,                       226785,         0  },     //   6  [0x01809ef8]
		// -------------------------------

		// 2 trampolines: ----------------
		{ &Unk00,                                        63846,       257  },     //   7  [0x00b371c0] => 0x00b371c0 + 0x101 = 0x00B372C1
		{ &hkbClipGenerator_Generate,                    59253,       128  },     //   8  [0x00a42e50] => 0x00a42e50 + 0x80 = 0x00A42ED0
		// -------------------------------

		// Unused by DARGH; used by DAR (see entry 41 below):
		{ &g_AnimationFileManager,                      407512,         0  },     //   9  [0x02fe6d60]

		// 4 (unused) trampolines: ------
		{ &QueuedRef_Unk,                                13034,       199  },     //  10  [0x001507b0] => 0x001507b0 + 0xc7 =  0x00150877
		{ &TESObjectREFR_Unk,                            20103,       274  },     //  11  [0x002b6cf0] => 0x002b6cf0 + 0x112 = 0x002B6E02
		{ &WeaponAnimGraphMgrHolder_Unk,                 32892,       313  },     //  12  [0x0050bc10] => 0x0050bc10 + 0x139 = 0x0050BD49
		{ &SimpleAnimGraphMgrLoadingTask_Unk,            32918,       281  },     //  13  [0x0050cdd0] => 0x0050cdd0 + 0x119 = 0x0050CEE9
		// ------------------------------

		// Used for evaluating conditions for conditional dynamic animations: ----
		{ &showMessageBox,                               52269,         0  },     //  14  [0x008ec2f0]
		{ &Game_GetForm,                                 14617,         0  },     //  15  [0x001a0b70]
		{ &Runtime_DynamicCast,                         109689,         0  },     //  16  [0x0147165a]
		{ &RTTI_TESForm,                                392216,         0  },     //  17  [0x01e9f9b8]
		{ &RTTI_BGSKeywordForm,                         392223,         0  },     //  18  [0x01e9fa88]
		{ &BSReadWriteLock_LockForRead,                  68233,         0  },     //  19  [0x00c3e090]
		{ &BSReadWriteLock_UnlockRead,                   68239,         0  },     //  20  [0x00c3e350]
		{ &ActorBase_GetSex,                             24374,         0  },     //  21  [0x00369220]
		{ &MagicTarget_HasMagicEffect,                   34517,         0  },     //  22  [0x00575370]
		{ &MagicTarget_HasMagicEffectWithKeyword,        34518,         0  },     //  23  [0x00575400]
		{ &Actor_HasPerk,                                37698,         0  },     //  24  [0x00632b00]
		{ &Actor_HasSpell,                               38782,         0  },     //  25  [0x0066a950]
		{ &Actor_HasShout,                               38783,         0  },     //  26  [0x0066a990]
		{ &Actor_GetLevel,                               37334,         0  },     //  27  [0x0060ca10]
		{ &g_theCalendar,                               400447,         0  },     //  28  [0x01f54780]
		{ &Calendar_GetCurrentGameTime,                  36303,         0  },     //  29  [0x005d96e0]
		{ &g_gameHoursPerGameDay,                       195681,         0  },     //  30  [0x0168f064]
		{ &Sky_GetSingleton,                             13878,         0  },     //  31  [0x00183330]
		{ &Actor_IsRunning,                              37234,         0  },     //  32  [0x00607590]
		{ &Actor_IsOnMount,                              17965,         0  },     //  33  [0x00240740]
		{ &Actor_IsInAir,                                37243,         0  },     //  34  [0x00607ae0]
		{ &ObjectReference_GetCurrentLocation,           19812,         0  },     //  35  [0x002ad8e0]
		{ &TESObjectREFR_GetWorldSpace,                  19816,         0  },     //  36  [0x002adb70]
		{ &Actor_GetFactionRank,                         37676,         0  },     //  37  [0x006305b0]
		{ &g_thePlayer,                                 403521,         0  },     //  38  [0x02fbbb58]
		{ &Actor_IsMoving,                               37953,         0  },     //  39  [0x00642130]
		{ &Actor_GetMoveDirRelToFacing,                  37960,         0  },     //  40  [0x00642340]

		// Use this to avoid deriving it via g_AnimFileManager VFT (which is what DAR does):
		{ &AnimationFileManager_Load,                    63982,         0  }      //  41  [0x00B40D10]
		// ====================================================================================================
	};

	bool InitialiseOffsets()
	{
		VersionDb db;

		// Load database with current executable version.
		if (!db.Load())
		{
			_FATALERROR("Failed to load version database for current executable!");
			return false;
		}
		else
		{
			_MESSAGE("Loaded database for %s version %s.",
				     db.GetModuleName().c_str(), db.GetLoadedVersionString().c_str());
		}

		// Iterate over the function vector. Look up the address for each ID, then add 
		// any additional offset (as specified in the vector) to it. These addresses already
		// include the base address of module so we can use them directly. Store the result
		// back against the function pointer in the vector. N.B. The structured vector
		// bindings we use in this loop require C++ 17 or later.
		uint64_t addr = NULL;
		for (auto& i : skyrim_addr)
		{
			auto ptr = std::get<0>(i);
			auto id = std::get<1>(i);
			auto offset = std::get<2>(i);
			addr = (uint64_t)db.FindAddressById(id);
			if (addr == NULL)
			{
				_FATALERROR("Failed to find address!");
				return false;
			}
			*(uint64_t*)ptr = (addr + offset);
		}

		// Everything was successful.
		return true;
	}

	// DEBUGGING FUNCTIONS
	// Based on the sample code in the "How to Use" section at 
	// https://www.nexusmods.com/skyrimspecialedition/mods/32444
	bool DumpSpecificVersion()
	{
		static int versions[] = { 353, 640, 659, -1 };
		for (int i = 0; versions[i] >= 0; i++)
		{
			VersionDb* db = new VersionDb();
			char version[20];
			sprintf_s(version, 20, "1.6.%d.0", versions[i]);

			// Try to load database version regardless of running executable version.
			if (!db->Load(1, 6, versions[i], 0))
			{
				_FATALERROR("Failed to load version database!");
				delete db;
				return false;
			}

			// Write out a file called offsets-1.6.659.0.txt where each line is the ID and offset.
			// This file will be saved in the main Skyrim folder.
			char fName[50];
			char msg[50];
			sprintf_s(fName, 50, "offsets-%s", version);
			db->Dump(fName);
			sprintf_s(msg, 50, "Dumped offsets for %s", version);
			_MESSAGE(msg);
			delete db;
		}
		return true;
	}

	void DumpOffsets()
	{
		_MESSAGE("Dumping address vector (address in hex, ID in dec, any additional offset in hex).");
		for (auto& [ptr, id, addtl_offset] : skyrim_addr)
		{
			(addtl_offset > 0) ?
				_MESSAGE("%#010x   %d   %#010x", *(uint64_t*)ptr, id, addtl_offset) :
				_MESSAGE("%#010x   %d", *(uint64_t*)ptr, id);
		}
		_MESSAGE("\n");
	}
}