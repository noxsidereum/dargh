// ============================================================================
//                           SettingCollection.h
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
//    https://github.com/Ryan-rsm-McKenzie/CommonLibSSE/blob/master/include/RE/S/SettingCollection.h
#pragma once

struct SettingCollection
{
	uint64_t       pVFT;    // 000
	const char*    subKey;  // 008
	uint64_t       unk010;  // 010
	uint64_t       unk018;  // 018
	uint64_t       unk020;  // 020
	uint64_t       unk028;  // 028
	uint64_t       unk030;  // 030
	uint64_t       unk038;  // 038
	uint64_t       unk040;  // 040
	uint64_t       unk048;  // 048
	uint64_t       unk050;  // 050
	uint64_t       unk058;  // 058
	uint64_t       unk060;  // 060
	uint64_t       unk068;  // 068
	uint64_t       unk070;  // 070
	uint64_t       unk078;  // 078
	uint64_t       unk080;  // 080
	uint64_t       unk088;  // 088
	uint64_t       unk090;  // 090
	uint64_t       unk098;  // 098
	uint64_t       unk0A0;  // 0A0
	uint64_t       unk0A8;  // 0A8
	uint64_t       unk0B0;  // 0B0
	uint64_t       unk0B8;  // 0B8
	uint64_t       unk0C0;  // 0C0
	uint64_t       unk0C8;  // 0C8
	uint64_t       unk0D0;  // 0D0
	uint64_t       unk0D8;  // 0D8
	uint64_t       unk0E0;  // 0E0
	uint64_t       unk0E8;  // 0E8
	uint64_t       unk0F0;  // 0F0
	uint64_t       unk0F8;  // 0F8
	uint64_t       unk100;  // 100
	uint64_t       unk108;  // 108
	void*          handle;  // 110
};
static_assert(sizeof(SettingCollection) == 0x118);