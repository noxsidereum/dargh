// ============================================================================
//                          InventoryChanges.h
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
//    https://github.com/Ryan-rsm-McKenzie/CommonLibSSE/blob/master/include/RE/I/InventoryChanges.h
#pragma once
#include "RE/T/TESBoundObject.h"
#include "RE/T/TESObjectREFR.h"

struct BSTSimpleList_pExtraDataList
{
	ExtraDataList*                       item;          // 00
	BSTSimpleList_pExtraDataList*        next;          // 08
};
static_assert(sizeof(BSTSimpleList_pExtraDataList) == 0x10);

struct InventoryEntryData
{
	TESBoundObject*                      object;        // 00 (TESBoundObject*)
	BSTSimpleList_pExtraDataList*        extraLists;    // 08 (BSSimpleList<ExtraDataList*>*)
	int32_t					             countDelta;    // 10
	uint32_t                             pad14;         // 14
};
static_assert(sizeof(InventoryEntryData) == 0x18);

struct BSTSimpleList_pInventoryEntryData
{
	InventoryEntryData*                  item;          // 00
	BSTSimpleList_pInventoryEntryData*   next;          // 08
};
static_assert(sizeof(BSTSimpleList_pInventoryEntryData) == 0x10);

struct InventoryChanges
{
	BSTSimpleList_pInventoryEntryData*   entryList;     // 00 (BSSimpleList<InventoryEntryData*>*)
	TESObjectREFR*                       owner;         // 08
	float                                totalWeight;   // 10
	float                                armorWeight;   // 14
	bool                                 changed;       // 18
	uint8_t                              unk19;         // 19
	uint8_t                              unk1A;         // 1A
	uint8_t                              unk1B;         // 1B
	uint32_t                             unk1C;         // 1C
};
static_assert(sizeof(InventoryChanges) == 0x20);