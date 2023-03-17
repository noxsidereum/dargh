// ============================================================================
//                           Trampolines.cpp
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
#include "hooks.h"
#include "trampolines.h"
#include "DARProjectRegistry.h"
#include "DARLink.h"
#include "Plugin.h"
#include "DebugUtils.h"

#include "RE/B/BShkbAnimationGraph.h"
#include "RE/H/hkbProjectData.h"
#include "RE/H/hkbCharacter.h"
#include "RE/H/hkbCharacterData.h"
#include "RE/H/hkbCharacterStringData.h"
#include "RE/H/hkbClipGenerator.h"
#include "RE/H/hkbAnimationBindingSet.h"
#include "RE/H/hkbContext.h"
#include "RE/Offsets.h"

#include "SKSE/BranchTrampoline.h"

#include "xbyak/xbyak.h"

// Turn this on if you want to trace & debug the trampolines
//  (CAUTION: only use for debugging - this will generate large dargh.log files
//            and degrade performance)
// #define DEBUG_TRACE_TRAMPOLINES

// ============================================================================
//                         FUNCTION SIGNATURES
// ============================================================================
// Direct function pointers
typedef uint64_t  (* _GenerateAnimation)
                  (hkbCharacterStringData*, hkbAnimationBindingSet*,
				   uint64_t, uint64_t, const char*, uint64_t, uint64_t);

typedef void      (* _hkbClipGenerator_Activate)
                  (hkbClipGenerator*, hkbContext*);

// ============================================================================
//                        LOCAL HELPER STRUCTURES
// ============================================================================
// Temporary structures used when generating animations from DAR data:

// Actor base remappings (method 1)
struct obj16_m1
{
	uint32_t        animIndex_orig;
	uint32_t        animIndex_new;
	ActorBaseLink*  ActorBaseLink;
};

// Condition remappings (method 2)
struct obj16_m2
{
	uint32_t        animIndex_orig;
	uint32_t        animIndex_new;
	ConditionLink*  ConditionLink;
};

// Prior to 1.6.629, DAR was using additional trampolines.
// From 1.6.629, the active trampolines are just these two:

// ============================================================================
//                          TRAMPOLINE 1
// ============================================================================
// Instructions near the address Plugin::Unk00, denoted below with a (*), are:
// 
//   40B37282     call    sub_40C70970
//   40B37287     mov     rcx, [rdi+98h]
//   40B3728E     test    rcx, rcx
//   40B37291     jz      short loc_40B372C7
//   40B37293     lea     rdx, Default
//   40B3729A     mov     rax, [rsp+248h+var_40]
//   40B372A2     test    rax, rax
//   40B372A5     cmovnz  rdx, rax
//   40B372A9     mov     [rsp+248h+var_218], rbx
//   40B372AE     mov     [rsp+248h+var_220], r14
//   40B372B3     mov     [rsp+248h+var_228], rdx 
//   40B372B8     mov     r9, r12
//   40B372BB     mov     r8, r15
//   40B372BE     mov     rdx, r13
//   40B372C1     call    GenAnimation_Orig         <= (*) | Modify to call our trampoline, trampoline later
//   40B372C6     nop                               <=       calls this, then jumps back to next instr (nop).
_GenerateAnimation GenAnimation_Orig;

// ============================================================================
//                          TRAMPOLINE 2
// ============================================================================
// Instructions near the address Plugin::hkbClipGenerator_Generate, denoted below with a (*), are:   
// 
//   40A42EB8     mov     rcx, cs:g_AnimationFileManagerSingleton
//   40A42EBF     test    rcx, rcx
//   40A42EC2     jz      short loc_40A42ED7
//   40A42EC4     mov     rax, [rcx]
//   40A42EC7     xor     r9d, r9d
//   40A42ECA     mov     r8, rbx
//   40A42ECD     mov     rdx, rsi
//   40A42ED0     call    qword ptr[rax + 10h]      <= (*) |  WE DO ALL OF THESE INSTRUCTIONS IN THE TRAMPOLINE
//   40A42ED3     test    al, al                           |  After running our code, we call qword ptr[rax + 10h],
//   40A42ED5     jz      short loc_40A42EE3               |  AnimationLoader_Orig, the 2nd function in VFT for the 
//                                                         |  AnimationFileManager, which DAR finds by chasing
//   40A42ED7  loc_40A42ED7 :                              |  pointers from g_AnimationFileManagerSingleton.
//   40A42ED7     mov     rax, [rbx]                       |  
//   40A42EDA     mov     rdx, rsi                         |  If that call returns true, we then call
//   40A42EDD     mov     rcx, rbx                         |  qword ptr[[rbx] + 32], the 4th function in the VFT of
//   40A42EE0     call    qword ptr[rax + 20h]             |  hkbClipGenerator, i.e. the function pointed to by
//                                                         |  0x41809EF8 + 0x20 = 0x41809F18, which is the function
//                                                         |  at 0x40A42760, namely, 
//                                                         |  hkbClipGenerator::activate(hkbCharacter character)
//   40A42EE3  loc_40A42EE3 :                              |
//   40A42EE3     movss   xmm2, dword ptr[rbx + 64h]  <=  trampoline jumps back here (0x13 bytes from (*))
_hkbClipGenerator_Activate hkbClipGenerator_Activate;

// ============================================================================
//                    CODE RELATING TO TRAMPOLINE 1
// ============================================================================
bool g_ShownConditionError = false;

uint64_t GenAnimation_Hook(const char* a7_dar, hkbAnimationBindingSet* a2, uint64_t a3,
	                       uint64_t a4, const char* a5, uint64_t a6, hkbCharacter* a8_dar)
{
	// --------------------------------------------------------------------------------
	// uint64_t GenAnimation_Orig(a1, a2, a3, a4, a5, a6, a7)
	// 
	// Original argument types are:
	//      a1 - hkbCharacterStringData object                     (VFT:  0x418044A8)
	//      a2 - hkbAnimationBindingSet object                     (VFT:  0x417E7A78)
	//      a3 - BSResourceAssetLoader or NullAssetLoader object   (VFT:  0x4182F338 or 0x4182F368 resp.)
	//      a4 - hkbBehaviorGraph object                           (VFT:  0x417E74D0)
	//      a5 - char* strProjectFolder (null-terminated string)
	//      a6 - ?? always 65536?
	//      a7 - always 0
	//
	// In the xbyak trampoline code below, we modify the call to
	// replace a1 and a7 with:
	//      a7_dar - char* and something else?
	//      a8_dar - hkbCharacter object                           (VFT: 0x417E7A98)
	//
	// We restore these args before calling the original function.
	// --------------------------------------------------------------------------------
#ifdef DEBUG_TRACE_TRAMPOLINES
	_MESSAGE("\n--------------------------------------------------------------");
	_MESSAGE("                      GenAnimation_Hook                        ");
	_MESSAGE("---------------------------------------------------------------");
#endif

	// Obtain orig arg 1, in the same way the Skyrim code does.
	hkbCharacterStringData* hkbCharStringData_obj =
		a8_dar->setup->data->m_stringData;
	hkbProjectData* projData = a8_dar->projectData;

	if (DARGH::g_isDARDataLoaded)
	{
		if (projData)
		{
			// Get the full project file path and convert it to lowercase.
			const char* hkxProjFileName = a7_dar + 288;
			std::string projFilePath;
			projFilePath.assign(a7_dar);
			projFilePath.append("\\");
			projFilePath.append(hkxProjFileName);
			std::transform(projFilePath.begin(), projFilePath.end(),
				           projFilePath.begin(), tolower);

			// Do we have an entry for it in our global project data map?
			std::map<std::string, DARProject>::iterator itProj =
				DARGH::g_DARProjectRegistry.find(projFilePath);
			if (itProj != DARGH::g_DARProjectRegistry.end())
			{
				// Found the project.
				// Get the (original) animation names array.
#ifdef DEBUG_TRACE_TRAMPOLINES
				_MESSAGE("Found entry for project file '%s' in g_DARProjectRegistry.",
					     projFilePath.c_str());
#endif
				DARProject& darProj = itProj->second;
				char** datAnimNames_Orig = (char**)hkbCharStringData_obj->animationNames._data;
				uint32_t szAnimNames_Orig = hkbCharStringData_obj->animationNames._size;

				if (szAnimNames_Orig > 0)
				{
					// ------------------------------------------------------------------------------
					// Iterate over the animation names array, see what M1 and/or M2 mappings we have 
					// for each one, temporarily store those mappings in m1data and m2data vectors.
					// ------------------------------------------------------------------------------
					std::vector<obj16_m1> m1data_vec;
					std::vector<obj16_m2> m2data_vec;
					std::string animName_Orig;

					for (uint64_t i = 0; i < szAnimNames_Orig; ++i)
					{
						animName_Orig.assign((const char*)(*(datAnimNames_Orig + i)));
						std::transform(animName_Orig.begin(), animName_Orig.end(),
							           animName_Orig.begin(), tolower);

						// Actor Base replacement animation files (M1).
						for (auto& pM1Obj : darProj.actorBaseLinks)
						{
							if (pM1Obj.from_hkx_file == animName_Orig)
							{
								obj16_m1 m1data;
								m1data.animIndex_orig = i;
								m1data.ActorBaseLink = &pM1Obj;
								m1data_vec.push_back(m1data);
							}
						}

						// Conditional replacement animation files (M2).
						for (auto& pM2Obj : darProj.conditionLinks)
						{
							if (pM2Obj.from_hkx_file == animName_Orig)
							{
								obj16_m2 m2data;
								m2data.animIndex_orig = i;
								m2data.ConditionLink = &pM2Obj;
								m2data_vec.push_back(m2data);
							}
						}
					}  // for (uint64_t i = 0; i < szAnimNames; ++i)

					// ------------------------------------------------------------------------------
					// Calculate the total number of replacement animations and inform the user.
					// Also advise the user if we have breached the MAX_ANIMATION_FILES limit
					// (by default this is 16384).
					// ------------------------------------------------------------------------------
					uint32_t szAnimNames_New = szAnimNames_Orig + m2data_vec.size() + m1data_vec.size();
#ifdef DEBUG_TRACE_TRAMPOLINES
					_MESSAGE("    => Total anim files is %d = %d orig + %d M1 remaps + %d M2 remaps",
						     szAnimNames_New, szAnimNames_Orig, m1data_vec.size(), m2data_vec.size());
#endif
					if (!darProj.animationsLoaded)
					{
						darProj.animationsLoaded = true;
						if (szAnimNames_New <= Plugin::g_MAX_ANIMATION_FILES)
						{
							_MESSAGE("%d / %d : %s", 
								szAnimNames_New, Plugin::g_MAX_ANIMATION_FILES,
								itProj->first.c_str());
						}
						else
						{
							_MESSAGE("Too many animation files. %d / %d : %s",
								szAnimNames_New, Plugin::g_MAX_ANIMATION_FILES,
								itProj->first.c_str());

							// Also display error via an in-game message box:
							std::string msg = "Too many animation files.\n";
							msg += std::to_string(szAnimNames_New);
							msg += " / ";
							msg += std::to_string(Plugin::g_MAX_ANIMATION_FILES);
							msg += "\n";
							RE::showMessageBox(msg.c_str(), 0, 0, 4, 10, "OK", 0);
						}
					}

					// ------------------------------------------------------------------------------
					// If there are available slots for the replacement animations, create
					// a new hkArray with MAX_ANIMATION_FILES elements. Then copy into that:
					//        (1) the M1 remapped animation file names
					//        (2) the M2 remapped animation file names
					//        (3) the original animation file names
					// ------------------------------------------------------------------------------
					if (szAnimNames_Orig < Plugin::g_MAX_ANIMATION_FILES)
					{
						darProj.allLinks.clear();

						char** datAnimNames_New =
							(char**)operator new(8ui64 * Plugin::g_MAX_ANIMATION_FILES);
						if (szAnimNames_New >= Plugin::g_MAX_ANIMATION_FILES)
						{
							// Put empty strings in the new hkArray, but we won't actually
							// copy in any new animation remappings as there are not enough slots to
							// include all of them (we only do all or none).
							char** animName_New = datAnimNames_New;
							for (uint16_t i = 0; i < Plugin::g_MAX_ANIMATION_FILES - szAnimNames_Orig;
								++i, ++animName_New)
							{
								*animName_New = (char*)operator new(1);
								strcpy_s(*animName_New, 1, "");
							}
						}
						else
						{
							// ==============================================
							//      1. COPY ACTOR BASE MAPPINGS (M1)
							// ==============================================
							for (uint16_t i = 0; i < m1data_vec.size(); ++i)
							{
								// Copy all non-null animation names from our M1 vector into our new hkArray.
								const char* toHkxFile_M1 =
									m1data_vec.at(i).ActorBaseLink->to_hkx_file.c_str();
								if (toHkxFile_M1)
								{
									// Create a new string, copy the TO animation name from the
									// m1 vector into it, then stash the pointer in the new array.
									size_t sz = strlen(toHkxFile_M1);
									char* toHkxFile_M1_dup = (char*)operator new(sz + 1);
									strcpy_s(toHkxFile_M1_dup, sz + 1, toHkxFile_M1);
									datAnimNames_New[i] = toHkxFile_M1_dup;
								}
								else
								{
									// No animation name to remap to, so stash a NULL in the new array.
									datAnimNames_New[i] = 0;
								}

								// Calculate a revised animation index for the original FROM animation name.
								uint32_t fromAnimIndex_rev = 
									(Plugin::g_MAX_ANIMATION_FILES - szAnimNames_Orig) +
									m1data_vec[i].animIndex_orig;
									
								// Does the revised anim index already exist in our link data map for this project?
								// I.e. have we already stored M1 mappings to this index?
								auto& search = darProj.allLinks.find(fromAnimIndex_rev);
								if (search == darProj.allLinks.end())
								{
									// --------------------------------------------------------------------
									// Revised FROM animation index was NOT found in the project hash map.
									// --------------------------------------------------------------------
									// Store the relevant data in a new BaseLinkData object.
									BaseLinkData* oBLinkData = new BaseLinkData();
									oBLinkData->allLinks.insert(
										std::pair(m1data_vec[i].ActorBaseLink->actorBaseID, i)
									);

									// Create an ordered map, with <priority> => <BaseLinkData object>
									// BaseLinkData always has a priority of 0.
									std::map<int, LinkData*, std::greater<int>> oMap;
									oMap.insert(std::pair<int, LinkData*>(0, oBLinkData));
									darProj.allLinks.insert(
										std::pair<uint32_t, std::map<int, LinkData*, std::greater<int>>>
										(fromAnimIndex_rev, oMap)
									);
								}
								else
								{
									// --------------------------------------------------------------------
									// Revised FROM animation index WAS found in the project hash map.
									// --------------------------------------------------------------------
									// Retrieve the BaseLinkData object in the existing ordered map and add this
									// TO mapping to that. There should only be one BaseLinkData object in 
									// the map (if not next line will throw exception), with priority of 0.
									BaseLinkData* oBLinkData =
										dynamic_cast<BaseLinkData*>(search->second.at(0));
									oBLinkData->allLinks.insert(
										std::pair(m1data_vec[i].ActorBaseLink->actorBaseID, i)
									);
								}
							} // for (uint16_t i = 0; i < m1data.size(); ++i)
							
							// ==============================================
							//        2. COPY CONDITION MAPPINGS (M2)
							// ==============================================
							uint16_t startIndex = m1data_vec.size();
							uint16_t destIndex;
							for (uint16_t i = 0; i < m2data_vec.size(); i++)
							{
								destIndex = startIndex + i;
								const char* toHkxFile_M2 =
									m2data_vec.at(i).ConditionLink->to_hkx_file.c_str();
								if (toHkxFile_M2)
								{
									// Create a new string, copy the TO animation name from the
									// m2 vector into it, then stash the pointer in the new array.
									size_t sz = strlen(toHkxFile_M2);
									char* toHkxFile_M2_dup = (char*)operator new(sz + 1);
									strcpy_s(toHkxFile_M2_dup, sz + 1, toHkxFile_M2);
									datAnimNames_New[destIndex] = toHkxFile_M2_dup;
								}
								else
								{
									// No animation name to remap to, so stash a NULL in the new array.
									datAnimNames_New[destIndex] = 0;
								}
								
								// Calculate new animation index for the FROM animation name.
								// And get the priority.
								int priority = m2data_vec[i].ConditionLink->priority;
								uint32_t fromAnimIndex_rev = 
									(Plugin::g_MAX_ANIMATION_FILES - szAnimNames_Orig) +
									m2data_vec[i].animIndex_orig;
									
								// Does the new anim index already exist in our link data map for this project?
								// I.e. have we already stored M2 mappings to this index?
								auto& search = darProj.allLinks.find(fromAnimIndex_rev);
								if (search == darProj.allLinks.end())
								{
									// --------------------------------------------------------------------
									// Revised FROM animation index was NOT found in the project hash map.
									// --------------------------------------------------------------------
									// Store the relevant data in a new ConditionLinkData object.
									ConditionLinkData* oCLinkData = new ConditionLinkData();
									oCLinkData->conditions = m2data_vec[i].ConditionLink->conditions;
									oCLinkData->to_hkx_index = destIndex;

									// Create an ordered map, with <priority> => <ConditionLinkData object>.
									// ConditionLinkData can have any priority from -ve to +ve, except 0.
									// Larger numbers mean greater priority (and thus their associated
									// ConditionLinkData objects should appear earlier when iterating
									// over the map).
									std::map<int, LinkData*, std::greater<int>> oMap;
									oMap.insert(std::pair(priority, (LinkData*)oCLinkData));
									darProj.allLinks.insert(
										std::pair<uint32_t, std::map<int, LinkData*, std::greater<int>>>
										(fromAnimIndex_rev, oMap)
									);
								}
								else
								{
									// --------------------------------------------------------------------
									// Revised FROM animation index WAS found in the project hash map.
									// --------------------------------------------------------------------
									// Store the relevant data in a new ConditionLinkData object.
									ConditionLinkData* oCLinkData = new ConditionLinkData();
									oCLinkData->conditions = m2data_vec[i].ConditionLink->conditions;
									oCLinkData->to_hkx_index = destIndex;
									
									// Retrieve the existing ordered map, then add the ConditionLinkData
									// object to it.
									std::map<int, LinkData*, std::greater<int>>& oMap = search->second;
									const auto [it, success2] =
										oMap.insert(std::pair(priority, (LinkData*)oCLinkData));
									if (!success2 && !g_ShownConditionError)
									{
										g_ShownConditionError = true;
										_ERROR("couldn't add conditions");
									}
								}
							} // for (uint16_t i = 0; i < m2data.size(); i++)

							// ============================================================================
							//    3. PAD ANY REMAINING ELEMENTS WITH EMPTY STRINGS, UNTIL WE
							//       REACH THE START OF THE ORIGINAL FILE NAME SLOTS.
							// ============================================================================
							uint16_t j = m2data_vec.size() + m1data_vec.size();
							char** animName_New = &datAnimNames_New[j];
							for (uint16_t i = j; i < Plugin::g_MAX_ANIMATION_FILES - szAnimNames_Orig;
								 ++i, ++animName_New)
							{
								*animName_New = (char*)operator new(1);
								strcpy_s(*animName_New, 1, "");
							}
						} // if (nAnimationFiles <= g_MAX_ANIMATION_FILES)

						// ==============================================
						//    4. FINALLY, COPY THE ORIGINAL FILE NAMES.
						// ==============================================
						uint16_t startIndex = Plugin::g_MAX_ANIMATION_FILES - szAnimNames_Orig;
						char** pDestAnimName = &datAnimNames_New[startIndex];
						char** pSrcAnimName = datAnimNames_Orig;
						for (uint16_t i = startIndex; i < Plugin::g_MAX_ANIMATION_FILES;
							 ++i, ++pDestAnimName, ++pSrcAnimName)
						{
							const char* srcAnimName = *pSrcAnimName;
							char* destAnimName = *pDestAnimName;
							if (srcAnimName != destAnimName)
							{
								if (srcAnimName)
								{
									size_t sz = strlen(srcAnimName);
									*pDestAnimName = (char*)operator new(sz + 1);
									strcpy_s(*pDestAnimName, sz + 1, srcAnimName);
								}
								else
								{
									*pDestAnimName = 0;
								}
							}
						}

						// ==============================================
						//                 4. DONE...!
						// ==============================================
						// We're done: replace the arguments with our modified version.
#ifdef DEBUG_TRACE_TRAMPOLINES
						_MESSAGE("We're done! Overwriting the original mappings...");
#endif
						cacheModifiedCharStringData(hkbCharStringData_obj);
						hkbCharStringData_obj->animationNames._data = (uint64_t)datAnimNames_New;
						hkbCharStringData_obj->animationNames._size = Plugin::g_MAX_ANIMATION_FILES;
						darProj.projData = projData;
					} // if (szAnimNames_Orig < Plugin::g_MAX_ANIMATION_FILES)
				} // if ( szAnimNames_Orig > 0 )
			} // if (itProj != Plugin::g_ProjDataMap.end())
		}
	}

#ifdef DEBUG_TRACE_TRAMPOLINES
	_MESSAGE("Calling function at %08x...", GenAnimation_Orig);
#endif
	return GenAnimation_Orig(hkbCharStringData_obj, a2, a3, a4, a5, a6, 0);
}

struct GenAnimationHook_Code : Xbyak::CodeGenerator
{
	GenAnimationHook_Code(void* buf) : Xbyak::CodeGenerator(4096, buf)
	{
		// Replace the args we want for our hook function.
		mov(ptr[rsp + 0x30], rbp);         // a8_dar (hkbCharacter*) => arg 7
		mov(rcx, rsi);                     // a7_dar (char*) => arg 1  

		// Call our hook function.
		mov(rax, (uintptr_t)GenAnimation_Hook);
		call(rax);

		// Return to original code (RIP-relative addressing). 
		// i.e. here jmp goes to address pointed at by RIP, which
		// is the next instruction after this one. :-)
		jmp(ptr[rip]);
		dq(RE::Unk00 + 0x5);
	}
};

// ============================================================================
//                    CODE RELATING TO TRAMPOLINE 2
// ============================================================================
void AnimationLoader_Hook(uint64_t a1, hkbContext* a2, hkbClipGenerator* a3, uint64_t a4)
{
	// --------------------------------------------------------------------------------
	// Argument types here (unaltered from original) are:
	//      a1 - AnimationFileManagerSingleton object (this)   (VFT:  0x4182FDA0)
	//      a2 - hkbContext object
	//      a3 - hkbClipGenerator object                       (VFT:  0x41809EF8)
	//      a4 - always 0
	// --------------------------------------------------------------------------------
#ifdef DEBUG_TRACE_TRAMPOLINES
	_MESSAGE("\n----------------------------------------------------------------");
	_MESSAGE("                      AnimationLoader_Hook                       ");
	_MESSAGE("-----------------------------------------------------------------");
#endif
	uint16_t origIndex = a3->animationBindingIndex;
	uint16_t newIndex;
	DARProject* darProj;
	
	// Assume here that the hkbCharacter reference is stored in a
	// BShkbAnimationGraph object o, at o.characterInstance (offset 0xC0):
	BShkbAnimationGraph* animGraph =
		(BShkbAnimationGraph*)((uint64_t)a2->character - 0xC0);
	Actor* tesActor = animGraph->holder;

#ifdef DEBUG_TRACE_TRAMPOLINES
	_MESSAGE("Original anim index = %d...", animIndex_Orig);
#endif
	if (origIndex != -1
		&& (darProj =
			 DARGH::getDARProject(a2->character->projectData)) != 0
		&& tesActor != 0
		&& tesActor->ref.form.formType == FormType::ActorCharacter
		&& (newIndex = 
			 DARGH::getNewAnimIndex(darProj, origIndex, tesActor)) != -1)
	{
#ifdef DEBUG_TRACE_TRAMPOLINES
		_MESSAGE("Replacing with index %d.", animIndex_New);
#endif
		// REPLACE ANIMATION
		// Attempt to load the replacement animation file,
		// then, if successful, activate the clip generator.
		a3->animationBindingIndex = newIndex;
		if (RE::AnimationFileManager_Load(a1, a2, a3, a4))
		{
			hkbClipGenerator_Activate(a3, a2);
		}
		a3->animationBindingIndex = origIndex;
	}
	else
	{
#ifdef DEBUG_TRACE_TRAMPOLINES
		_MESSAGE("Not replacing.");
#endif
		// DON'T REPLACE ANIMATION
		// Attempt to load the original animation file,
		// then, if successful, activate the clip generator.
		if (RE::AnimationFileManager_Load(a1, a2, a3, a4))
		{
			hkbClipGenerator_Activate(a3, a2);
		}
	}
}

struct AnimationLoaderHook_Code : Xbyak::CodeGenerator
{
	AnimationLoaderHook_Code(void* buf) : Xbyak::CodeGenerator(4096, buf)
	{
		// Call our hook function.
		mov(rax, (uintptr_t)AnimationLoader_Hook);
		call(rax);

		// Return to original code (RIP-relative addressing). 
		// i.e. here jmp goes to address pointed at by RIP, which
		// is the next instruction after this one. :-)
		jmp(ptr[rip]);
		dq(RE::hkbClipGenerator_Generate + 0x13);
	}
};

bool install_trampolines()
{
	// The addresses in the comments are those we would expect to see
	// for 1.6.659 (GOG edition). They are intended to facilitate basic
	// sanity checks.
	
	// TRAMPOLINE 1.
	_MESSAGE("Installing Trampoline 1: GenAnimation_Hook...");
	uint64_t t1OverwriteCall = RE::Unk00;
	void* codeBuf = g_localTrampoline.StartAlloc();
	GenAnimationHook_Code code1(codeBuf);
	g_localTrampoline.EndAlloc(code1.getCurr());
	_MESSAGE("  1. Consider the 5 bytes at %#010x.",  t1OverwriteCall);                          // 0x40B372C1
	_MESSAGE("  2. These are:");                                                                 //     call
	dumpBytes((char*)t1OverwriteCall, 5);                                                        // E8 2A 70 00 00
	_MESSAGE("  3. Assume this is a RIP-relative call: E8 + 4 byte callee address offset.");
	GenAnimation_Orig = (_GenerateAnimation)((uint64_t)*(int32_t*)                               // *(0x40B372C1 + 1) + 0x40B372C1 + 5
		                (t1OverwriteCall + 1) + t1OverwriteCall + 5);                            // = 0x702A + 0x40B372C1 + 5
	_MESSAGE("  4. Then the original callee address is %#010x.", GenAnimation_Orig);             // = 0x40B3E2F0
	_MESSAGE("  5. Overwriting the 5 bytes with a 5 byte JMP to our trampoline...");
	if (!g_branchTrampoline.Write5Branch(t1OverwriteCall, uintptr_t(code1.getCode())))
	{
		_MESSAGE("FAILED.");
		return false;
	}

	// TRAMPOLINE 2.
	_MESSAGE("Installing Trampoline 2: AnimationLoader_Hook...");
	uint64_t t2OverwriteCall = RE::hkbClipGenerator_Generate;
	_MESSAGE("  1. Consider the 7 bytes at %#010x.", t2OverwriteCall);                           // 0x40A42ED0
	_MESSAGE("  2. These are:");                                                                 //   call     test     jz
	dumpBytes((char*)t2OverwriteCall, 7);                                                        // FF 50 10   84 C0   74 0C
	_MESSAGE("  3. Assume they decode as: 3 byte 'call', 2 byte 'test', 2 byte 'jz'.");
	_MESSAGE("  4. Assume the call is to 3rd VFT func for AnimationFileManagerSingleton.");
	_MESSAGE("     (i.e. AnimationFileManager_Load)");
	_MESSAGE("  5. AnimationFileManager_Load is at %#010x.", RE::AnimationFileManager_Load);     // 0x40B40D10
	// N.B. ... We can't determine the address of the AnimationFileManager VFT
	//      using the Plugin::g_AnimationFileManager singleton object at this time 
	//      because that object hasn't been initialised yet (for 1.6.659, its
	//      constructor is at 0x40B3F6B0, so we DO know its address from static
	//      analysis). It seems the original DAR code uses this approach: deriving 
	//      VFT address dynamically from the singleton object when the trampolines are
	//      called. But a simpler and easier to maintain solution is to include
	//      the SKSE Address Library ID for it and load it in Offsets.cpp, which is
	//      what I'm now doing.
	
	// Overwrite leaves one garbage byte (but shouldn't matter, as no longer accessed):
	_MESSAGE("  6. Overwriting the first 6 bytes with a 6 byte JMP to our trampoline...");
	codeBuf = g_localTrampoline.StartAlloc();
	AnimationLoaderHook_Code code2(codeBuf);
	g_localTrampoline.EndAlloc(code2.getCurr());
	if (!g_branchTrampoline.Write6Branch(t2OverwriteCall, uintptr_t(code2.getCode())))
	{
		_MESSAGE("FAILED.\n");
		return false;
	}

	// Store the address of hkbClipGenerator::Activate
	// (the 4th entry in the VFT for hkbClipGenerator, i.e. offset 0x20)
	// Potential TODO: this is also loaded and stored by Hooks.cpp, so could
	// just do once and share. Irrespective, the perf overhead here is negligible
	// (pointer dereferencing).
	hkbClipGenerator_Activate =
		(_hkbClipGenerator_Activate)*(uint64_t*)(RE::VTBL_hkbClipGenerator + 0x20);
	_MESSAGE("hkbClipGenerator::Activate is at %#010x.",                                         // 0x40A42760
		     hkbClipGenerator_Activate);
	
	_MESSAGE("Successfully installed trampolines.");
	return true;
}