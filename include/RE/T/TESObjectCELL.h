// ============================================================================
//                             TESObjectCELL.h
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
//    https://github.com/Ryan-rsm-McKenzie/CommonLibSSE/blob/master/include/RE/T/TESObjectCELL.h
#pragma once
#include "RE/B/BSSpinLock.h"
#include "RE/E/ExtraDataList.h"
#include "RE/T/TESForm.h"
#include "RE/T/TESFullName.h"

struct LOADED_CELL_DATA
{
    void*                    unk000;                   // 000
	uint64_t                 cell3D;                   // 008 (NiPointer<NiNode>)
	void*                    unk010;                   // 010
	void*                    unk018;                   // 018
	void*                    unk020;                   // 020
	uint64_t                 unk028;                   // 028
	uint64_t                 unk030;                   // 030
	uint64_t                 unk038;                   // 038
	uint64_t                 unk040[3];                // 040 (BSTArray<void*>)
	uint64_t                 unk058[3];                // 058 (BSTArray<void*>)
	uint64_t                 unk070[4];                // 070 (NiTMap<TESForm*, ObjectRefHandle>)
	uint64_t                 emittanceLightRefMap[4];  // 090 (NiTMap<ObjectRefHandle, NiNode*>)
	uint64_t                 multiboundRefMap[4];      // 0B0 (NiTMap<ObjectRefHandle, NiPointer<BSMultiBoundNode>>)
	uint64_t                 refMultiboundMap[4];      // 0D0 (NiTMap<BSMultiBoundNode*, ObjectRefHandle>)
	uint64_t                 activatingRefs[2];        // 0F0 (BSSimpleList<ObjectRefHandle>)
	uint64_t                 unk100[2];                // 100 (BSSimpleList<ObjectRefHandle>)
	uint64_t                 unk110;                   // 110
	uint64_t                 unk118[3];                // 118 (BSTArray<void*>)
	uint64_t                 unk130[3];                // 130 (BSTArray<void*>)
	uint64_t                 unk148[3];                // 148 (BSTArray<void*>)
	uint64_t                 encounterZone;            // 160 (BGSEncounterZone*)
	uint64_t                 unk168;                   // 168
	uint64_t                 unk170;                   // 170
	uint64_t                 unk178;                   // 178
};
static_assert(sizeof(LOADED_CELL_DATA) == 0x180);

enum TESObjectCELLFlag : uint16_t  // DATA
{
	kNone_TESObjectCELLFlag = 0,
	kIsInteriorCell = 1 << 0,
	kHasWater = 1 << 1,
	kCanTravelFromHere = 1 << 2,
	kNoLODWater = 1 << 3,
	kHasTempData = 1 << 4,
	kPublicArea = 1 << 5,
	kHandChanged = 1 << 6,
	kShowSky = 1 << 7,
	kUseSkyLighting = 1 << 8
};

struct TESObjectCELL
{
	// Override
	TESForm                  form;                     // 000 (public TESForm)
	TESFullName              fullName;                 // 020 (public TESFullName)

	// Add
	mutable BSSpinLock       grassCreateLock;          // 030
	mutable BSSpinLock       grassTaskLock;            // 038
	uint16_t                 cellFlags;                // 040 (stl::enumeration<Flag, std::uint16_t>)
	uint16_t                 cellGameFlags;            // 042
	uint8_t                  cellState;                // 044 (stl::enumeration<CellState, std::uint8_t>)
	bool                     autoWaterLoaded;          // 045
	bool                     cellDetached;             // 046
	uint8_t                  pad047;                   // 047
	ExtraDataList            extraList;                // 048
	uint64_t                 cellData;                 // 060 XCLL if interior, XCLC if exterior (CellData)
	uint64_t                 cellLand;                 // 068 (CellData)
	float                    waterHeight;              // 070 XCLW
	uint64_t                 navMeshes;                // 078 (CellData)
	uint64_t                 references[6];            // 080 (BSTSet<NiPointer<TESObjectREFR>>)
	TESForm*                 unk0B0;                   // 0B0
	uint64_t                 objectList[3];            // 0B8 (BSTArray<TESObjectREFR*>)
	uint64_t                 unk0D0[3];                // 0D0 (BSTArray<void*>)
	uint64_t                 unk0E8[3];                // 0E8 (BSTArray<void*>)
	uint64_t                 unk100[3];                // 100 (BSTArray<void*>)
	mutable BSSpinLock       spinLock;                 // 118
	uint64_t                 worldSpace;               // 120 (TESWorldSpace*)
	LOADED_CELL_DATA*        loadedData;               // 128
	uint64_t                 lightingTemplate;         // 130 LTMP (BGSLightingTemplate*)
	uint64_t                 unk138;                   // 138
};
static_assert(sizeof(TESObjectCELL) == 0x148);