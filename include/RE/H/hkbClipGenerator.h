// ============================================================================
//                          hkbClipGenerator.h
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
#include "RE/H/hkbBindable.h"
#include "RE/H/hkbNode.h"
#include "RE/H/hkReferencedObject.h"
#include "RE/H/hkTypes.h"

enum PlaybackMode
{
	MODE_SINGLE_PLAY = 0,
	MODE_LOOPING,
	MODE_USER_CONTROLLED,
	MODE_PING_PONG,

	MODE_COUNT
};

struct hkbClipGenerator
{
	// override:
	hkReferencedObject     refObj;                              // 000
	hkbBindable            bindable;                            // 010
	hkbNode                node;                                // 030

	// add:
	hkStringPtr            animationName;                       // 048
	uint64_t               triggers;                            // 050 (hkRefPtr<hkbClipTriggerArray>)
	float                  cropStartAmountLocalTime;            // 058
	float                  cropEndAmountLocalTime;              // 05C
	float                  startTime;                           // 060
	float                  playbackSpeed;                       // 064
	float                  enforcedDuration;                    // 068
	float                  userControlledTimeFraction;          // 06C
	hkInt16                animationBindingIndex;               // 070
	hkInt8                 mode;                                // 072 (enum PlaybackMode).
	hkInt8                 flags;                               // 073
	uint32_t               pad74;                               // 074
	hkArray                animDatas;                           // 078 (hkArray<struct hkaAnimationSampleOnlyJob::AnimationData>)
	uint64_t               animationControl;                    // 088 (hkRefPtr<hkaDefaultAnimationControl>)
	uint64_t               originalTriggers;                    // 090 (hkRefPtr<hkbClipTriggerArray>)
	uint64_t               mapperData;                          // 098 (hkaDefaultAnimationControlMapperData*)
	uint64_t               binding;                             // 0A0 (hkaAnimationBinding*)
	int32_t                mirroredAnimation;                   // 0A8 (hkInt32)
	char                   extractedMotion[48];                 // 0AC (hkQsTransform => hkVector, hkQuaternion, hkVector)
	hkArray                echos;                               // 0E0 (hkArray<struct hkbClipGenerator::Echo>)
	double                 localTime;                           // 0F0 (hkReal)
	double                 time;                                // 0F8 (hkReal)
	double                 previousUserControlledTimeFraction;  // 100 (hkReal)
	int32_t                bufferSize;                          // 108 (hkInt32)
	char                   echoBufferSize;                      // 10C (hkBool)
	char                   atEnd;                               // 10D (hkBool)
	char                   ignoreStartTime;                     // 10E (hkBool)
	char                   pingPongBackward;                    // 10F (hkBool)
};
static_assert(sizeof(hkbClipGenerator) == 0x110);