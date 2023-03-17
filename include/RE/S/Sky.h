// ============================================================================
//                                 Sky.h
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
//    https://github.com/Ryan-rsm-McKenzie/CommonLibSSE/blob/master/include/RE/S/Sky.h
#pragma once
#include "RE/T/TESWeather.h"

struct Sky
{
	uint64_t           pVFT;                              // 000
	uint64_t           root;                              // 008 (NiPointer<BSMultiBoundNode>)
	uint64_t           moonsRoot;                         // 010 (NiPointer<NiNode>)
	uint64_t           auroraRoot;                        // 018 (NiPointer<NiNode>)
	uint64_t           unk020;                            // 020
	uint64_t           extLightingOverride;               // 028 (BGSLightingTemplate*)
	uint32_t           currentRoom;                       // 030 (ObjectRefHandle)
	uint32_t           previousRoom;                      // 034 (ObjectRefHandle)
	float              lightingTransition;                // 038
	float              lightingTransitionTimer;           // 03C
	uint64_t           currentClimate;                    // 040 (TESClimate*)
	TESWeather*        currentWeather;                    // 048
	TESWeather*        lastWeather;                       // 050
	TESWeather*        defaultWeather;                    // 058
	TESWeather*        overrideWeather;                   // 060
	uint64_t           region;                            // 068 (TESRegion*)
	uint64_t           atmosphere;                        // 070 (Atmosphere*)
	uint64_t           stars;                             // 078 (Stars*)
	uint64_t           sun;                               // 080 (Sun*)
	uint64_t           clouds;                            // 088 (Clouds*)
	uint64_t           masser;                            // 090 (Moon*)
	uint64_t           secunda;                           // 098 (Moon*)
	uint64_t           precip;                            // 0A0 (Precipitation*)
	char               skyColor[204];                     // 0A8 (NiColor [17])
	uint32_t           unk174;                            // 174
	uint64_t           unk178;                            // 178
	uint64_t           unk180;                            // 180
	uint64_t           unk188;                            // 188
	uint64_t           unk190;                            // 190
	uint64_t           unk198;                            // 198
	uint32_t           unk1A0;                            // 1A0
	uint32_t           unk1A4;                            // 1A4
	float              unk1A8;                            // 1A8
	float              unk1AC;                            // 1AC
	float              currentGameHour;                   // 1B0
	float              lastWeatherUpdate;                 // 1B4
	float              currentWeatherPct;                 // 1B8
	uint32_t           mode;                              // 1BC (stl::enumeration<Mode, uint32_t>)
	uint64_t           skySoundList;                      // 1C0 (BSSimpleList<SkySound*>*)
	float              flash;                             // 1C8
	uint32_t           pad1CC;                            // 1CC
	uint64_t           flashTime;                         // 1D0 (std::time_t)
	uint32_t           lastMoonPhaseUpdate;               // 1D8
	uint32_t           flags;                             // 1DC
	uint64_t           currentWeatherImageSpaceMod;       // 1E0 (ImageSpaceModifierInstanceForm*)
	uint64_t           currentWeatherImageSpaceMod2;      // 1E8 (ImageSpaceModifierInstanceForm*)
	uint64_t           lastWeatherImageSpaceMod;          // 1F0 (ImageSpaceModifierInstanceForm*)
	uint64_t           lastWeatherImageSpaceMod2;         // 1F8 (ImageSpaceModifierInstanceForm*)
	uint64_t           directionalAmbientColors[9];       // 200  (NiColor[3][2])
	char               ambientSpecularTint[12];           // 248  (NiColor)
	float              ambientSpecularFresnel;            // 254
	float              auroraInStart;                     // 258
	float              auroraIn;                          // 25C
	float              auroraOutStart;                    // 260
	float              auroraOut;                         // 264
	uint64_t           currentReferenceEffect;            // 268 (NiPointer<ReferenceEffect>)
	uint64_t           lastReferenceEffect;               // 270 (NiPointer<ReferenceEffect>)
	uint64_t           effectController;                  // 278 (SkyEffectController)
	uint64_t           storedCloudTextures[3];            // 280 (BSTArray<NiPointer<NiTexture>>)
	uint64_t           storedWorldMapCloudTextures[3];    // 298 (BSTArray<NiPointer<NiTexture>>)
	uint64_t           skyStaticRefData[3];               // 2B0 (BSTArray<SkyStaticRefData>)
};
static_assert(sizeof(Sky) == 0x2C8);