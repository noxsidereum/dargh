// ============================================================================
//                          hkbCharacterSetup.h
// ----------------------------------------------------------------------------
// Part of the open-source Dynamic Animation Replacer (DARGH).
// 
// Copyright (c) 2023 Nox Sidereum
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
// For further info on Havok and the Havok Animation Studio, see
//   http://web.archive.org/web/http://anarchy.cn/manual/12/HavokSdk_ProgrammersManual/index.html
//   https://github.com/Bewolf2/projectanarchy
#pragma once
#include "RE/H/hkbCharacterData.h"
#include "RE/H/hkReferencedObject.h"
#include "RE/H/hkTypes.h"

struct hkbCharacterSetup
{
	// override:
	hkReferencedObject     refObj;                             // 00

	// add:
	hkArray                retargetingSkeletonMappers;         // 10 (hkArray<hkRefPtr<const hkaSkeletonMapper>>)
	uint64_t               animationSkeleton;                  // 20 (hkaSkeleton*)
	uint64_t               ragdollToAnimationSkeletonMapper;   // 28 (hkaSkeletonMapper*)
	uint64_t               animationToRagdollSkeletonMapper;   // 30 (hkaSkeletonMapper*)
	uint64_t               animationBindingSet;                // 38 (hkbAnimationBindingSet*)
	hkbCharacterData*      data;                               // 40
	uint64_t               unscaledAnimationSkeleton;          // 48 (hkaSkeleton*)
	uint64_t               mirroredSkeleton;                   // 50 (hkaMirroredSkeleton*)
	uint64_t               characterPropertyIdMap;             // 58 (hkbSymbolIdMap*)
	uint64_t               criticalSection;                    // 60 (hkCriticalSection*)
};
static_assert(sizeof(hkbCharacterSetup) == 0x68);