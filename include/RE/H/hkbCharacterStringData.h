// ============================================================================
//                       hkbCharacterStringData.h
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
#include "RE/H/hkReferencedObject.h"
#include "RE/H/hkTypes.h"

struct hkbCharacterStringData
{
	// override:
	hkReferencedObject     refObj;                              // 00

	// add:
	hkArray                deformableSkinNames;                 // 10 (hkArray<FileNameMeshNamePair>)
	hkArray                rigidSkinNames;                      // 20 (hkArray<FileNameMeshNamePair>)
	hkArray                animationNames;                      // 30 (hkArray<<hkbAssetBundleStringData>)
	hkArray                animationBundleFilenameData;         // 40 (hkArray<hkbAssetBundleStringData>) - unused by Skyrim?
	hkArray                characterPropertyNames;              // 50 (hkArray<hkStringPtr>)
	hkArray                retargetingSkeletonMapperFilenames;  // 60 (hkArray<hkStringPtr>)
	hkArray                lodNames;                            // 70 (hkArray<hkStringPtr>)
	hkArray                mirroredSyncPointSubstringsA;        // 80 (hkArray<hkStringPtr>)
	hkArray                mirroredSyncPointSubstringsB;        // 90 (hkArray<hkStringPtr>)
	hkStringPtr            name;                                // A0
	hkStringPtr            rigName;                             // A8
	hkStringPtr            ragdollName;                         // B0
	hkStringPtr            behaviorFilename;                    // B8
};
static_assert(sizeof(hkbCharacterStringData) == 0xC0);