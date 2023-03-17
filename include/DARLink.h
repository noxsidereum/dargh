// ============================================================================
//                                 DARLink.h
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
#include "RE/A/Actor.h"
#include "RE/B/BSSpinLock.h"
#include "RE/H/hkbCharacterStringData.h"
#include "RE/H/hkbProjectData.h"

#include <vector>
#include <unordered_map>
#include <variant>

// Used for actor base data (method 1) ------------
struct ActorBaseLink
{
	std::string   from_hkx_file;                           // animation file path to map FROM
	std::string   to_hkx_file;                             // animation file path to map TO
	uint32_t      actorBaseID = 0;                         // complete actor form ID: i.e. modIndex | (actor base)
};
// ------------------------------------------------

// Used for condition data (method 2) -------------
struct ConditionLinkFunc
{
	void* funcPtr;                                         // function pointer
	std::vector<std::variant<uint32_t, float>> args;       // function args: form IDs (unsigned ints) or floats
	uint32_t bmArgIsFloat;                                 // bit mask - if the bit is set, the corresponding arg is a float.
	bool bNot = false;                                     // result of this condition should be NOTed
	bool bAnd = true;                                      // result of this condition should be ANDed with the next condition
	bool bESPNotLoaded = false;                            // mod associated with this condition is not loaded.
};

struct ConditionLink
{
	std::string from_hkx_file;                             // animation file path to map FROM
	std::string to_hkx_file;                               // animation file path to map TO
	int32_t priority;                                      // condition link's priority
	std::vector<ConditionLinkFunc> conditions;             // conditions to evaluate
};
// ------------------------------------------------

class LinkData
{
public:
	virtual hkInt16 getNewAnimIndex(Actor* actor) = 0;
};

class BaseLinkData : public LinkData
{
public:
	// key: actor base form ID, val: to_hkx_index
	std::unordered_map<uint32_t, uint16_t> allLinks;

	hkInt16 getNewAnimIndex(Actor* actor) override
	{
		TESForm* baseForm = actor->ref.baseForm;
		if (!baseForm) {
			// Base form is invalid (NULL)
			return -1;
		}

		// Try to find a mapped index for the
		// given actor base form ID.
		auto& search = allLinks.find(baseForm->formID);
		if (search == allLinks.end())
		{
			// Not found.
			return -1;
		}
		return search->second;
	}
};

class ConditionLinkData : public LinkData
{
public:
	std::vector<ConditionLinkFunc> conditions;
	uint16_t to_hkx_index;

private:
	bool evaluateConditions(std::vector<ConditionLinkFunc> vecFuncData, Actor* actor)
	{
		bool bTrueOr = false;    // If true the last evaluated expression was true || ...
		bool bCond;

		// Evaluate the chain of conditions.
		for (auto& curFuncData : vecFuncData)
		{
			if (bTrueOr)
			{
				// Don't need to evaluate this expression.

				// We have true || ... which evaluates to true, 
				// irrespective of what the second expression is. 
				// So don't waste time evaluating it. Move onto the next one.
				if (curFuncData.bAnd)
				{
					// This breaks our ability to avoid evaluating.
					// We now have (true || ... && ...) == (true && ...)
					bTrueOr = false;
				}
			}
			else
			{
				// Need to evaluate this expression.
				if (curFuncData.bESPNotLoaded)
				{
					bCond = false;
				}
				else
				{
					bCond =
						((bool(__fastcall*)(Actor*, std::variant<uint32_t, float>*, uint32_t))curFuncData.funcPtr)
						(actor, curFuncData.args.data(), curFuncData.bmArgIsFloat);
				}

				if (bCond == curFuncData.bNot)
				{
					// We have either
					//   !true or false
					// both of which evaluate to false.
					if (curFuncData.bAnd)
					{
						// We have false && ...
						// which always evaluates to false.
						// No need to evaluate any further expressions.
						return false;
					}

					// We have false || ...
					// Need to evaluate the next expression.
				}
				else
				{
					// We have true (&& or ||) ...
					// If we have true || ... we don't need
					// to evaluate the next condition.
					bTrueOr = !curFuncData.bAnd;
				}
			}
		}
		return true;
	}

public:
	hkInt16 getNewAnimIndex(Actor* actor) override
	{
		if (evaluateConditions(conditions, actor)) {
			// Conditions evaluated to true, return the mapped index.
			return to_hkx_index;
		}
		return -1;
	}
};