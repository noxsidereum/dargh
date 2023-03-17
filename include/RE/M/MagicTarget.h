// ============================================================================
//                              MagicTarget.h
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
//    https://github.com/Ryan-rsm-McKenzie/CommonLibSSE/blob/master/include/RE/M/MagicTarget.h
#pragma once
#include "RE/M/MagicItem.h"

struct SpellDispelData
{
	MagicItem*         spell;                 // 00
	uint32_t           caster;                // 08 (BSPointerHandle<Actor>)
	uint32_t           pad0C;                 // 0C
	uint64_t           activeEffect;          // 10 (BSTSmartPointer<ActiveEffect>)
	SpellDispelData*   next;                  // 18
};
static_assert(sizeof(SpellDispelData) == 0x20);

struct MagicTarget
{
	uint64_t           pVFT;                  // 00
	SpellDispelData*   postUpdateDispelList;  // 08
	uint8_t            flags;                 // 10
	uint8_t            pad11;                 // 11
	uint16_t           pad12;                 // 12
	uint32_t           pad14;                 // 14
};
static_assert(sizeof(MagicTarget) == 0x18);