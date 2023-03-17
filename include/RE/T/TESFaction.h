// ============================================================================
//                            TESFaction.h
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
//   https://github.com/Ryan-rsm-McKenzie/CommonLibSSE/blob/master/include/RE/T/TESFaction.h
#pragma once
#include "RE/T/TESObjectREFR.h"

struct FACTION_DATA
{
	enum Flag : uint32_t
	{
		kNone = 0,
		kHiddenFromNPC = 1 << 0,
		kSpecialCombat = 1 << 1,
		kPlayerIsExpelled = 1 << 2,
		kPlayerIsEnemy = 1 << 3,
		kTrackCrime = 1 << 6,
		kIgnoresCrimes_Murder = 1 << 7,
		kIgnoresCrimes_Assult = 1 << 8,
		kIgnoresCrimes_Stealing = 1 << 9,
		kIngoresCrimes_Trespass = 1 << 10,
		kDoNotReportCrimesAgainstMembers = 1 << 11,
		kCrimeGold_UseDefaults = 1 << 12,
		kIgnoresCrimes_Pickpocket = 1 << 13,
		kVendor = 1 << 14,
		kCanBeOwner = 1 << 15,
		kIgnoresCrimes_Werewolf = 1 << 16
	};

	Flag flags;                                                      // 00
};
static_assert(sizeof(FACTION_DATA) == 0x4);

struct FACTION_CRIME_DATA_VALUES  // CRVA
{
	bool                          arrest;                            // 00
	bool                          attackOnSight;                     // 01
	uint16_t                      murderCrimeGold;                   // 02
	uint16_t                      assaultCrimeGold;                  // 04
	uint16_t                      trespassCrimeGold;                 // 06
	uint16_t                      pickpocketCrimeGold;               // 08
	uint16_t                      pad0A;                             // 0A
	float                         stealCrimeGoldMult;                // 0C
	uint16_t                      escapeCrimeGold;                   // 10
	uint16_t                      werewolfCrimeGold;                 // 12
};
static_assert(sizeof(FACTION_CRIME_DATA_VALUES) == 0x14);

struct FACTION_CRIME_DATA
{
	TESObjectREFR*                factionJailMarker;                 // 00 JAIL
	TESObjectREFR*                factionWaitMarker;                 // 08 WAIT
	TESObjectREFR*                factionStolenContainer;            // 10 STOL
	TESObjectREFR*                factionPlayerInventoryContainer;   // 18 PLCN
	uint64_t                      crimeGroup;                        // 20 CRGR (BGSListForm*)
	uint64_t                      jailOutfit;                        // 28 JOUT (BGSOutfit*)
	FACTION_CRIME_DATA_VALUES     crimevalues;                       // 30 CRVA
	uint32_t                      pad44;                             // 44
};
static_assert(sizeof(FACTION_CRIME_DATA) == 0x48);

struct FACTION_VENDOR_DATA_VALUES  // VENV
{
	uint16_t                      startHour;                         // 0
	uint16_t                      endHour;                           // 2
	uint32_t                      locationRadius;                    // 4
	bool                          buysStolen;                        // 8
	bool                          notBuySell;                        // 9
	bool                          buysNonStolen;                     // A
	uint8_t                       padB;                              // B
};
static_assert(sizeof(FACTION_VENDOR_DATA_VALUES) == 0xC);

struct FACTION_VENDOR_DATA  // VENV
{
	FACTION_VENDOR_DATA_VALUES    vendorValues;                      // 00
	uint32_t                      pad0C;                             // 0C
	uint64_t                      vendorLocation;                    // 10 PLVD (PackageLocation*)
	uint64_t                      vendorConditions;                  // 18 (TESCondition*)
	uint64_t                      vendorSellBuyList;                 // 20 VEND (BGSListForm*)
	TESObjectREFR*                merchantContainer;                 // 28 VENC
	uint32_t                      lastDayReset;                      // 30
	uint32_t                      pad34;                             // 34
};
static_assert(sizeof(FACTION_VENDOR_DATA) == 0x38);

struct TESFaction
{
	// Override
	TESForm                       form;	                             // 000 (public TESForm)
	TESFullName                   fullName;	                         // 020 (public TESFullName)
	uint64_t                      reactionForm[4];                   // 030	(public TESReactionForm)

	// Add
	uint64_t                      crimeGoldMap;                      // 050 (BSTHashMap<const TESNPC*, uint32_t>*)
	FACTION_DATA                  data;                              // 058 DATA
	uint32_t                      pad05C;                            // 05C
	FACTION_CRIME_DATA            crimeData;                         // 060
	FACTION_VENDOR_DATA           vendorData;                        // 0A8
	uint64_t                      rankData[2];                       // 0E0 (BSSimpleList<RANK_DATA*>)
	int32_t                       majorCrime;                        // 0F0
	int32_t                       minorCrime;                        // 0F4
	uint32_t                      resistArrestTimeStamp;             // 0F8 (AITimeStamp)
	float                         pcEnemyFlagTimeStamp;              // 0FC current game time in hours
};
static_assert(sizeof(TESFaction) == 0x100);