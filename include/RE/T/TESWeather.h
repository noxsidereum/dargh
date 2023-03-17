// ============================================================================
//                           TESWeather.h
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
//    https://github.com/Ryan-rsm-McKenzie/CommonLibSSE/blob/master/include/RE/T/TESWeather.h
#pragma once
#include "RE/T/TESForm.h"
#include "RE/T/TESModel.h"

struct WeatherData  // DATA
{
	struct Color3
	{
		int8_t  red;    // 0
		int8_t  green;  // 1
		int8_t  blue;   // 2
	};
	static_assert(sizeof(Color3) == 0x3);

	int8_t         windSpeed;                                     // 00
	int8_t         unk01;                                         // 01
	int8_t         unk02;                                         // 02
	int8_t         transDelta;                                    // 03
	int8_t         sunGlare;                                      // 04
	int8_t         sunDamage;                                     // 05
	int8_t         precipitationBeginFadeIn;                      // 06
	int8_t         precipitationEndFadeOut;                       // 07
	int8_t         thunderLightningBeginFadeIn;                   // 08
	int8_t         thunderLightningEndFadeOut;                    // 09
	int8_t         thunderLightningFrequency;                     // 0A
	uint8_t        flags;                                         // 0B (stl::enumeration<WeatherDataFlag, std::uint8_t>)
	Color3         lightningColor;                                // 0C
	int8_t         visualEffectBegin;                             // 0F
	int8_t         visualEffectEnd;                               // 10
	int8_t         windDirection;                                 // 11
	int8_t         windDirectionRange;                            // 12
	int8_t         unk13;                                         // 13
};
static_assert(sizeof(WeatherData) == 0x14);

struct FogData  // FNAM
{
	float          dayNear;                                       // 00
	float          dayFar;                                        // 04
	float          nightNear;                                     // 08
	float          nightFar;                                      // 0C
	float          dayPower;                                      // 10
	float          nightPower;                                    // 14
	float          dayMax;                                        // 18
	float          nightMax;                                      // 1C
};
static_assert(sizeof(FogData) == 0x20);

struct TESWeather
{
	// override:
	TESForm        form;                                          // 000 (public TESForm)

	// add:
	char           cloudTextures[0x200];                          // 020 00TX - L0TX (TESTexture1024 [kTotalLayers])
	int8_t         cloudLayerSpeedY[32];                          // 220 RNAM ([kTotalLayers])
	int8_t         cloudLayerSpeedX[32];                          // 240 QNAM ([kTotalLayers])
	uint32_t       cloudColorData[32][4];                         // 260 PNAM (Color)[kTotalLayers][ColorTime::kTotal]
	float          cloudAlpha[32][4];                             // 460 JNAM
	uint32_t       cloudLayerDisabledBits;                        // 660 NAM1 - bitfield
	WeatherData    data;                                          // 664 DATA
	FogData        fogData;                                       // 678 FNAM
	uint32_t       colorData[17][4];                              // 698 NAM0 (Color)[kTotalLayers][ColorTime::kTotal]
	uint64_t       sounds[2];                                     // 7A8 (WeatherSoundList)
	uint64_t       skyStatics[3];                                 // 7B8 (BSTArray<TESObjectSTAT*>)
	uint32_t       numCloudLayers;                                // 7D0 LNAM
	uint32_t       pad7D4;                                        // 7D4
	uint64_t       imageSpaces[4];                                // 7D8 IMSP (TESImageSpace* [ColorTime::kTotal])
	char           directionalAmbientLightingColors[0x20][4];     // 7F8 (BGSDirectionalAmbientLightingColors [ColorTime::kTotal])
	TESModel       aurora;                                        // 878
	uint64_t       sunGlareLensFlare;                             // 8A0 (BGSLensFlare*)
	uint64_t       volumetricLighting[4];                         // 8A8 - HNAM (BGSVolumetricLighting* [ColorTime::kTotal])
	uint64_t       precipitationData;                             // 8C8 - MNAM (BGSShaderParticleGeometryData*)
	uint64_t       referenceEffect;                               // 8D0 - NNAM (BGSReferenceEffect*)
};
static_assert(sizeof(TESWeather) == 0x8D8);