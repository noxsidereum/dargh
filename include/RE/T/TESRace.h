// ============================================================================
//                               TESRace.h
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
//    https://github.com/Ryan-rsm-McKenzie/CommonLibSSE/blob/master/include/RE/T/TESRace.h
#pragma once
#include "RE/B/BGSBehaviorGraphModel.h"
#include "RE/B/BGSKeywordForm.h"
#include "RE/B/BGSVoiceType.h"
#include "RE/T/TESForm.h"
#include "RE/T/TESFullName.h"
#include "RE/T/TESModel.h"

struct TESRace
{
	// override
	TESForm                    form;                           // 000 (public TESForm)
	TESFullName                fullName;                       // 020 (public TESFullName)
	uint64_t                   description[2];                 // 030 (public TESDescription)
	uint64_t                   spellList[2];                   // 040 (public TESSpellList)
	uint64_t                   skin[2];                        // 050 (public BGSSkinForm)
	uint64_t                   biped[2];                       // 060 (public BGSBipedObjectForm)
	BGSKeywordForm             keyword;                        // 070 (public BGSKeywordForm)
	uint64_t                   attackData[2];                  // 088 (public BGSAttackDataForm)

	// add
	TESModel                   skeletonModels[2];              // 098 ANAM ([SEXES::kTotal])
	char                       data[0xA4];                     // 0E8 (RACE_DATA, sizeof(RACE_DATA) == 0xA4)
	float                      clampFaceGeoValue;              // 18C PNAM
	float                      clampFaceGeoValue2;             // 18C UNAM
	uint32_t                   pad194;                         // 194
	char                       bodyTextureModels[0x50];        // 198 (BGSTextureModel [SEXES::kTotal])
	BGSBehaviorGraphModel      behaviorGraphs[2];              // 1E8 ([SEXES::kTotal])
	char*                      rootBehaviorGraphNames[2];      // 238 (BSFixedString [SEXES::kTotal])
	char*                      behaviorGraphProjectNames[2];   // 248 (BSFixedString [SEXES::kTotal])
	BGSVoiceType*              defaultVoiceTypes[2];           // 258 VTCK (BGSVoiceType* [SEXES::kTotal])
	uint64_t                   bodyPartData;                   // 268 GNAM (BGSBodyPartData*)
	uint64_t                   decapitateArmors[2];            // 270 DNAM (TESObjectARMO* [SEXES::kTotal])
	uint64_t                   unk280[3];                      // 280 
	uint64_t                   unk298[3];                      // 298
	uint64_t                   unk2B0;                         // 2B0
	uint64_t                   unk2B8;                         // 2B8
	uint64_t                   unk2C0;                         // 2C0
	uint64_t                   unk2C8;                         // 2C8
	uint64_t                   attackAnimationArrayMap[2];     // 2D0 (AttackAnimationArrayMap* [SEXES::kTotal])
	char*                      formEditorID;                   // 2E0 EDID (BSFixedString)
	uint64_t                   bloodImpactMaterial;            // 2E8 NAM4 (BGSMaterialType*)
	uint64_t                   impactDataSet;                  // 2F0 NAM5 (BGSImpactDataSet*)
	uint64_t                   dismemberBlood;                 // 2F8 NAM7 (BGSArtObject*)
	uint64_t                   corpseOpenSound;                // 300 ONAM (BGSSoundDescriptorForm*)
	uint64_t                   corpseCloseSound;               // 308 LNAM (BGSSoundDescriptorForm*)
	char*                      bipedObjectNameA[32];           // 310 NAME (BSFixedString [BIPED_OBJECTS::kEditorTotal])
	uint64_t                   equipSlots[3];                  // 410 QNAM (BSTArray<BGSEquipSlot*>)
	uint32_t                   validEquipTypes;                // 428 VNAM
	uint32_t                   unk42C;                         // 42C
	uint64_t                   unarmedEquipSlot;               // 430 UNES (BGSEquipSlot*)
	TESRace*                   morphRace;                      // 438 NAM8
	TESRace*                   armorParentRace;                // 440 RNAM
	uint64_t                   unk448[3];                      // 448 
	uint64_t                   phonemeTargets[3];              // 460 PHTN (BSTArray<BSFixedString>)
	uint64_t                   baseMoveTypes[6];               // 478 WKMV / RNMV / WMMV / FLMV / SNMV / SPMV (BGSMovementType* [MovementTypes::kTotal])
	uint64_t                   faceRelatedData[2];             // 4A8 (FaceRelatedData* [SEXES::kTotal])
};
static_assert(sizeof(TESRace) == 0x4B8);