// ============================================================================
//                            TESCombatStyle.h
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
//   https://github.com/Ryan-rsm-McKenzie/CommonLibSSE/blob/master/include/RE/T/TESCombatStyle.h
#pragma once
#include "RE/T/TESForm.h"

struct CombatStyleGeneralData  // CSGD
{
	float offensiveMult;                        // 00
	float defensiveMult;                        // 04
	float groupOffensiveMult;                   // 08
	float meleeScoreMult;                       // 0C
	float magicScoreMult;                       // 10
	float rangedScoreMult;                      // 14
	float shoutScoreMult;                       // 18
	float unarmedScoreMult;                     // 1C
	float staffScoreMult;                       // 20
	float avoidThreatChance;                    // 24
};
static_assert(sizeof(CombatStyleGeneralData) == 0x28);

struct CombatStyleMeleeData  // CSME
{
	float attackIncapacitatedMult;              // 00
	float powerAttackIncapacitatedMult;         // 04
	float powerAttackBlockingMult;              // 08
	float bashMult;                             // 0C
	float bashRecoilMult;                       // 10
	float bashAttackMult;                       // 14
	float bashPowerAttackMult;                  // 18
	float specialAttackMult;                    // 1C
};
static_assert(sizeof(CombatStyleMeleeData) == 0x20);

struct CombatStyleCloseRangeData  // CSCR
{
	float circleMult;                           // 00
	float fallbackMult;                         // 04
	float flankDistanceMult;                    // 08
	float stalkTimeMult;                        // 0C
};
static_assert(sizeof(CombatStyleCloseRangeData) == 0x10);

struct CombatStyleLongRangeData  // CSLR
{
	float strafeMult;                           // 0
};
static_assert(sizeof(CombatStyleLongRangeData) == 0x4);

struct CombatStyleFlightData  // CSFL
{
	float hoverChance;                          // 00
	float diveBombChance;                       // 04
	float groundAttackChance;                   // 08
	float hoverTimeMult;                        // 0C
	float groundAttackTimeMult;                 // 10
	float perchAttackChance;                    // 14
	float perchAttackTimeMult;                  // 18
	float flyingAttackChance;                   // 1C
};
static_assert(sizeof(CombatStyleFlightData) == 0x20);

struct TESCombatStyle
{
	// Override
	TESForm                    form;            // 00 public TESForm

	// Add
	CombatStyleGeneralData     generalData;     // 20 - CSGD
	CombatStyleMeleeData       meleeData;       // 48 - CSME
	CombatStyleCloseRangeData  closeRangeData;  // 68 - CSCR
	CombatStyleLongRangeData   longRangeData;   // 78 - CSLR
	CombatStyleFlightData      flightData;      // 7C - CSFL
	uint32_t                   flags;           // 9C - DATA (stl::enumeration<FLAG, uint32_t>)
};
static_assert(sizeof(TESCombatStyle) == 0xA0);