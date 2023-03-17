// ============================================================================
//                                TESNPC.h
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
//   https://github.com/Ryan-rsm-McKenzie/CommonLibSSE/blob/master/include/RE/T/TESNPC.h
#pragma once
#include "RE/T/TESActorBase.h"
#include "RE/T/TESClass.h"
#include "RE/T/TESCombatStyle.h"
#include "RE/T/TESFaction.h"
#include "RE/T/TESRace.h"
#include "RE/T/TESRaceForm.h"

struct TESNPC
{
	// override
	TESActorBase             actorBase;           // 000 (public TESActorBase)
	TESRaceForm              raceForm;            // 150 (public TESRaceForm)
	uint64_t                 overridePacks[5];    // 160 (public BGSOverridePackCollection)
	uint64_t                 eventSink;           // 188 (public BSTEventSink<MenuOpenCloseEvent>)

	// add
	uint64_t                 playerSkills[6];     // 190 DNAM (Skills)
	TESClass*                npcClass;            // 1C0 CNAM
	uint64_t                 headRelatedData;     // 1C8 (HeadRelatedData*)
	uint64_t                 giftFilter;          // 1D0 GNAM (BGSListForm*)
	TESCombatStyle*          combatStyle;         // 1D8 ZNAM
	uint32_t                 fileOffset;          // 1E0
	uint32_t                 pad1E4;              // 1E4
	TESRace*                 originalRace;        // 1E8
	TESNPC*                  faceNPC;             // 1F0
	float                    height;              // 1F8 NAM6
	float                    weight;              // 1FC NAM7
	uint64_t                 sounds;              // 200 CSCR (Sounds)
	char*                    shortName;           // 208 SHRT (BSFixedString)
	uint64_t                 farSkin;             // 210 ANAM (TESObjectARMO*)
	uint64_t                 defaultOutfit;       // 218 DOFT (BGSOutfit*)
	uint64_t                 sleepOutfit;         // 220 SOFT (BGSOutfit*)
	uint64_t                 defaultPackList;     // 228 DPLT (BGSListForm*)
	TESFaction*              crimeFaction;        // 230 CRIF
	uint64_t                 headParts;           // 238 PNAM (BGSHeadPart**)
	uint8_t                  numHeadParts;        // 240
	uint8_t                  unk241;              // 241
	uint8_t                  unk242;              // 242
	uint8_t                  unk243;              // 243
	uint8_t                  unk244;              // 244
	uint8_t                  soundLevel;          // 245 NAM8 (stl::enumeration<SOUND_LEVEL, uint8_t>)
	char                     bodyTintColor[4];    // 246 QNAM (Color)
	uint16_t                 pad24A;              // 24A
	uint32_t                 pad24C;              // 24C
	uint64_t                 relationships;       // 250 (BSTArray<BGSRelationship*>*)
	uint64_t                 faceData;            // 258 (FaceData*)
	uint64_t                 tintLayers;          // 260 (BSTArray<Layer*>*)
};
static_assert(sizeof(TESNPC) == 0x268);