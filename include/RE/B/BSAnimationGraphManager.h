// ============================================================================
//                       BSAnimationGraphManager.h
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
//   https://github.com/Ryan-rsm-McKenzie/CommonLibSSE/blob/master/include/RE/B/BSAnimationGraphManager.h
#pragma once
#include "RE/B/BShkbAnimationGraph.h"
#include "RE/B/BSSpinLock.h"

struct BSAnimationGraphManager
{
	// override:
	uint64_t                     BSTEventSink_VFT;           // 00 (public BSTEventSink < BSAnimationGraphEvent>)
	mutable volatile uint32_t    _refCount{ 0 };             // 08 (public BSIntrusiveRefCounted)

	// add:
	uint32_t                     pad0C;                      // 0C
	uint64_t                     boundChannels[3];           // 10 (BSTArray<BSTSmartPointer<BSAnimationGraphChannel>>)
	uint64_t                     bumpedChannels[3];          // 28 (BSTArray<BSTSmartPointer<BSAnimationGraphChannel>>)
	uint64_t                     graphs[3];                  // 40 (BSTSmallArray<BSTSmartPointer<BShkbAnimationGraph>>)
	uint64_t                     subManagers[3];             // 58 (BSTArray<BSAnimationGraphManagerPtr>)
	uint64_t                     variableCache[5];           // 70 (BSAnimationGraphVariableCache)
	BShkbAnimationGraph*         pBsHkbAnimGraph;            // 98
	BSSpinLock                   updateLock;                 // A0
	uint32_t                     activeGraph;                // A8
	uint32_t                     generateDepth;              // AC
};
static_assert(sizeof(BSAnimationGraphManager) == 0xB0);