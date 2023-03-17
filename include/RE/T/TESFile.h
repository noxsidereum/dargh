// ============================================================================
//                               TESFile.h
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
//   https://github.com/Ryan-rsm-McKenzie/CommonLibSSE/blob/master/include/RE/T/TESFile.h
#pragma once
#include "RE/B/BSString.h"
#include "RE/T/TESObjectCell.h"

struct TESFile
{
	uint32_t           lastError;                        // 000 (stl::enumeration<Error, uint32_t>)
	uint32_t           pad004;                           // 004
	TESFile*           threadSafeParent;                 // 008
	void*              threadSafeFileMap;                // 010 (NiTPointerMap<uint32_t, TESFile*>*)
	uint64_t           unk018;                           // 018
	uint64_t           unk020;                           // 020
	uint8_t            unk028;                           // 028
	uint8_t            unk029;                           // 029
	uint16_t           pad02A;                           // 02A
	uint32_t           pad02C;                           // 02C
	uint64_t           lockedFile;                       // 030 (BSFile*)
	uint64_t           file;                             // 038 (BSFile*)
	uint64_t           formUserDataBitArray;             // 040 (TESBitArrayFile*)
	uint64_t           formVersionBitArray;              // 048 (TESBitArrayFile*)
	uint64_t           formIDBitArray;                   // 050 (TESBitArrayFile*)
	char               fileName[0x104];                  // 058 [WinAPI::MAX_PATH]
	char               path[0x104];                      // 15C [WinAPI::MAX_PATH]
	char*              buffer;                           // 260
	uint32_t           bufferAllocSize;                  // 268
	uint32_t           firstCellOffset;                  // 26C
	uint32_t           currCellOffset;                   // 270
	uint32_t           unk274;                           // 274
	TESObjectCELL*     currCell;                         // 278
	uint32_t           currRefOffset;                    // 280
	char               currentform[24];                  // 284 (FORM)
	uint32_t           currentchunkID;                   // 29C
	uint32_t           actualChunkSize;                  // 2A0
	uint32_t           filesize;                         // 2A4
	uint32_t           fileOffset;                       // 2A8
	uint32_t           formoffset;                       // 2AC
	uint32_t           chunkoffset;                      // 2B0
	char               saveform[24];                     // 2B4 (FORM)
	uint32_t           saveFormOffset;                   // 2CC
	uint64_t           saveChunkOffset;                  // 2D0
	uint64_t           unk2D8;                           // 2D8
	uint64_t           unk2E0;                           // 2E0
	uint8_t            unk2E8;                           // 2E8
	bool               isBigEndian;                      // 2E9
	uint8_t            unk2EA;                           // 2EA
	uint8_t            pad2EB;                           // 2EB
	char               fileData[0x140];                  // 2EC (WinAPI::WIN32_FIND_DATAA)
	float              unk42C;                           // 42C
	uint32_t           unk430;                           // 430
	uint32_t           flags;                            // 434
	uint32_t           recordFlags;                      // 438 (stl::enumeration<RecordFlag, uint32_t>)
	uint32_t           pad43C;                           // 43C
	uint64_t           masters[2];                       // 440 (BSSimpleList<const char*>)
	uint64_t           mastersData[2];                   // 450 (BSSimpleList<uint64_t*>)
	uint32_t           masterCount;                      // 460
	uint32_t           pad464;                           // 464
	TESFile**          masterPtrs;                       // 468
	uint64_t           deletedFormTime;                  // 470 (WinAPI::FILETIME)
	uint8_t            compileIndex;                     // 478 (== SKSE's index)
	uint8_t            pad479;                           // 479
	uint16_t           smallFileCompileIndex;            // 47A (== SKSE's lightIndex)
	uint32_t           pad47C;                           // 47C
	BSString           createdBy;                        // 480
	BSString           summary;                          // 490
	char*              decompressedFormBuffer;           // 4A0
	uint32_t           decompressedFormBufferSize;       // 4A8
	uint32_t           pad4AC;                           // 4AC
	void*              reservedDecompressionBuffer;      // 4B0
	uint32_t           reservedDecompressionBufferSize;  // 4B8
	uint32_t           pad4BC;                           // 4BC
	void*              unk4C0;                           // 4C0
};
static_assert(sizeof(TESFile) == 0x4C8);