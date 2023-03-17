// ============================================================================
//                             DARProject.h
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
#pragma once
#include "DARLink.h"

#include "RE/T/TESDataHandler.h"

#include <map>
#include <unordered_map>

struct DARProject
{
	// Maps from (from_hkx_index => ordered map with (priority => LinkData))
	// N.B. higher numbers == higher priority so we want them to appear
	// first when iterating, which is why we use the custom comparator
	std::unordered_map<
		uint32_t,
		std::map<int, LinkData*, std::greater<int>>
	> allLinks;
	std::vector<ActorBaseLink> actorBaseLinks;
	std::vector<ConditionLink> conditionLinks;
	std::string projFolder;
	hkbProjectData* projData = NULL;
	bool animationsLoaded = false;
};

namespace DARGH
{
	void loadDARMaps_ActorBase(DARProject& darProj,
		                       TESDataHandler* dh, std::string darDir);

	void loadDARMaps_Conditional(DARProject& darProj,
		                         TESDataHandler* dh, std::string darDir);
}