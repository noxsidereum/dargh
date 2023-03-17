// ============================================================================
//                           BGSLocation.h
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
//   https://github.com/Ryan-rsm-McKenzie/CommonLibSSE/blob/master/include/RE/B/BGSLocation.h
#pragma once
#include "RE/B/BGSKeywordForm.h"
#include "RE/T/TESFaction.h"
#include "RE/T/TESForm.h"
#include "RE/T/TESFullName.h"

struct BGSLocation
{
	// Override
	TESForm            form;                                 // 00 (public TESForm)
	TESFullName        fullName;                             // 20 (public TESFullName)
	BGSKeywordForm     bgsKeywordForm;                       // 30 (public BGSKeywordForm)

	// Add
	BGSLocation*       parentLoc;                            // 48 - PNAM
	TESFaction*        unreportedCrimeFaction;               // 50 - FNAM
	uint64_t           musicType;                            // 58 - NAM1 (BGSMusicType*)
	uint32_t           worldLocMarker;                       // 60 - MNAM (ObjectRefHandle)
	float              worldLocRadius;                       // 64 - RNAM
	uint32_t           horseLocMarker;                       // 68 - NAM0 (ObjectRefHandle)
	uint32_t           pad6C;                                // 6C
	uint64_t           specialRefs[3];                       // 70 - LCSR (BSTArray<SpecialRefData>)
	uint64_t           uniqueNPCs[3];                        // 88 - LCUN (BSTArray<UniqueNPCData>)
	uint64_t           overrideData;                         // A0 (OverrideData*)
	uint64_t           promoteRefsTask;                      // A8 (NiPointer<QueuedPromoteLocationReferencesTask>)
	uint64_t           promotedRefs[3];                      // B0 (BSTArray<ObjectRefHandle>)
	int32_t            loadedCount;                          // C8
	uint32_t           fileOffset;                           // CC
	uint64_t           keywordData[3];                       // D0 (BSTArray<KEYWORD_DATA>)
	uint32_t           lastChecked;                          // E8
	bool               cleared;                              // EC
	bool               everCleared;                          // ED
	uint16_t           padEE;                                // EE
};
static_assert(sizeof(BGSLocation) == 0xF0);