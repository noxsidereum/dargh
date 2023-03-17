// ============================================================================
//                            TESDataHandler.h
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
//   https://github.com/Ryan-rsm-McKenzie/CommonLibSSE/blob/master/include/RE/T/TESDataHandler.h
#pragma once
#include "RE/B/BSTList.h"
#include "RE/F/FormTypes.h"
#include "RE/T/TESFile.h"

struct FormArray
{
	TESForm**                    entries;	                    // 00
	uint32_t                     capacity;	                    // 08
	uint32_t                     pad0C;                         // 0C
	uint32_t                     count;                         // 10
	uint32_t                     pad14;                         // 14
};
static_assert(sizeof(FormArray) == 0x18);

struct TESDataHandler
{
	// override:
	uint8_t                      singletonSDM;                  // 000 public BSTSingletonSDM<TESDataHandler>

	// add:
	uint8_t                      pad001;                        // 001
	uint16_t                     pad002;                        // 002
	uint32_t                     pad004;                        // 004
	uint64_t                     objectList;                    // 008 (TESObjectList*)
	FormArray                    formArrays[FormType::Max];     // 010 (BSTArray<TESForm*>)
	uint64_t                     regionList;                    // D00 (TESRegionList*)
	uint64_t                     interiorCells[3];              // D08 (NiTPrimitiveArray<TESObjectCELL*>)
	uint64_t                     addonNodes[3];                 // D20 (NiTPrimitiveArray<BGSAddonNode*>)
	uint64_t                     badForms[3];                   // D38 (NiTList<TESForm*>)
	uint32_t                     nextID;                        // D50 (FormID)
	uint32_t                     padD54;                        // D54
	TESFile*                     activeFile;                    // D58
	BSSimpleList<TESFile*>       files;                         // D60
	uint64_t                     compiledFileCollection[6];     // D70
	bool                         masterSave;                    // DA0
	bool                         blockSave;                     // DA1
	bool                         saveLoadGame;                  // DA2
	bool                         autoSaving;                    // DA3
	bool                         exportingPlugin;               // DA4
	bool                         clearingData;                  // DA5
	bool                         hasDesiredFiles;               // DA6
	bool                         checkingModels;                // DA7
	bool                         loadingFiles;                  // DA8
	bool                         dontRemoveIDs;                 // DA9
	uint8_t                      unkDAA;                        // DAA
	uint8_t                      padDAB;                        // DAB
	uint32_t                     padDAC;                        // DAC
	uint64_t                     regionDataManager;             // DB0 (TESRegionDataManager*)
	uint64_t                     unkDB8;                        // DB8
};
static_assert(sizeof(TESDataHandler) == 0xDC0);

const TESFile* LookupModByName(TESDataHandler* dh, std::string_view a_modName);