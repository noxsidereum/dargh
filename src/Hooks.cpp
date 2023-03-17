// ============================================================================
//                                Hooks.cpp
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
#include "DARProjectRegistry.h"
#include "hooks.h"

#include "RE/B/BShkbAnimationGraph.h"
#include "RE/H/hkbCharacterStringData.h"
#include "RE/H/hkbCharacter.h"
#include "RE/H/hkbProjectData.h"
#include "RE/Offsets.h"

#include "SKSE/SafeWrite.h"

// Turn this on if you want to trace & debug the hooks
// (CAUTION: only use for debugging - this will generate large dargh.log files
//  and degrade performance)
// #define DEBUG_TRACE_HOOKS

// HOOK 1: finish constructor for hkbCharacterStringData
uint64_t hkbCharacterStringData_fctor_Orig;
typedef void (*hkbCharacterStringData_fctor)
   (hkbCharacterStringData* thisObj, hkFinishLoadedObjectFlag flag);

// HOOK 2: finish constructor for hkbProjectData
uint64_t hkbProjectData_fctor_Orig;
typedef void (*hkbProjectData_fctor)
   (hkbProjectData* thisObj, hkFinishLoadedObjectFlag flag);

// HOOK 3: hkbClipGenerator::Activate(hkbContext* context)
uint64_t hkbClipGenerator_activate_Orig;
typedef void (*hkbClipGenerator_activate)
   (hkbClipGenerator* thisObj, hkbContext* context);

BSSpinLock lock;
std::unordered_map<hkbCharacterStringData*, hkArray*> g_animHashmap;

void cacheModifiedCharStringData(hkbCharacterStringData* p_hkbCharStringData)
{
	BSSpinLock_lock(&lock);
	g_animHashmap.insert(std::pair(p_hkbCharStringData, &p_hkbCharStringData->animationNames));
	BSSpinLock_unlock(&lock);
}

void hkbCharacterStringData_fctor_Hook(hkbCharacterStringData* thisObj, hkFinishLoadedObjectFlag flag)
{
	// Finish constructor for a hkbCharacterStringData object.
	// In the Havok system, finish constructors are the very last step in order to make
	// an object usable. It's at this point that we actually replace the animation names,
	// and then remove the entry from our cache (g_animHashmap).
	// TODO: Should we free any other memory at this point? Memory leaks?
	BSSpinLock_lock(&lock);
#ifdef DEBUG_TRACE_HOOKS
	_MESSAGE("\n========= HOOK 1: hkbCharacterStringData: FCTOR ========\n");
#endif
	auto& search = g_animHashmap.find(thisObj);
	if (search != g_animHashmap.end())
	{
		// Found it.
		hkArray* animationNames = search->second;
		thisObj->animationNames._data = animationNames->_data;
		thisObj->animationNames._size = animationNames->_size;

		// Clear the entry in the map.
		g_animHashmap.erase(search);
	}
#ifdef DEBUG_TRACE_HOOKS
	_MESSAGE("Unlocking and passing control back to orig function...");
#endif
	BSSpinLock_unlock(&lock);
	((hkbCharacterStringData_fctor)hkbCharacterStringData_fctor_Orig)(thisObj, flag);
}

void hkbProjectData_fctor_Hook(hkbProjectData* thisObj, hkFinishLoadedObjectFlag flag)
{
	// Finish constructor for a hkbProjectData object.
	// We can now clear any refs to this object in our project registry.
	// TODO: Should we free any other memory at this point? Memory leaks?
#ifdef DEBUG_TRACE_HOOKS
	_MESSAGE("\n========= HOOK 2: hkbProjectData FCTOR ========\n");
#endif
	if (DARGH::g_isDARDataLoaded)
	{
		// Nullify all refs to this object in our registry.
		for (auto& entry : DARGH::g_DARProjectRegistry)
		{
			if (entry.second.projData == thisObj)
			{
				entry.second.projData = NULL;
			}
		}
	}
#ifdef DEBUG_TRACE_HOOKS
	_MESSAGE("Passing control back to orig function...");
#endif
	((hkbProjectData_fctor)hkbProjectData_fctor_Orig)(thisObj, flag);
}

void hkbClipGenerator_activate_Hook(hkbClipGenerator* thisObj, hkbContext* context)
{ 
	// Activate a clip generator. This resets the clip generator (which is a type
	// of behaviour graph node) to its initial state, ready for reuse. E.g. inter alia
	// it resets the animation clip to the beginning.
#ifdef DEBUG_TRACE_HOOKS
	_MESSAGE("\n========= HOOK 3: hkbClipGenerator::Activate IN ========\n");
#endif
	hkInt16 origIndex = thisObj->animationBindingIndex;
	if (origIndex == -1)
	{
		return ((hkbClipGenerator_activate)hkbClipGenerator_activate_Orig)(thisObj, context);
	}
	
	DARProject *darProj = DARGH::getDARProject(context->character->projectData);
	if (!darProj)
	{
		return ((hkbClipGenerator_activate)hkbClipGenerator_activate_Orig)(thisObj, context);
	}
		
	BShkbAnimationGraph* animGraph = (BShkbAnimationGraph*)((uint64_t)context->character - 0xC0);
	Actor* actor = animGraph->holder;
	if (!actor) 
	{
		return ((hkbClipGenerator_activate)hkbClipGenerator_activate_Orig)(thisObj, context);
	}
		
	if (actor->ref.form.formType != FormType::ActorCharacter)
	{
		return ((hkbClipGenerator_activate)hkbClipGenerator_activate_Orig)(thisObj, context);
	}
	
	hkInt16 newIndex = DARGH::getNewAnimIndex(darProj, origIndex, actor);
	if (newIndex == -1)
	{
		return ((hkbClipGenerator_activate)hkbClipGenerator_activate_Orig)(thisObj, context);
	}

	// Temporarily change the animation index to point to the replacement, then activate
	// the associated clip generator.
	thisObj->animationBindingIndex = newIndex;
	((hkbClipGenerator_activate)hkbClipGenerator_activate_Orig)(thisObj, context);
	thisObj->animationBindingIndex = origIndex;
}

bool install_hooks()
{
	// The addresses in the comments are those we would expect to see
	// for 1.6.659 (GOG edition). They are intended to facilitate basic
	// sanity checks.

	// HOOK 1.
	_MESSAGE("Installing Hook 1: CharacterStringData FCTOR...");
	uint64_t hkbCharacterStringData_VFT = RE::VTBL_hkbCharacterStringData;
	_MESSAGE("  1. Redirecting fctor VFT pointer at %#010x.", hkbCharacterStringData_VFT);            // 0x418044a8
	_MESSAGE("  2. Before hooking, it points to %#010x.", *(uint64_t*)hkbCharacterStringData_VFT);    // 0x40a3cfa0
	hkbCharacterStringData_fctor_Orig = *(uint64_t*)hkbCharacterStringData_VFT;                       
	SafeWrite64(hkbCharacterStringData_VFT, (uint64_t)&hkbCharacterStringData_fctor_Hook);
	_MESSAGE("  3. After hooking, it points to %#010x.", *(uint64_t*)hkbCharacterStringData_VFT);     // 0x31bf1bc0

	// HOOK 2.
	_MESSAGE("Installing Hook 2: ProjectData FCTOR...");
	uint64_t hkbProjectData_VFT = RE::VTBL_hkbProjectData;
	_MESSAGE("  1. Redirecting fctor VFT pointer at %#010x.", hkbProjectData_VFT);                    // 0x4180ac48
	_MESSAGE("  2. Before hooking, it points to %#010x.", *(uint64_t*)hkbProjectData_VFT);            // 0x40a5b060
	hkbProjectData_fctor_Orig = *(uint64_t*)hkbProjectData_VFT;
	SafeWrite64(hkbProjectData_VFT, (uint64_t)&hkbProjectData_fctor_Hook);
	_MESSAGE("  3. After hooking, it points to %#010x.", *(uint64_t*)hkbProjectData_VFT);             // 0x31bf1df0

	// HOOK 3.
	_MESSAGE("Installing Hook 3: ClipGenerator::Activate...");
	uint64_t hkbClipGenerator_activate = RE::VTBL_hkbClipGenerator + 0x20;  // FOURTH function in the VFT
	_MESSAGE("  1. Redirecting activate VFT pointer at %#010x.", hkbClipGenerator_activate);          // 0x41809f18
	_MESSAGE("  2. Before hooking, it points to %#010x.", *(uint64_t*)hkbClipGenerator_activate);     // 0x40a42760
	hkbClipGenerator_activate_Orig = (*(uint64_t*)hkbClipGenerator_activate);
	SafeWrite64(hkbClipGenerator_activate, (uint64_t)&hkbClipGenerator_activate_Hook);
	_MESSAGE("  3. After hooking, it points to %#010x.", *(uint64_t*)hkbClipGenerator_activate);      // 0x31bf1e80
	
	_MESSAGE("Successfully installed hooks.");
	return true;
}