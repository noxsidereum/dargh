// ============================================================================
//                             EffectSetting.h
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
//   https://github.com/Ryan-rsm-McKenzie/CommonLibSSE/blob/master/include/RE/E/EffectSetting.h
#pragma once
#include "RE/A/ActorValues.h"
#include "RE/B/BGSKeywordForm.h"
#include "RE/T/TESForm.h"
#include "RE/T/TESFullName.h"

enum EffectSettingFlag
{
	kNone_effectSetting = 0,
	kHostile = 1 << 0,
	kRecover = 1 << 1,
	kDetrimental = 1 << 2,
	kSnapToNavMesh = 1 << 3,
	kNoHitEvent = 1 << 4,
	kDispelWithKeywords = 1 << 8,
	kNoDuration = 1 << 9,
	kNoMagnitude = 1 << 10,
	kNoArea = 1 << 11,
	kFXPersist = 1 << 12,
	kGoryVisuals = 1 << 14,
	kHideInUI = 1 << 15,
	kNoRecast = 1 << 17,
	kPowerAffectsMagnitude = 1 << 21,
	kPowerAffectsDuration = 1 << 22,
	kPainless = 1 << 26,
	kNoHitEffect = 1 << 27,
	kNoDeathDispel = 1 << 28
};

struct EffectSettingData
{
	uint32_t           effectSettingFlags;		             // 00 (stl::enumeration<Flag, uint32_t>)
	float              baseCost;				             // 04
	TESForm*           associatedForm;		                 // 08
	ActorValue         associatedSkill;		                 // 10
	ActorValue         resistVariable;		                 // 14
	int16_t            numCounterEffects;	                 // 18
    uint16_t           pad1A;				                 // 1A
	uint32_t           pad1C;				                 // 1C
	uint64_t           light;				                 // 20 (TESObjectLIGH*)
	float              taperWeight;			                 // 28
	uint32_t           pad2C;				                 // 2C
	uint64_t           effectShader;			             // 30 (TESEffectShader*)
	uint64_t           enchantShader;		                 // 38 (TESEffectShader*)
	int32_t            minimumSkill;			             // 40
	int32_t            spellmakingArea;		                 // 44
	float              spellmakingChargeTime;                // 48
	float              taperCurve;			                 // 4C
	float              taperDuration;		                 // 50
	float              secondAVWeight;		                 // 54
	uint32_t           archetype;			                 // 58 (Archetype)
	ActorValue         primaryAV;			                 // 5C
	uint64_t           projectileBase;		                 // 60 (BGSProjectile*)
	uint64_t           explosion;			                 // 68 (BGSExplosion*)
	uint32_t           castingType;			                 // 70 (MagicSystem::CastingType)
	uint32_t           delivery;				             // 74 (MagicSystem::Delivery)
	ActorValue         secondaryAV;			                 // 78
	uint64_t           castingArt;			                 // 80 (BGSArtObject*)
	uint64_t           hitEffectArt;			             // 88 (BGSArtObject*)
	uint64_t           impactDataSet;		                 // 90 (BGSImpactDataSet*)
	float              skillUsageMult;		                 // 98
	uint32_t           pad9C;				                 // 9C
	uint64_t           dualCastData;			             // A0 (BGSDualCastData*)
	float              dualCastScale;		                 // A8
	uint32_t           padAC;				                 // AC
	uint64_t           enchantEffectArt;		             // B0 (BGSArtObject*)
	uint64_t           hitVisuals;			                 // B8 (BGSReferenceEffect*)
	uint64_t           enchantVisuals;		                 // C0 (BGSReferenceEffect*)
	uint64_t           equipAbility;			             // C8 (SpellItem*)
	uint64_t           imageSpaceMod;		                 // D0 (TESImageSpaceModifier*)
	uint64_t           perk;					             // D8 (BGSPerk*)
	uint32_t           castingSoundLevel;	                 // E0 (SOUND_LEVEL)
	float              aiScore;				                 // E4
	float              aiDelayTimer;			             // E8
	uint32_t           padEC;				                 // EC
};
static_assert(sizeof(EffectSettingData) == 0xF0);

struct EffectSetting
{
	// Override:
	TESForm            form;				                 // 000
	TESFullName        fullName;			                 // 020
	uint64_t           menuDisplayObj[2];	                 // 030 (public BGSMenuDisplayObject)
	BGSKeywordForm     keywordForm;	                         // 040

	// Add:
	uint64_t           filterValidationFunction;	         // 058 (FilterValidation_t*)
	void*              filterValidationItem;		         // 060
	EffectSettingData  data;				                 // 068 - DATA
	uint64_t           counterEffects[2];			         // 158 - ESCE (BSSimpleList<EffectSetting*>)
	uint64_t           effectSounds[3];				         // 168 - SNDD (BSTArray<SoundPair>)
	char*              magicItemDescription;		         // 180 - DNAM (BSFixedString)
	int32_t            effectLoadedCount;			         // 188
	int32_t            associatedItemLoadedCount;	         // 18C
	uint64_t           conditions;				             // 190 (TESCondition)
};
static_assert(sizeof(EffectSetting) == 0x198);