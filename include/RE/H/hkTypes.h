// ============================================================================
//                            hkTypes.h
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

struct hkFinishLoadedObjectFlag
{
	int m_finishing;
};

typedef float           hkFloat32;
typedef double          hkDouble64;
typedef signed char		hkChar;
typedef signed char		hkInt8;
typedef signed short	hkInt16;
typedef signed int		hkInt32;
typedef unsigned char	hkUchar;
typedef unsigned char	hkUint8;
typedef unsigned short	hkUint16;
typedef unsigned int	hkUint32;
typedef char**          hkStringPtr;          

struct hkArray
{
	uint64_t           _data;                      // 00
	uint32_t           _size;                      // 08
	uint32_t           _capacityAndFlags;          // 0C
};
static_assert(sizeof(hkArray) == 0x10);

struct FileNameMeshNamePair
{
	hkStringPtr m_fileName;
	hkStringPtr m_meshName;
};

struct hkbAssetBundleStringData
{
	hkStringPtr        m_bundleName;
	hkArray            m_assetNames;     // hkArray<hkStringPtr> (Skyrim doesn't use these)
};