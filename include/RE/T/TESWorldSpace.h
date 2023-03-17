// ============================================================================
//                           TESWorldSpace.h
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
//    https://github.com/Ryan-rsm-McKenzie/CommonLibSSE/blob/master/include/RE/T/TESWorldSpace.h
#pragma once
#include "RE/B/BSString.h"
#include "RE/T/TESForm.h"
#include "RE/T/TESFullName.h"
#include "RE/T/TESModel.h"
#include "RE/T/TESObjectCELL.h"

struct TESWorldSpace
{
	// override:
	TESForm                  form;                       // 000 (public TESForm)
	TESFullName              name;	                     // 020 (public TESFullName)
	TESModel                 model;                      // 030 (public TESModel)

	// add:
	uint64_t                 cellMap[6];                 // 058 (BSTHashMap<CellID, TESObjectCELL*>)
	TESObjectCELL*           persistentCell;             // 088
	uint64_t                 terrainManager;             // 090 (BGSTerrainManager*)
	uint64_t                 climate;                    // 098 CNAM (TESClimate*)
	uint8_t                  flags;                      // 0A0 DATA (stl::enumeration<Flag, std::uint8_t>)
	uint8_t                  unk0A1;                     // 0A1
	uint16_t                 parentUseFlags;             // 0A2 PNAM (stl::enumeration<ParentUseFlag, std::uint16_t>)
	uint32_t                 fixedCenter;                // 0A4 WCTR (ShortPoint)
	uint64_t                 fixedPersistentRefMap[6];   // 0A8 (BSTHashMap<std::uint32_t, BSTArray<NiPointer<TESObjectREFR>>>)
	uint64_t                 mobilePersistentRefs[3];    // 0D8 (BSTArray<NiPointer<TESObjectREFR>>)
	uint64_t                 overlappedMultiboundMap;    // 0F0 (NiTPointerMap<std::uint32_t, BSSimpleList<TESObjectREFR*>*>*)
	TESObjectCELL*           skyCell;                    // 0F8
	uint64_t                 locationMap[6];             // 100 (BSTHashMap<FormID, BGSLocation*>)
	void*                    unk130;                     // 130
	void*                    unk138;                     // 138
	void*                    unk140;                     // 140
	void*                    unk148;                     // 148
	void*                    unk150;                     // 150
	TESWorldSpace*           parentWorld;                // 158 WNAM
	uint64_t                 lightingTemplate;           // 160 LTMP (BGSLightingTemplate*)
	uint64_t                 worldWater;                 // 168 NAM2 (TESWaterForm*)
	uint64_t                 lodWater;                   // 170 NAM3 (TESWaterForm*)
	float                    lodWaterHeight;             // 178 NAM4
	uint32_t                 pad17C;                     // 17C
	uint64_t                 unk180;                     // 180
	char                     worldMapData[0x1C];         // 188 MNAM
	uint64_t                 worldMapOffsetData[2];      // 1A4 ONAM (WORLD_MAP_OFFSET_DATA)
	uint32_t                 pad1B4;                     // 1B4
	uint64_t                 musicType;                  // 1B8 ZNAM (BGSMusicType*)
	uint64_t                 minimumCoords;              // 1C0 (NiPoint2)
	uint64_t                 maximumCoords;              // 1C8 (NiPoint2)
	uint64_t                 unk1D0[6];                  // 1D0 BSTHashMap<TESFile*, OFFSET_DATA*> offsetDataMap? (BSTHashMap<UnkKey, UnkValue>)
	BSString                 editorID;                   // 200 EDID
	float                    defaultLandHeight;          // 210 DNAM~
	float                    defaultWaterHeight;         // 214 ~DNAM
	float                    distantLODMult;             // 218 NAMA
	uint32_t                 pad21C;                     // 21C
	uint64_t                 encounterZone;              // 220 XEZN (BGSEncounterZone*)
	BGSLocation*             location;                   // 228 XLCN (BGSLocation*)
	uint64_t                 canopyShadowTexture;        // 230 TNAM (TESTexture)
	uint64_t                 waterEnvMap[2];             // 240 UNAM (TESTexture)
	char                     largeRefData[0x90];         // 250 RNAM (BGSLargeRefData)
	uint64_t                 unk2E0;                     // 2E0
	uint64_t                 unk2E8[6];                  // 2E8 (BSTHashMap<UnkKey, UnkValue>)
	uint64_t                 unk318[6];                  // 318 (BSTHashMap<UnkKey, UnkValue>)
	float                    northRotation;              // 348
	uint32_t                 pad34C;                     // 34C
	int8_t*                  maxHeightData;              // 350 MHDT
};
static_assert(sizeof(TESWorldSpace) == 0x358);