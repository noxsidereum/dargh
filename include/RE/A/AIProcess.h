// ============================================================================
//                               AIProcess.h
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
//   https://github.com/Ryan-rsm-McKenzie/CommonLibSSE/blob/master/include/RE/A/AIProcess.h
#pragma once
#include "RE/T/TESForm.h"

struct AIProcess
{
	uint64_t           middleLow;                            // 000 (MiddleLowProcessData*)
	uint64_t           middleHigh;                           // 008 (MiddleHighProcessData*)
	uint64_t           high;                                 // 010 (HighProcessData*)
	uint64_t           currentPackage[6];                    // 018 (ActorPackage)
	float              unk48;                                // 048
	uint32_t           unk4C;                                // 04C
	uint64_t           cachedValues;                         // 050 (CachedValues*)
	int32_t            numberItemsActivate;                  // 058
	uint32_t           pad5C;                                // 05C
	uint64_t           objects[2];                           // 060 (BSSimpleList<ObjectstoAcquire*>)
	uint64_t           genericLocations[2];                  // 070 (BSSimpleList<TESObjectREFR*>)
	uint64_t           acquireObject;                        // 080 (ObjectstoAcquire*)
	uint64_t           savedAcquireObject;                   // 088 (ObjectstoAcquire*)
	float              essentialDownTimer;                   // 090
	float              deathTime;                            // 094
	float              trackedDamage;                        // 098
	uint32_t           pad9C;                                // 09C
	uint64_t           forms[3];                             // 0A0 (BSTArray<TESForm*>)
	uint64_t           unkB8[7];                             // 0B8 (Data0B8)
	TESForm*           equippedObjects[2];                   // 0F0 (TESForm* [Hand::kTotal])
	uint64_t           unk100;                               // 100
	uint64_t           unk108;                               // 108
	uint32_t           unk110;                               // 110
	uint32_t           target;                               // 114 (RefHandle)
	uint64_t           unk118;                               // 118
	uint64_t           unk120;                               // 120
	uint64_t           unk128;                               // 128
	uint32_t           unk130;                               // 130
	uint16_t           unk134;                               // 134
	uint8_t            lowProcessFlags;                      // 136 (stl::enumeration<LowProcessFlags, uint8_t>)
	int8_t             processLevel;                         // 137 (stl::enumeration<PROCESS_TYPE, uint8_t>)
	bool               skippedTimeStampForPathing;           // 138
	bool               ignoringCombat;                       // 139
	bool               endAlarmOnActor;                      // 13A
	bool               escortingPlayer;                      // 13B
	uint32_t           pad13C;                               // 13C
};
static_assert(sizeof(AIProcess) == 0x140);