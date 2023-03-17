// ============================================================================
//                              hkbCharacter.h
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
//    https://github.com/Ryan-rsm-McKenzie/CommonLibSSE/blob/master/include/RE/H/hkbCharacter.h
// 
// For further info on Havok and the Havok Animation Studio, see
//   http://web.archive.org/web/http://anarchy.cn/manual/12/HavokSdk_ProgrammersManual/index.html
//   https://github.com/Bewolf2/projectanarchy
#pragma once
#include "RE/H/hkbAnimationBindingSet.h"
#include "RE/H/hkbCharacterSetup.h"
#include "RE/H/hkbCharacterStringData.h"
#include "RE/H/hkbProjectData.h"
#include "RE/H/hkReferencedObject.h"
#include "RE/H/hkTypes.h"

struct hkbCharacter
{
	// override:
	hkReferencedObject        refObj;                          // 00

	// add:
	hkArray                   nearbyCharacters;                // 10 (hkArray<hkbCharacter*>)
	hkInt16                   currentLod;                      // 20
	hkInt16                   numTracksInLod;                  // 22
	uint32_t	              userData;                        // 24
	hkStringPtr               name;                            // 28
	uint64_t                  ragdollDriver;                   // 30 (hkbRagdollDriver*)
	uint64_t                  characterControllerDriver;       // 38 (hkbCharacterControllerDriver*)
	uint64_t                  footIkDriver;                    // 40 (hkbFootIkDriver*)
	uint64_t                  handIkDriver;                    // 48 (hkbHandIkDriver*)
	hkbCharacterSetup*        setup;                           // 50 (hkbCharacterSetup*)
	uint64_t                  behaviorGraph;                   // 58 (hkbBehaviorGraph*)
	hkbProjectData*           projectData;                     // 60
    hkbAnimationBindingSet*	  animationBindingSet;             // 68
	uint64_t                  raycastInterface;                // 70
	uint64_t                  world;                           // 78 (hkbWorld*)
	uint64_t                  eventQueue;                      // 80 (hkbEventQueue*)
	uint64_t                  worldFromModel;                  // 88
	uint64_t                  poseLocal;                       // 90
	uint32_t                  numPoseLocal;                    // 98
	bool                      deleteWorldFromModel;            // 9C
	bool                      deletePoseLocal;                 // 9D
	uint16_t                  pad9E;                           // 9E
};
static_assert(sizeof(hkbCharacter) == 0xA0);