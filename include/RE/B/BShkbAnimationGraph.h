// ============================================================================
//                          BShkbAnimationGraph.h
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
//   https://github.com/Ryan-rsm-McKenzie/CommonLibSSE/blob/master/include/RE/B/BShkbAnimationGraph.h
#pragma once
#include "RE/A/Actor.h"
#include "RE/H/hkbCharacter.h"

struct BShkbAnimationGraph
{
	// override:
	uint64_t           bsiRagdollDriver;                     // 000 (public BSIRagdollDriver)
	uint64_t           bsIntrusiveRefCounted;                // 008 (public BSIntrusiveRefCounted)
	char               bstEventSourceTransDelta[0x58];       // 010 (public BSTEventSource<BSTransformDeltaEvent>)
	char               bstEventSourceAnimGraph[0x58];        // 068 (public BSTEventSource<BSAnimationGraphEvent>)

	// add:
	hkbCharacter       characterInstance;                    // 0C0
	char               unk160[0x88];                         // 160
	float              interpolationTimeOffsets[2];          // 1E8
	char*              projectName;                          // 1F0 (BSFixedString)
	uint64_t           unk1F8;                               // 1F8
	uint64_t           projDbData;                           // 200 (BshkbHkxDB::ProjectDBData*)
	uint64_t           behaviourGraph;                       // 208 (hkbBehaviorGraph*)
	Actor*             holder;                               // 210
	uint64_t           unk218;                               // 218 (BSFadeNode*)
	uint64_t           unk220;                               // 220
	uint64_t           unk228;                               // 228
	uint64_t           unk230;                               // 230
	uint64_t           physicsWorld;                         // 238 (bhkWorld*)
    uint32_t           unk240;                               // 240
	uint16_t           unk244;                               // 244
	uint8_t            unk246;                               // 246
	uint8_t            unk247;                               // 247
	uint16_t           unk248;                               // 248
	uint16_t           unk24A;                               // 24A
	uint32_t           unk24C;                               // 24C
};
static_assert(sizeof(BShkbAnimationGraph) == 0x250);