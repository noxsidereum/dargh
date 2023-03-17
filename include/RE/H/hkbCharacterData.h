// ============================================================================
//                          hkbCharacterData.h
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
#include "RE/H/hkbCharacterStringData.h"
#include "RE/H/hkReferencedObject.h"
#include "RE/H/hkTypes.h"

struct hkbCharacterData
{
	// override:
	hkReferencedObject        refObj;                           // 00

	// add:
	char                      m_characterControllerSetup[24];   // 10 (hkbCharacterControllerSetup)  
	uint32_t                  m_modelUpMS[4];                   // 28 (hkVector4)
	uint32_t                  m_modelForwardMS[4];              // 38 (hkVector4)
	uint32_t                  m_modelRightMS[4];                // 48 (hkVector4)
	hkArray                   m_characterPropertyInfos;         // 58 (hkArray<hkbVariableInfo>)
	hkArray                   m_numBonesPerLod;                 // 68 (hkArray<hkInt32>)
	uint64_t                  m_characterPropertyValues;        // 78 (hkbVariableValueSet*)
	uint64_t                  m_footIkDriverInfo;               // 80 (hkbFootIkDriverInfo*)
	uint64_t                  m_handIkDriverInfo;               // 88 (hkbHandIkDriverInfo*)
	hkReferencedObject*       m_aiControlDriverInfo;            // 90
	hkbCharacterStringData*   m_stringData;                     // 98
};
static_assert(sizeof(hkbCharacterData) == 0xA0);