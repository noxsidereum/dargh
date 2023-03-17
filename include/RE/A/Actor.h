// ============================================================================
//                              Actor.h
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
//   https://github.com/Ryan-rsm-McKenzie/CommonLibSSE/blob/master/include/RE/A/Actor.h
#pragma once
#include "RE/A/ActorState.h"
#include "RE/A/ActorValueOwner.h"
#include "RE/A/AIProcess.h"
#include "RE/B/BGSLocation.h"
#include "RE/B/BSString.h"
#include "RE/M/MagicItem.h"
#include "RE/M/MagicTarget.h"
#include "RE/T/TESFaction.h"
#include "RE/T/TESObjectREFR.h"
#include "RE/T/TESRace.h"

struct SlotTypes
{
	enum
	{
		kLeftHand = 0,
		kRightHand,
		kUnknown,
		kPowerOrShout,

		kTotal
	};
};

struct Actor
{
	// Override:
	TESObjectREFR      ref;                                  // 000 (public TESObjectREFR)
	MagicTarget        magicTarget;                          // 098 (public MagicTarget)
	ActorValueOwner    actorValueOwner;                      // 0B0 (public ActorValueOwner)
	ActorState         actorState;                           // 0B8 (public ActorState)
	uint64_t           transformDeltaEvent;                  // 0C8 (public BSTEventSink<BSTransformDeltaEvent>)
	uint64_t           characterMoveFinishEvent;             // 0D0 (public BSTEventSink<bhkCharacterMoveFinishEvent>)
	uint64_t           unk0D8;                               // 0D8 (public IPostAnimationChannelUpdateFunctor)

	// Add:
	uint32_t           boolBits;                             // 0E0
	float              updateTargetTimer;                    // 0E4 
	uint32_t           criticalStage;                        // 0E8 
	uint32_t           pad0EC;                               // 0EC 
	AIProcess*         currentProcess;                       // 0F0
	uint32_t           dialogueItemTarget;                   // 0F8
	uint32_t           currentCombatTarget;                  // 0FC
	uint32_t           myKiller;                             // 100
	float              checkMyDeadBodyTimer;                 // 104
	float              voiceTimer;                           // 108
	float              underWaterTimer;                      // 10C
	int32_t            thiefCrimeStamp;                      // 110
	int32_t            actionValue;                          // 114
	float              timerOnAction;                        // 118
	uint32_t           unk11C;                               // 11C
	float              editorLocCoord[3];                    // 120
	float              editorLocRot;                         // 12C
	TESForm*           editorLocForm;                        // 130
	BGSLocation*       editorLocation;                       // 138
	uint64_t           actorMover;                           // 140 (ActorMover*)
	uint64_t           movementController;                   // 148 (BSTSmartPointer<MovementControllerNPC>)
	uint64_t           unk150;                               // 150
	uint64_t           combatController;                     // 158 (CombatController*)
	TESFaction*        vendorFaction;                        // 160
	float              calculateVendorFactionTimer;          // 168 (AITimeStamp)
	uint32_t           unk16C;                               // 16C
	uint32_t           unk170;                               // 170
	uint32_t           unk174;                               // 174
	uint32_t           unk178;                               // 178
	uint32_t           intimidateBribeDayStamp;              // 17C
	uint64_t           unk180;                               // 180
	uint64_t           addedSpells[3];                       // 188 (BSTSmallArray<SpellItem*>)
	uint64_t           magicCasters[SlotTypes::kTotal];      // 1A0 (ActorMagicCaster* [SlotTypes::kTotal])
	MagicItem*         selectedSpells[SlotTypes::kTotal];    // 1C0
	TESForm*           selectedPower;                        // 1E0
	uint32_t           unk1E8;                               // 1E8
	uint32_t           pad1EC;                               // 1EC
	TESRace*           race;                                 // 1F0
	float              equippedWeight;                       // 1F8
	uint32_t           boolFlags;                            // 1FC
	BSString           avStorage;                            // 200 (ActorValueStorage)
	BSString           unk210;                               // 210
	uint64_t           exclusiveBranch;                      // 220 (BGSDialogueBranch*)
	uint32_t           healthModifiers[3];                   // 228 (Modifiers)
	uint32_t           magickaModifiers[3];                  // 234 (Modifiers)
	uint32_t           staminaModifiers[3];                  // 240 (Modifiers)
	uint32_t           voicePointsModifiers[3];              // 24C (Modifiers)
	float              lastUpdate;                           // 258
	uint32_t           lastSeenTime;                         // 25C
	uint64_t           biped;                                // 260 (BSTSmartPointer<BipedAnim>)
	float              armorRating;                          // 268
	float              armorBaseFactorSum;                   // 26C
	int8_t             soundCallBackSet;                     // 270
	uint8_t            unk271;                               // 271
	uint8_t            unk272;                               // 272
	uint8_t            unk273;                               // 273
	uint32_t           unk274;                               // 274
	uint64_t           unk278;                               // 278
	uint64_t           unk280;                               // 280
	uint64_t           unk288[5];                            // 288 (WinAPI:CRITICAL_SECTION) havok related
};
static_assert(sizeof(Actor) == 0x2B8);