// ============================================================================
//                              ActorState.h
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
//   https://github.com/Ryan-rsm-McKenzie/CommonLibSSE/blob/master/include/RE/A/ActorState.h
#pragma once

enum ACTOR_LIFE_STATE : uint32_t
{
	kAlive = 0,
	kDying = 1,
	kDead = 2,
	kUnconcious = 3,
	kReanimate = 4,
	kRecycle = 5,
	kRestrained = 6,
	kEssentialDown = 7,
	kBleedout = 8
};

enum class ATTACK_STATE_ENUM : uint32_t
{
	kNone = 0,
	kDraw = 1,
	kSwing = 2,
	kHit = 3,
	kNextAttack = 4,
	kFollowThrough = 5,
	kBash = 6,
	kBowDraw = 8,
	kBowAttached = 9,
	kBowDrawn = 10,
	kBowReleasing = 11,
	kBowReleased = 12,
	kBowNextAttack = 13,
	kBowFollowThrough = 14,
	kFire = 15,
	kFiring = 16,
	kFired = 17
};

enum class FLY_STATE : uint32_t
{
	kNone = 0,
	kTakeOff = 1,
	kCruising = 2,
	kHovering = 3,
	kLanding = 4,
	kPerching = 5,
	kAction = 6
};

enum class KNOCK_STATE_ENUM : uint32_t
{
	kNormal = 0,
	kExplode = 1,
	kExplodeLeadIn = 2,
	kOut = 3,
	kOutLeadIn = 4,
	kQueued = 5,
	kGetUp = 6,
	kDown = 7,
	kWaitForTaskQueue = 8
};

enum class SIT_SLEEP_STATE : uint32_t
{
	kNormal = 0,
	kWantToSit = 1,
	kWaitingForSitAnim = 2,

	kIsSitting = 3,
	kRidingMount = static_cast<std::underlying_type_t<SIT_SLEEP_STATE>>(kIsSitting),

	kWantToStand = 4,

	kWantToSleep = 5,
	kWaitingForSleepAnim = 6,
	kIsSleeping = 7,
	kWantToWake = 8
};

enum class WEAPON_STATE : uint32_t
{
	kSheathed = 0,
	kWantToDraw = 1,
	kDrawing = 2,
	kDrawn = 3,
	kWantToSheathe = 4,
	kSheathing = 5
};

struct ActorState1
{
	uint32_t            movingBack : 1;                      // 0:00
	uint32_t            movingForward : 1;                   // 0:01
	uint32_t            movingRight : 1;                     // 0:02
	uint32_t            movingLeft : 1;                      // 0:03
	uint32_t            unk04 : 2;                           // 0:04
	uint32_t            walking : 1;                         // 0:06
	uint32_t            running : 1;                         // 0:07
	uint32_t            sprinting : 1;                       // 0:08
	uint32_t            sneaking : 1;                        // 0:09
	uint32_t            swimming : 1;                        // 0:10
	uint32_t            unk11 : 3;                           // 0:11
	SIT_SLEEP_STATE     sitSleepState : 4;                   // 0:14
	FLY_STATE           flyState : 3;                        // 0:18
	ACTOR_LIFE_STATE    lifeState : 4;                       // 0:21
	KNOCK_STATE_ENUM    knockState : 3;                      // 0:25
	ATTACK_STATE_ENUM   meleeAttackState : 4;                // 0:28
};
static_assert(sizeof(ActorState1) == 0x4);

struct ActorState2
{
	uint32_t            talkingToPlayer : 1;                 // 0:00
	uint32_t            forceRun : 1;                        // 0:01
	uint32_t            forceSneak : 1;                      // 0:02
	uint32_t            headTracking : 1;                    // 0:03
	uint32_t            reanimating : 1;                     // 0:04
	WEAPON_STATE        weaponState : 3;                     // 0:05
	uint32_t            wantBlocking : 1;                    // 0:08
	uint32_t            flightBlocked : 1;                   // 0:09
	uint32_t            recoil : 2;                          // 0:10
	uint32_t            allowFlying : 1;                     // 0:12
	uint32_t            staggered : 1;                       // 0:13
	uint32_t            unk14 : 18;                          // 0:14
};
static_assert(sizeof(ActorState2) == 0x4);

struct ActorState
{
	// Override
	uint64_t            pVFT;                                // 00 (public IMovementState)

	// Add
	ActorState1         actorState1;                         // 08
	ActorState2         actorState2;                         // 0C
};
static_assert(sizeof(ActorState) == 0x10);