// ============================================================================
//                              Offsets.h
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
#pragma once

#include "RE/A/Actor.h"
#include "RE/B/BGSLocation.h"
#include "RE/B/BSReadWriteLock.h"
#include "RE/C/Calendar.h"
#include "RE/H/hkbClipGenerator.h"
#include "RE/H/hkbContext.h"
#include "RE/S/Setting.h"
#include "RE/S/SettingCollectionList.h"
#include "RE/S/Sky.h"
#include "RE/T/TESActorBase.h"
#include "RE/T/TESDataHandler.h"
#include "RE/T/TESFaction.h"
#include "RE/T/TESForm.h"
#include "RE/T/TESWorldSpace.h"

namespace RE
{
	extern TESDataHandler** g_dataHandler;
	extern SettingCollectionList** g_iniSettingCollection;

	typedef Setting* (* _SettingCollectionList_Get)(SettingCollectionList* list, const char* name);
	extern _SettingCollectionList_Get SettingCollectionList_Get;

	extern uint64_t VTBL_hkbCharacterStringData;
	extern uint64_t VTBL_hkbProjectData;
	extern uint64_t VTBL_hkbClipGenerator;
	extern uint64_t Unk00;
	extern uint64_t hkbClipGenerator_Generate;
	extern uint64_t g_AnimationFileManager;
	extern uint64_t QueuedRef_Unk;
	extern uint64_t TESObjectREFR_Unk;
	extern uint64_t WeaponAnimGraphMgrHolder_Unk;
	extern uint64_t SimpleAnimGraphMgrLoadingTask_Unk;

	typedef uint64_t (* _ShowMsgBox)
		(const char* message, uint64_t callbackYes, uint64_t callbackNo, uint64_t callbackCancel,
	     uint64_t unk1, const char* butText, uint64_t unk2);
	extern _ShowMsgBox showMessageBox;

	typedef TESForm* (* _Game_GetForm)(uint32_t id);
	extern _Game_GetForm Game_GetForm;

	typedef uint64_t(* _Runtime_DynamicCast)(TESForm*, uint64_t, uint64_t, uint64_t, uint64_t);
	extern _Runtime_DynamicCast Runtime_DynamicCast;

	extern uint64_t RTTI_TESForm;
	extern uint64_t RTTI_BGSKeywordForm;

	typedef void (* _BSReadWriteLock_LockForRead)(BSReadWriteLock* lock);
	extern _BSReadWriteLock_LockForRead BSReadWriteLock_LockForRead;

	typedef void (* _BSReadWriteLock_UnlockRead)(BSReadWriteLock* lock);
	extern _BSReadWriteLock_UnlockRead BSReadWriteLock_UnlockRead;

	typedef int (* _ActorBase_GetSex)(TESActorBase* actorBase);   // returns -1 (None), 0 (Male) or 1 (Female)
	extern _ActorBase_GetSex ActorBase_GetSex;

	typedef bool (* _MagicTarget_HasMagicEffect)(MagicTarget*, TESForm*);
	extern _MagicTarget_HasMagicEffect MagicTarget_HasMagicEffect;

	typedef bool (* _MagicTarget_HasMagicEffectWithKeyword)(MagicTarget*, TESForm*, uint64_t);
	extern _MagicTarget_HasMagicEffectWithKeyword MagicTarget_HasMagicEffectWithKeyword;

	typedef bool (* _Actor_HasPerk)(Actor* actor, TESForm* perk);
	extern _Actor_HasPerk Actor_HasPerk;

	typedef bool (* _Actor_HasSpell)(Actor* actor, TESForm* spell);
	extern _Actor_HasSpell Actor_HasSpell;

	typedef bool (* _Actor_HasShout)(Actor* actor, TESForm* shout);
	extern _Actor_HasShout Actor_HasShout;

	typedef uint16_t(* _Actor_GetLevel)(Actor* actor);
	extern _Actor_GetLevel Actor_GetLevel;

	extern Calendar** g_theCalendar;

	typedef float (* _Calendar_GetCurrentGameTime)(Calendar* calendar);
	extern _Calendar_GetCurrentGameTime Calendar_GetCurrentGameTime;

	extern float* g_gameHoursPerGameDay;

	typedef Sky* (* _Sky_GetSingleton)(void);
	extern _Sky_GetSingleton Sky_GetSingleton;

	typedef bool (* _Actor_IsRunning)(Actor* actor);
	extern _Actor_IsRunning Actor_IsRunning;

	typedef bool (* _Actor_IsOnMount)(Actor* actor);
	extern _Actor_IsOnMount Actor_IsOnMount;

	typedef bool (* _Actor_IsInAir)(Actor* actor);
	extern _Actor_IsInAir Actor_IsInAir;

	typedef BGSLocation* (* _TESObjectREFR_GetCurrentLocation)(TESObjectREFR* refr);
	extern _TESObjectREFR_GetCurrentLocation ObjectReference_GetCurrentLocation;

	typedef TESWorldSpace* (* _TESObjectREFR_GetWorldspace)(TESObjectREFR*);
	extern _TESObjectREFR_GetWorldspace TESObjectREFR_GetWorldSpace;

	typedef int (* _Actor_GetFactionRank)(Actor* actor, TESFaction* faction, bool isPlayer);
	extern _Actor_GetFactionRank Actor_GetFactionRank;

	extern Actor** g_thePlayer;

	typedef uint8_t (* _Actor_IsMoving)(Actor* actor);
	extern _Actor_IsMoving Actor_IsMoving;

	typedef double (* _Actor_GetMoveDirRelToFacing)(Actor* actor);
	extern _Actor_GetMoveDirRelToFacing Actor_GetMoveDirRelToFacing;

	typedef uint64_t (* _AnimationFileManager_Load)(uint64_t, hkbContext*, hkbClipGenerator*, uint64_t);
	extern _AnimationFileManager_Load AnimationFileManager_Load;

	bool InitialiseOffsets();
	bool DumpSpecificVersion();
	void DumpOffsets();
}