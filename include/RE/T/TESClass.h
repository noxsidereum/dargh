// ============================================================================
//                             TESClass.h
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
//    https://github.com/Ryan-rsm-McKenzie/CommonLibSSE/blob/master/include/RE/T/TESClass.h
#pragma once
#include "RE/T/TESForm.h"
#include "RE/T/TESFullName.h"

struct SkillWeights
{
	uint8_t oneHanded;    // 00
	uint8_t twoHanded;    // 01
	uint8_t archery;      // 02
	uint8_t block;        // 03
	uint8_t smithing;     // 04
	uint8_t heavyArmor;   // 05
	uint8_t lightArmor;   // 06
	uint8_t pickpocket;   // 07
	uint8_t lockpicking;  // 08
	uint8_t sneak;        // 09
	uint8_t alchemy;      // 0A
	uint8_t speech;       // 0B
	uint8_t alteration;   // 0C
	uint8_t conjuration;  // 0D
	uint8_t destruction;  // 0E
	uint8_t illusion;     // 0F
	uint8_t restoration;  // 10
	uint8_t enchanting;   // 11
};
static_assert(sizeof(SkillWeights) == 0x12);

struct AttributeWeights
{
	uint8_t health;       // 0
	uint8_t magicka;      // 1
	uint8_t stamina;      // 2
};
static_assert(sizeof(AttributeWeights) == 0x3);

struct TESClass
{
	// override
	TESForm            form;                     // 00 (public TESForm)
	TESFullName        fullName;                 // 20 (public TESFullName)
	uint64_t           description[2];           // 30 (public TESDescription)
	uint64_t           texture[2];               // 40 (public TESTexture)

	// add
	uint32_t           unk00;                    // 50
	uint8_t            teaches;                  // 54 (stl::enumeration<Skill, uint8_t>)
	uint8_t            maximumTrainingLevel;     // 55
	SkillWeights       skillWeights;             // 56
	float              bleedoutDefault;          // 68
	uint32_t           voicePoints;              // 6C
	AttributeWeights   attributeWeights;         // 70
	uint8_t            pad73;                    // 73 
	uint32_t           pad74;                    // 74
};
static_assert(sizeof(TESClass) == 0x78);
