// ============================================================================
//                             DebugUtils.cpp
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
#include "DebugUtils.h"

#include "RE/H/hkbCharacterStringData.h"
#include "RE/H/hkbClipGenerator.h"
#include "RE/H/hkbProjectData.h"

void dumpBytes(char* ptr, int nbytes)
{
	// nbytes should be a multiple of 8
	// Print in form
	//    <address>:  <byte0> ... <byte7>   <asc0> ... <asc7>
	//                             ...
	if (!ptr) {
		_MESSAGE("Pointer is NULL, can't dump bytes.");
		return;
	}
	std::string s1;
	std::string s2;
	char buf[64];
	for (int i = 0; i < nbytes; i += 8)
	{
		sprintf_s(buf, sizeof(buf), "%08x : ", (uint32_t)ptr);
		s1.assign(buf);
		s2 = "";
		for (int j = 0; j < 8; j++, ptr++)
		{
			sprintf_s(buf, sizeof(buf), " %02x", (uint8_t)*ptr);
			s1.append(buf);
			if (*ptr > 31 && *ptr < 127)
			{
				sprintf_s(buf, sizeof(buf), " %c", (uint8_t)*ptr);
				s2.append(buf);
			}
			else
			{
				s2 += "  ";
			}
		}
		s1 += s2;
		_MESSAGE(s1.c_str());
	}
}

void dumpHkArrayStringPtr(std::string header, hkArray stringPtrArray)
{
	uint32_t sz = stringPtrArray._size;
	if (sz > 0)
	{
		_MESSAGE("%s (numelements = %d)", header.c_str(), sz);
		for (uint64_t i = 0; i < sz; i++)
		{
			hkStringPtr str =
				(hkStringPtr)(stringPtrArray._data + (i * 8));
			if (str) {
				_MESSAGE("  [%04d] '%s'", i, *(char**)str);
			}
			else
			{
				_MESSAGE("  [%04d] <NULL PTR>", i);
			}
		}
	}
}

void dumpHkArrayAssetBundle(std::string header, hkArray assetBundleArray)
{
	uint32_t sz = assetBundleArray._size;
	if (sz > 0)
	{
		_MESSAGE("%s (numelements = %d)", header.c_str(), sz);
		for (uint64_t i = 0; i < sz; i++)
		{
			hkbAssetBundleStringData* assetBundle =
				(hkbAssetBundleStringData*)(assetBundleArray._data + (i * 8));
			_MESSAGE("  [%04d] '%s'", i, (char*)assetBundle->m_bundleName);
		}
	}
}

void dumpHkArrayFileNameMeshNamePair(std::string header, hkArray fileNameMeshNamePairArray)
{
	uint32_t sz = fileNameMeshNamePairArray._size;
	if (sz > 0)
	{
		_MESSAGE("%s (numelements = %d)", header.c_str(), sz);
		for (uint64_t i = 0; i < sz; i++)
		{
			FileNameMeshNamePair* pair =
				(FileNameMeshNamePair*)(fileNameMeshNamePairArray._data + (i * 8));
			_MESSAGE("  [%04d] ('%s', '%s')", i,
				(char*)pair->m_fileName, (char*)pair->m_meshName);
		}
	}
}

void dumpHkbCharacterStringData(hkbCharacterStringData* thisObj)
{
	_MESSAGE("\n--------------------------------------------------------------");
	_MESSAGE("           fctor: hkbCharacterStringData object [%08x]            ", thisObj);
	_MESSAGE("                         thread ID: %08x", GetCurrentThreadId());
	_MESSAGE("-----------------------------------------------------------------");
	//	dumpBytes((char*)thisObj, 192);

	dumpHkArrayFileNameMeshNamePair("deformableSkinNames:", thisObj->deformableSkinNames);
	dumpHkArrayFileNameMeshNamePair("rigidSkinNames:", thisObj->rigidSkinNames);
	dumpHkArrayAssetBundle("animationNames:", thisObj->animationNames);
	dumpHkArrayAssetBundle("animationBundleFilenameData:", thisObj->animationBundleFilenameData);
	dumpHkArrayStringPtr("characterPropertyNames:", thisObj->characterPropertyNames);
	dumpHkArrayStringPtr("retargetingSkeletonMapperFilenames:", thisObj->retargetingSkeletonMapperFilenames);
	dumpHkArrayStringPtr("lodNames:", thisObj->lodNames);
	dumpHkArrayStringPtr("mirroredSyncPointSubstringsA:", thisObj->mirroredSyncPointSubstringsA);
	dumpHkArrayStringPtr("mirroredSyncPointSubstringsB:", thisObj->mirroredSyncPointSubstringsB);

	if (thisObj->name && *((char*)thisObj->name))
	{
		_MESSAGE("name:             '%s'", (char*)thisObj->name);
	}
	if (thisObj->rigName && *((char*)thisObj->rigName))
	{
		_MESSAGE("rigName:          '%s'", (char*)thisObj->rigName);
	}
	if (thisObj->ragdollName && *((char*)thisObj->ragdollName))
	{
		_MESSAGE("ragdollName:      '%s'", (char*)thisObj->ragdollName);
	}
	if (thisObj->behaviorFilename && *((char*)thisObj->behaviorFilename))
	{
		_MESSAGE("behaviorFilename: '%s'", (char*)thisObj->behaviorFilename);
	}
}

void dumpHkbProjectData(hkbProjectData* thisObj)
{
	_MESSAGE("\n--------------------------------------------------------------");
	_MESSAGE("             fctor: hkbProjectData object [%08x]                  ", thisObj);
	_MESSAGE("                         thread ID: %08x", GetCurrentThreadId());
	_MESSAGE("-----------------------------------------------------------------");
	//dumpBytes((char*)thisObj, 1024);

	if (thisObj->stringData->animationPath && *((char*)thisObj->stringData->animationPath))
	{
		_MESSAGE("animationPath:    '%s'", *(char**)thisObj->stringData->animationPath);
	}
	if (thisObj->stringData->behaviorPath && *((char*)thisObj->stringData->behaviorPath))
	{
		_MESSAGE("behaviorPath:     '%s'", *(char**)thisObj->stringData->behaviorPath);
	}
	if (thisObj->stringData->characterPath && *((char*)thisObj->stringData->characterPath))
	{
		_MESSAGE("characterPath:    '%s'", *(char**)thisObj->stringData->characterPath);
	}
	if (thisObj->stringData->scriptsPath && *((char*)thisObj->stringData->scriptsPath))
	{
		_MESSAGE("scriptsPath:      '%s'", *(char**)thisObj->stringData->scriptsPath);
	}
	if (thisObj->stringData->fullPathToSource && *((char*)thisObj->stringData->fullPathToSource))
	{
		_MESSAGE("fullPathToSource: '%s'", *(char**)thisObj->stringData->fullPathToSource);
	}
	if (thisObj->stringData->rootPath && *((char*)thisObj->stringData->rootPath))
	{
		_MESSAGE("rootPath:         '%s'", *(char**)thisObj->stringData->rootPath);
	}
	dumpHkArrayStringPtr("animationFilenames:", thisObj->stringData->animationFilenames);
	dumpHkArrayStringPtr("behaviorFilenames:", thisObj->stringData->behaviorFilenames);
	dumpHkArrayStringPtr("characterFilenames:", thisObj->stringData->characterFilenames);
	dumpHkArrayStringPtr("eventNames:", thisObj->stringData->eventNames);
}

// Use this to ensure the log text is not interleaved
// (handles multithreaded output correctly)
CRITICAL_SECTION CriticalSection;
void initCriticalSection()
{
	if (!InitializeCriticalSectionAndSpinCount(&CriticalSection, 0x00000400))
	{
		_ERROR("Couldn't initialise critical section.");
	}
}

void dumpHkbClipGenerator(hkbClipGenerator* thisObj)
{
	// Request ownership of the critical section.
	EnterCriticalSection(&CriticalSection);

	_MESSAGE("\n--------------------------------------------------------------");
	_MESSAGE("             hkbClipGenerator::Activate [%08x]                ", thisObj);
	_MESSAGE("                         thread ID: %08x", GetCurrentThreadId());
	_MESSAGE("-----------------------------------------------------------------");
	//_MESSAGE("flag.m_finishing = %d", flag.m_finishing);

	if (thisObj->node.name) {
		_MESSAGE("name:                         '%s'", (char*)thisObj->node.name);
	}
	_MESSAGE("id:                           %d", thisObj->node.id);
	if (thisObj->animationName) {
		_MESSAGE("animationName:                '%s'", (char*)thisObj->animationName);
	}
	if (thisObj->triggers) {
		_MESSAGE("triggers:                     %08x", thisObj->triggers);
	}
	_MESSAGE("cropStartAmountLocalTime:     %.6f", thisObj->cropStartAmountLocalTime);
	_MESSAGE("cropEndAmountLocalTime:       %.6f", thisObj->cropEndAmountLocalTime);
	_MESSAGE("startTime:                    %.6f", thisObj->startTime);
	_MESSAGE("playbackSpeed:                %.6f", thisObj->playbackSpeed);
	_MESSAGE("enforcedDuration:             %.6f", thisObj->enforcedDuration);
	_MESSAGE("userControlledTimeFraction:   %.6f", thisObj->userControlledTimeFraction);
	_MESSAGE("animationBindingIndex:        %d", thisObj->animationBindingIndex);
	switch (thisObj->mode) {
	case MODE_SINGLE_PLAY:
		_MESSAGE("mode: MODE_SINGLE_PLAY");
		break;
	case MODE_LOOPING:
		_MESSAGE("mode: MODE_LOOPING");
		break;
	case MODE_USER_CONTROLLED:
		_MESSAGE("mode: MODE_USER_CONTROLLED");
		break;
	case MODE_PING_PONG:
		_MESSAGE("mode: MODE_PING_PONG");
		break;
	default:
		_MESSAGE("mode: UNKOWN");
		break;
	}
	_MESSAGE("flags:                  %d", (int)thisObj->flags);

	_MESSAGE("Passing control back to orig function...");
	LeaveCriticalSection(&CriticalSection);  // Release ownership of the critical section.
}