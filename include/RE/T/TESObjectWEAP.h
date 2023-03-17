// ============================================================================
//                           TESObjectWEAP.h
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
//    https://github.com/Ryan-rsm-McKenzie/CommonLibSSE/blob/master/include/RE/T/TESObjectWEAP.h
#pragma once
#include "RE/B/BGSKeywordForm.h"
#include "RE/T/TESBoundObject.h"
#include "RE/T/TESFullName.h"

enum WEAPON_TYPE
{
	kHandToHandMelee = 0,
	kOneHandSword = 1,
	kOneHandDagger = 2,
	kOneHandAxe = 3,
	kOneHandMace = 4,
	kTwoHandSword = 5,
	kTwoHandAxe = 6,
	kBow = 7,
	kStaff = 8,
	kCrossbow = 9,

	kTotal_WeapType
};

struct WeaponData  // DNAM
{
	uint64_t                     rangedData;               // 00 (RangedData*)
	float                        speed;                    // 08
	float                        reach;                    // 0C
	float                        minRange;                 // 10
	float                        maxRange;                 // 14
	float                        animationAttackMult;      // 18
	float                        unk1C;                    // 1C
	float                        staggerValue;             // 20
	uint32_t                     hitBehavior;              // 24 (stl::enumeration<WEAPONHITBEHAVIOR, uint32_t>)
	uint32_t                     skill;                    // 28 (stl::enumeration<ActorValue, uint32_t>)
	uint32_t                     resistance;               // 2C (stl::enumeration<ActorValue, uint32_t>)
	uint16_t                     flags2;                   // 30 (stl::enumeration<Flag2, uint16_t>)
	uint8_t                      baseVATSToHitChance;      // 32
	uint8_t                      attackAnimation;          // 33 (stl::enumeration<AttackAnimation, uint8_t>)
	uint8_t                      embeddedWeaponAV;         // 34 (stl::enumeration<ActorValue, uint8_t>)
	uint8_t                      animationType;            // 35 (stl::enumeration<WEAPON_TYPE, uint8_t>)
	uint8_t                      flags;                    // 36 (stl::enumeration<Flag, uint8_t>)
	uint8_t                      unk37;                    // 37
};
static_assert(sizeof(WeaponData) == 0x38);

struct TESObjectWEAP
{
	// Override:
	TESBoundObject               boundObj;                 // 000 (public TESBoundObject)
	TESFullName                  fullName;                 // 030 (public TESFullName)
	char                         modTextureSwap[0x38];     // 040 (public TESModelTextureSwap)
	char                         icon[0x10];               // 078 (public TESIcon)
	uint64_t                     enchForm[3];              // 088 (public TESEnchantableForm)
	uint64_t                     valForm[2];               // 0A0 (public TESValueForm)
	uint64_t                     weightForm[2];            // 0B0 (public TESWeightForm)
	uint64_t                     attDamForm[2];            // 0C0 (public TESAttackDamageForm)
	uint64_t                     desObjForm[2];            // 0D0 (public BGSDestructibleObjectForm)
	uint64_t                     equType[2];               // 0E0 (public BGSEquipType)
	uint64_t                     preloadable;              // 0F0 (public BGSPreloadable)
	char                         msgIcon[0x18];            // 0F8 (public BGSMessageIcon)
	uint64_t                     getDropSounds[3];         // 110 (public BGSPickupPutdownSounds)
	uint64_t                     blockBashDat[3];          // 128 (public BGSBlockBashData)
	BGSKeywordForm               keywordForm;              // 140 (public BGSKeywordForm)
	uint64_t                     description[2];           // 158 (public TESDescription)

	// Add:
	WeaponData                   weaponData;               // 168 DNAM
	uint64_t                     criticalData[3];          // 1A0 CRDT (CriticalData)
	uint64_t                     unk1B8;                   // 1B8 (Unk1B8*)
	uint64_t                     attackSound;              // 1C0 SNAM (BGSSoundDescriptorForm*)
	uint64_t                     attackSound2D;            // 1C8 XNAM (BGSSoundDescriptorForm*)
	uint64_t                     attackLoopSound;          // 1D0 NAM7 (BGSSoundDescriptorForm*)
	uint64_t                     attackFailSound;          // 1D8 TNAM (BGSSoundDescriptorForm*) 
	uint64_t                     idleSound;                // 1E0 UNAM (BGSSoundDescriptorForm*)
	uint64_t                     equipSound;               // 1E8 NAM9 (BGSSoundDescriptorForm*)
	uint64_t                     unequipSound;             // 1F0 NAM8 (BGSSoundDescriptorForm*)
	uint64_t                     impactDataSet;            // 1F8 (BGSImpactDataSet*)
	uint64_t                     firstPersonModelObject;   // 200 WNAM (TESObjectSTAT*)
	TESObjectWEAP*               templateWeapon;           // 208 CNAM
	char*                        embeddedNode;             // 210 (BSFixedString)
	uint32_t                     soundLevel;               // 218 VNAM (stl::enumeration<SOUND_LEVEL, uint32_t>)
	uint32_t                     pad21C;                   // 21C
};
static_assert(sizeof(TESObjectWEAP) == 0x220);