// ============================================================================
//                              ActorValues.h
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
//   https://github.com/Ryan-rsm-McKenzie/CommonLibSSE/blob/master/include/RE/A/ActorValues.h
#pragma once

enum ActorValue
{
	kNone_ActorValue = -1,
	kAggresion = 0,
	kConfidence = 1,
	kEnergy = 2,
	kMorality = 3,
	kMood = 4,
	kAssistance = 5,
	kOneHanded = 6,
	kTwoHanded = 7,
	kArchery = 8,
	kBlock = 9,
	kSmithing = 10,
	kHeavyArmor = 11,
	kLightArmor = 12,
	kPickpocket = 13,
	kLockpicking = 14,
	kSneak = 15,
	kAlchemy = 16,
	kSpeech = 17,
	kAlteration = 18,
	kConjuration = 19,
	kDestruction = 20,
	kIllusion = 21,
	kRestoration = 22,
	kEnchanting = 23,
	kHealth = 24,
	kMagicka = 25,
	kStamina = 26,
	kHealRate = 27,
	kMagickaRate = 28,
	KStaminaRate = 29,
	kSpeedMult = 30,
	kInventoryWeight = 31,
	kCarryWeight = 32,
	kCriticalChance = 33,
	kMeleeDamage = 34,
	kUnarmedDamage = 35,
	kMass = 36,
	kVoicePoints = 37,
	kVoiceRate = 38,
	kDamageResist = 39,
	kPoisonResist = 40,
	kResistFire = 41,
	kResistShock = 42,
	kResistFrost = 43,
	kResistMagic = 44,
	kResistDisease = 45,
	kUnknown46 = 46,
	kUnknown47 = 47,
	kUnknown48 = 48,
	kUnknown49 = 49,
	kUnknown50 = 50,
	kUnknown51 = 51,
	kUnknown52 = 52,
	kParalysis = 53,
	kInvisibility = 54,
	kNightEye = 55,
	kDetectLifeRange = 56,
	kWaterBreathing = 57,
	kWaterWalking = 58,
	kUnknown59 = 59,
	kFame = 60,
	kInfamy = 61,
	kJumpingBonus = 62,
	kWardPower = 63,
	kRightItemCharge = 64,
	kArmorPerks = 65,
	kShieldPerks = 66,
	kWardDeflection = 67,
	kVariable01 = 68,
	kVariable02 = 69,
	kVariable03 = 70,
	kVariable04 = 71,
	kVariable05 = 72,
	kVariable06 = 73,
	kVariable07 = 74,
	kVariable08 = 75,
	kVariable09 = 76,
	kVariable10 = 77,
	kBowSpeedBonus = 78,
	kFavorActive = 79,
	kFavorsPerDay = 80,
	kFavorsPerDayTimer = 81,
	kLeftItemCharge = 82,
	kAbsorbChance = 83,
	kBlindness = 84,
	kWeaponSpeedMult = 85,
	kShoutRecoveryMult = 86,
	kBowStaggerBonus = 87,
	kTelekinesis = 88,
	kFavorPointsBonus = 89,
	kLastBribedIntimidated = 90,
	kLastFlattered = 91,
	kMovementNoiseMult = 92,
	kBypassVendorStolenCheck = 93,
	kBypassVendorKeywordCheck = 94,
	kWaitingForPlayer = 95,
	kOneHandedModifier = 96,
	kTwoHandedModifier = 97,
	kMarksmanModifier = 98,
	kBlockModifier = 99,
	kSmithingModifier = 100,
	kHeavyArmorModifier = 101,
	kLightArmorModifier = 102,
	kPickpocketModifier = 103,
	kLockpickingModifier = 104,
	kSneakingModifier = 105,
	kAlchemyModifier = 106,
	kSpeechcraftModifier = 107,
	kAlterationModifier = 108,
	kConjurationModifier = 109,
	kDestructionModifier = 110,
	kIllusionModifier = 111,
	kRestorationModifier = 112,
	kEnchantingModifier = 113,
	kOneHandedSkillAdvance = 114,
	kTwoHandedSkillAdvance = 115,
	kMarksmanSkillAdvance = 116,
	kBlockSkillAdvance = 117,
	kSmithingSkillAdvance = 118,
	kHeavyArmorSkillAdvance = 119,
	kLightArmorSkillAdvance = 120,
	kPickpocketSkillAdvance = 121,
	kLockpickingSkillAdvance = 122,
	kSneakingSkillAdvance = 123,
	kAlchemySkillAdvance = 124,
	kSpeechcraftSkillAdvance = 125,
	kAlterationSkillAdvance = 126,
	kConjurationSkillAdvance = 127,
	kDestructionSkillAdvance = 128,
	kIllusionSkillAdvance = 129,
	kRestorationSkillAdvance = 130,
	kEnchantingSkillAdvance = 131,
	kLeftWeaponSpeedMultiply = 132,
	kDragonSouls = 133,
	kCombatHealthRegenMultiply = 134,
	kOneHandedPowerModifier = 135,
	kTwoHandedPowerModifier = 136,
	kMarksmanPowerModifier = 137,
	kBlockPowerModifier = 138,
	kSmithingPowerModifier = 139,
	kHeavyArmorPowerModifier = 140,
	kLightArmorPowerModifier = 141,
	kPickpocketPowerModifier = 142,
	kLockpickingPowerModifier = 143,
	kSneakingPowerModifier = 144,
	kAlchemyPowerModifier = 145,
	kSpeechcraftPowerModifier = 146,
	kAlterationPowerModifier = 147,
	kConjurationPowerModifier = 148,
	kDestructionPowerModifier = 149,
	kIllusionPowerModifier = 150,
	kRestorationPowerModifier = 151,
	kEnchantingPowerModifier = 152,
	kDragonRend = 153,
	kAttackDamageMult = 154,
	kHealRateMult = 155,
	kMagickaRateMult = 156,
	kStaminaRateMult = 157,
	kWerewolfPerks = 158,
	kVampirePerks = 159,
	kGrabActorOffset = 160,
	kGrabbed = 161,
	kUnknown162 = 162,
	kReflectDamage = 163,

	kTotal
};