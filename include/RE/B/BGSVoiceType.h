// ============================================================================
//                              BGSVoiceType.h
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
//   https://github.com/Ryan-rsm-McKenzie/CommonLibSSE/blob/master/include/RE/B/BGSVoiceType.h
#pragma once
#include "RE/B/BSString.h"
#include "RE/T/TESForm.h"

struct VOICE_TYPE_DATA    // DNAM
{
	enum class VoiceTypeFlag : uint8_t
	{
		kNone = 0,
		kAllowDefaultDialogue = 1 << 0,
		kFemale = 1 << 1
	};

	VoiceTypeFlag flags;  // 0
};
static_assert(sizeof(VOICE_TYPE_DATA) == 0x1);

struct BGSVoiceType
{
	// override:
	TESForm            form;                                 // 00 (public TESForm)

	// add:
	VOICE_TYPE_DATA    data;                                 // 20 - DNAM
	uint8_t            pad21;                                // 21
	uint16_t           pad22;                                // 22
	uint16_t           pad24;                                // 24
	BSString           formEditorID;                         // 28 - EDID
};
static_assert(sizeof(BGSVoiceType) == 0x38);