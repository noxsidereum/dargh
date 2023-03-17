// ============================================================================
//                           TESActorBaseData.h
// ----------------------------------------------------------------------------
// Part of the open-source Dynamic Animation Replacer (DARGH).
// 
// Copyright (c) 2023 Nox Sidereum
// Copyright (c) 2018 Ryan - rsm - McKenzie
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
// Note from Nox Sidereum:
// 
//     In writing DARGH and reverse engineering DAR, I have attempted to be
//     consistent with CommonLibSSE structures and variable names. This
//     should facilitate any future porting of this code to 100% CommonLibSSE
//     (which I'd encourage).
// 
//     Many thanks to Ryan and the authors of CommonLibSSE.
// 
// This particular file is based on
//    https://github.com/Ryan-rsm-McKenzie/CommonLibSSE/blob/master/include/RE/T/TESActorBaseData.h
#pragma once
#include "RE/B/BGSVoiceType.h"

struct ACTOR_BASE_DATA
{
	enum Flag : int32_t
	{
		kNone = 0,
		kFemale = 1 << 0,
		kEssential = 1 << 1,
		kIsChargenFacePreset = 1 << 2,
		kRespawn = 1 << 3,
		kAutoCalcStats = 1 << 4,
		kUnique = 1 << 5,
		kDoesntAffectStealthMeter = 1 << 6,
		kPCLevelMult = 1 << 7,
		kUsesTemplate = 1 << 8,
		kProtected = 1 << 11,
		kSummonable = 1 << 14,
		kDoesntBleed = 1 << 16,
		kBleedoutOverride = 1 << 18,
		kOppositeGenderanims = 1 << 19,
		kSimpleActor = 1 << 20,
		kLoopedScript = 1 << 21,  // ?
		kLoopedAudio = 1 << 28,   // ?
		kIsGhost = 1 << 29,
		kInvulnerable = 1 << 31
	};

	enum TEMPLATE_USE_FLAG : uint16_t
	{
		template_use_kNone = 0,
		kTraits = 1 << 0,
		kStats = 1 << 1,
		kFactions = 1 << 2,
		kSpells = 1 << 3,
		kAIData = 1 << 4,
		kAIPackages = 1 << 5,
		kUnused = 1 << 6,
		kBaseData = 1 << 7,
		kInventory = 1 << 8,
		kScript = 1 << 9,
		kAIDefPackList = 1 << 10,
		kAttackData = 1 << 11,
		kKeywords = 1 << 12
	};

	int32_t            actorBaseFlags;           // 00 (stl::enumeration<Flag, std::uint32_t>)
	int16_t            magickaOffset;            // 04
	int16_t            staminaOffset;            // 06
	uint16_t           level;                    // 08
	uint16_t           calcLevelMin;             // 0A
	uint16_t           calcLevelMax;             // 0C
	uint16_t           speedMult;                // 0E
	uint16_t           baseDisposition;          // 10 - unused
	uint16_t           templateUseFlags;         // 12 (stl::enumeration<TEMPLATE_USE_FLAG, std::uint16_t>)
	int16_t            healthOffset;             // 14
	int16_t            bleedoutOverride;         // 16
};
static_assert(sizeof(ACTOR_BASE_DATA) == 0x18);

struct TESActorBaseData
{
	uint64_t           pVFT;                     // 00
	ACTOR_BASE_DATA    actorData;                // 08
	uint64_t           deathItem;                // 20 - INAM (TESLevItem*)
	BGSVoiceType*      voiceType;                // 28 - VTCK
	TESForm*           baseTemplateForm;         // 30 - TPLT
	TESForm**          templateForms;            // 38
	uint64_t           factions[3];              // 40 (BSTArray<FACTION_RANK>)
};
static_assert(sizeof(TESActorBaseData) == 0x58);