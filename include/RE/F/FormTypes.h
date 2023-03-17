// ============================================================================
//                              FormTypes.h
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
//    https://github.com/Ryan-rsm-McKenzie/CommonLibSSE/blob/master/include/RE/F/FormTypes.h
#pragma once

enum FormType
{
	None = 0x0,
	PluginInfo = 0x1,
	FormGroup = 0x2,
	GameSetting = 0x3,
	Keyword = 0x4,
	LocationRefType = 0x5,
	Action = 0x6,
	TextureSet = 0x7,
	MenuIcon = 0x8,
	Global = 0x9,
	Class = 0xA,
	Faction = 0xB,
	HeadPart = 0xC,
	Eyes = 0xD,
	Race = 0xE,
	Sound = 0xF,
	AcousticSpace = 0x10,
	Skill = 0x11,
	MagicEffect = 0x12,
	Script = 0x13,
	LandTexture = 0x14,
	Enchantment = 0x15,
	Spell = 0x16,
	Scroll = 0x17,
	Activator = 0x18,
	TalkingActivator = 0x19,
	Armor = 0x1A,
	Book = 0x1B,
	Container = 0x1C,
	Door = 0x1D,
	Ingredient = 0x1E,
	Light = 0x1F,
	Misc = 0x20,
	Apparatus = 0x21,
	Static = 0x22,
	StaticCollection = 0x23,
	MovableStatic = 0x24,
	Grass = 0x25,
	Tree = 0x26,
	Flora = 0x27,
	Furniture = 0x28,
	Weapon = 0x29,
	Ammo = 0x2A,
	NPC = 0x2B,
	LeveledNPC = 0x2C,
	KeyMaster = 0x2D,
	AlchemyItem = 0x2E,
	IdleMarker = 0x2F,
	Note = 0x30,
	ConstructibleObject = 0x31,
	Projectile = 0x32,
	Hazard = 0x33,
	SoulGem = 0x34,
	LeveledItem = 0x35,
	Weather = 0x36,
	Climate = 0x37,
	ShaderParticleGeometryData = 0x38,
	ReferenceEffect = 0x39,
	Region = 0x3A,
	Navigation = 0x3B,
	Cell = 0x3C,
	Reference = 0x3D,
	ActorCharacter = 0x3E,
	ProjectileMissile = 0x3F,
	ProjectileArrow = 0x40,
	ProjectileGrenade = 0x41,
	ProjectileBeam = 0x42,
	ProjectileFlame = 0x43,
	ProjectileCone = 0x44,
	ProjectileBarrier = 0x45,
	PlacedHazard = 0x46,
	WorldSpace = 0x47,
	Land = 0x48,
	NavMesh = 0x49,
	TLOD = 0x4A,
	Dialogue = 0x4B,
	Info = 0x4C,
	Quest = 0x4D,
	Idle = 0x4E,
	Package = 0x4F,
	CombatStyle = 0x50,
	LoadScreen = 0x51,
	LeveledSpell = 0x52,
	AnimatedObject = 0x53,
	Water = 0x54,
	EffectShader = 0x55,
	TOFT = 0x56,
	Explosion = 0x57,
	Debris = 0x58,
	ImageSpace = 0x59,
	ImageAdapter = 0x5A,
	FormList = 0x5B,
	Perk = 0x5C,
	BodyPartData = 0x5D,
	AddonNode = 0x5E,
	ActorValueInfo = 0x5F,
	CameraShot = 0x60,
	CameraPath = 0x61,
	VoiceType = 0x62,
	MaterialType = 0x63,
	Impact = 0x64,
	ImpactDataSet = 0x65,
	Armature = 0x66,
	EncounterZone = 0x67,
	Location = 0x68,
	Message = 0x69,
	Ragdoll = 0x6A,
	DefaultObject = 0x6B,
	LightingMaster = 0x6C,
	MusicType = 0x6D,
	Footstep = 0x6E,
	FootstepSet = 0x6F,
	StoryManagerBranchNode = 0x70,
	StoryManagerQuestNode = 0x71,
	StoryManagerEventNode = 0x72,
	DialogueBranch = 0x73,
	MusicTrack = 0x74,
	DialogueView = 0x75,
	WordOfPower = 0x76,
	Shout = 0x77,
	EquipSlot = 0x78,
	Relationship = 0x79,
	Scene = 0x7A,
	AssociationType = 0x7B,
	Outfit = 0x7C,
	ArtObject = 0x7D,
	MaterialObject = 0x7E,
	MovementType = 0x7F,
	SoundRecord = 0x80,
	DualCastData = 0x81,
	SoundCategory = 0x82,
	SoundOutputModel = 0x83,
	CollisionLayer = 0x84,
	ColorForm = 0x85,
	ReverbParam = 0x86,
	LensFlare = 0x87,
	LensSprite = 0x88,
	VolumetricLighting = 0x89,
	Max = 0x8A,
};