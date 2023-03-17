// ============================================================================
//                            DARProject.cpp
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
#include "DARProject.h"
#include "Utilities.h"
#include "Conditions.h"

#include "RE/T/TESFile.h"

#include <fstream>
#include <algorithm>

// Turn this on if you want to trace & debug DAR data loading.
//#define DEBUG_TRACE_DAR_LOADING

// Turn this on if you want to trace & debug animation condition evaluation
// (CAUTION: only use for debugging - this will generate large dargh.log files
//  and degrade performance)
// #define DEBUG_TRACE_CONDITION_EVAL

// Temporary structures used when reading in the DAR data.
struct modIndexAndIsESL
{
	uint32_t modIndex;
	bool isESL;
};

struct modNameActorBaseId
{
	std::string modName;
	int actorBaseID = 0;
	uint32_t modIndex = 0;
};

namespace DARGH
{
	hkInt16 getNewAnimIndex(DARProject* darProj,
		                    hkInt16 from_hkx_index, Actor* actor)
	{
		// ====================================================================
		//                        getNewAnimIndex
		// --------------------------------------------------------------------
		// Searches the project node for any potential mappings from
		// 'from_hkx_index'. Search is done in order of descending priority
		// number (i.e. 345 is higher priority than 3). Queries each potential
		// mapping in turn to see if it should be applied (the mapping will
		// return -1 if NO, otherwise the new index). Stops on the first
		// mapping that doesn't return -1 and returns that index. If no
		// mappings found or they all return -1, returns -1.
		// ====================================================================
		hkInt16 to_hkx_index;

		// Try to find the orig index.
		auto& search = darProj->allLinks.find(from_hkx_index);
		if (search == darProj->allLinks.end())
		{
			// Not found
			return -1;
		}

		// Found it. Now return the new animation index of the first link
		// data item in that map (over which we iterate in order from higher
		// priority number to lower priority), that returns a valid index.
#ifdef DEBUG_TRACE_CONDITION_EVAL
		_MESSAGE("getNewAnimIndex: found potential mapping(s) for index %d",
			from_hkx_index);
#endif
		std::map<int, LinkData*, std::greater<int>> all_links = search->second;
		for (auto& link : all_links)
		{
#ifdef DEBUG_TRACE_CONDITION_EVAL
			_MESSAGE("getNewAnimIndex: apply map with priority %d...?",
				link.first);
#endif
			auto& link_dat = link.second;
			to_hkx_index = link_dat->getNewAnimIndex(actor);
			if (to_hkx_index != -1)
			{
#ifdef DEBUG_TRACE_CONDITION_EVAL
				_MESSAGE("  => yes, new anim index = %d", to_hkx_index);
#endif
				return to_hkx_index;
			}
#ifdef DEBUG_TRACE_CONDITION_EVAL
			_MESSAGE("  => no");
#endif
		}
#ifdef DEBUG_TRACE_CONDITION_EVAL
		_MESSAGE(" => no applicable mappings.");
#endif
		return -1;
	}

	void loadDARMaps_ActorBase(DARProject& darProj,
		                       TESDataHandler* dh, std::string darDir)
	{
		// ====================================================================
		//            METHOD 1: Assignment depending on ActorBase
		// --------------------------------------------------------------------
		// Loads all valid DAR M1 animation file links for the given project.
		// This data is loaded into darProj.actorBaseLinks.
		// 
		// Method 1 description from the DAR documentation:
		// 
		// " Animations placed in the following folder:
		// 
		//       meshes\actors\(project folder)\animations\
		//       DynamicAnimationReplacer\(esp name)\(actor base id)\
	    //      (animation folders and files)
		//
		//  (actor base id) must be 8 characters. First two characters are 00.
		// 
		//  E.g. mapping could be:
		//   
		//    meshes\actors\character\animations\1hm_attackpowerleft.hkx =>
		//      meshes\actors\character\animations\DynamicAnimationReplacer\
		//        Skyrim.esm\00000007\1hm_attackpowerleft.hkx                 "
		// ====================================================================

		// Get (esp name) subfolders.
		std::vector<std::string> modNames;
		if (!findMatchingFiles(darDir, modNames, 0, 0,
			                   std::string(""), std::string("")))
		{
			_WARNING("couldn't find %s\\animations\\DynamicAnimationReplacer",
				      darProj.projFolder.c_str());
			// No subfolders found. No mappings to load for this project.
			return;
		}

		// Iterate over the matches, find those that are ESP, ESM or ESL,
		// determine if they are active, and if so, store the mod info data.
		std::unordered_map<std::string, modIndexAndIsESL> modInfoMap;
		for (auto& modName : modNames)
		{
			if (!endsWith(modName, ".esp")
				&& !endsWith(modName, ".esm")
				&& !endsWith(modName, ".esl"))
			{
				// WARNING: Not a valid mod name. Don't load the mappings.
				continue;    // Skip to next (esp name) subfolder.
			}

			// Ok, this is an ESP, ESM or ESL. Is it active?
			const TESFile* modinfo = LookupModByName(dh, modName.c_str());
			if (!modinfo)
			{
				// WARNING: The mod is not active. Don't load the mappings.
				_WARNING("esp file not loaded: %s", modName.c_str());
				continue;    // Skip to the next (esp name) subfolder.
			}

			// Yes mod is active. Store (mod_name, index) tuple in modInfoMap,
			// as information we'll later use to recreate the complete form ID.
			modIndexAndIsESL modDat;
			modDat.isESL = ((modinfo->recordFlags & 0x200) != 0);
			modDat.modIndex =
				(modinfo->compileIndex << 24) + 
				(modinfo->smallFileCompileIndex << 12);
			modInfoMap.insert(
				std::pair<std::string, modIndexAndIsESL>(modName, modDat)
			);
		}

		// Iterate over the valid (esp name) subfolders stored in
		// modInfoMap and find their (actor base id) subfolders.
		for (auto& modInfo : modInfoMap)
		{
			std::string espDir = darDir + "\\" + modInfo.first;
			// ... i.e.:
			//     "data\meshes\actors\(project folder)\animations\
			//      DynamicAnimationReplacer\(esp name)"

			// Get the (actor base id) subfolders.
			std::vector<std::string> sActorBaseIDs;
			findMatchingFiles(espDir, sActorBaseIDs, 0, 0,
				              std::string(""), std::string(""));
			std::unordered_map<std::string, modNameActorBaseId> mActorBaseIDs;
			for (auto& sActorBaseID : sActorBaseIDs)
			{
				if (sActorBaseID.size() != 8)
				{
					// *** USER ERROR ***
					// The base ID should be exactly 8 digits,
					// with first two digits 00.
					continue;    // Skip to next (actor base id) subfolder.
				}

				// ------------------------------------------------------------
				//                  FORM ID RECONSTRUCTION
				// ------------------------------------------------------------
				// We need to reconstruct the complete form ID by combining the
				// runtime information (mod index and light mod index) with the
				// static reference provided by the user. All Form IDs are 4
				// bytes (eight characters in hex format).
				// 
				// For .esm and .esp mods, the complete form ID has the
				// structure xx yy yy yy. Where the first two digits xx are
				// determined at runtime - they are the load index of the mod.
				// FF is reserved for dynamically allocated IDs.  The other
				// digits yy yy yy are static and provided by the user. So
				// the user could write "0xyyyyyy" or "0x00yyyyyy".
				// 
				// For .esl mods, the complete form ID has the structure
				// FE xxx yyy. Where xxx is the light mod load index and yyy is
				// provided by the user. In this case user could write "0xyyy"
				// or "0x00000yyy".
				// 
				// Useful references at:
				//   https://en.uesp.net/wiki/Skyrim:Form_ID and
				//   https://skyrim.fandom.com/wiki/ID
				// ------------------------------------------------------------
				std::string strHex = "0x" + sActorBaseID;
				uint32_t iActorBaseID = std::stoi(strHex, nullptr, 0);

				if (iActorBaseID <= 0xFFFFFF)
				{
					// Only add the mapping if the user's actor base hex string
					// is valid and begins with two zeros.
					modNameActorBaseId dat;
					dat.modName = modInfo.first;
					dat.actorBaseID = iActorBaseID;
					dat.modIndex = modInfo.second.modIndex;
					mActorBaseIDs.insert(
						std::pair<std::string, modNameActorBaseId>
						(sActorBaseID, dat)
					);
				}
			}

			// Find and store all the animation HKX mappings in each of the
			// valid actor base directories (and their subdirectories, if any).
			for (auto& mActorBaseID : mActorBaseIDs)
			{
				std::string actorBaseDir = espDir + "\\" + mActorBaseID.first;
				// ... i.e. looks like:
				//     "data\meshes\actors\(project folder)\animations\
				//      DynamicAnimationReplacer\(esp name)\(actor base id)"
				std::vector<std::string> hkxFiles;
				findMatchingFiles(actorBaseDir, hkxFiles, 1, 1,
					              std::string(".hkx"), std::string(""));
				for (auto& hkxFile : hkxFiles)
				{
					std::string fromHkx = "Animations\\" + hkxFile;
					std::transform(fromHkx.begin(), fromHkx.end(),
						           fromHkx.begin(), tolower);
					// ... i.e. looks like:
					//     "animations\<hkx file>"
					// all lower case, what to map FROM

					std::string toHkx =
						"Animations\\DynamicAnimationReplacer\\" +
						mActorBaseID.second.modName + "\\" +
						mActorBaseID.first + "\\" + hkxFile;
					// ... i.e. looks like:
					//     "Animations\DynamicAnimationReplacer\(mod name)\
					//      (actor base ID)\<hkx file>"
					// not lower-cased, what to map TO

					// Store the link.
					// 
					// We generate the complete actorBaseID by xoring the mod
					// index with the partial base ID provided by the user.
					// 
					// E.g. if Dawnguard.esm is the third mod in the load order
					// and the user provides the base ID for Serana we would
					// have something like
					//        
					//   02000000 xor 00002B6C = 02002B6C
					//
					// which is the complete form ID
					ActorBaseLink actorBaseLink;
					actorBaseLink.from_hkx_file = fromHkx;
					actorBaseLink.to_hkx_file = toHkx;
					actorBaseLink.actorBaseID =
						mActorBaseID.second.modIndex +
						mActorBaseID.second.actorBaseID;
					darProj.actorBaseLinks.push_back(actorBaseLink);

#ifdef DEBUG_TRACE_DAR_LOADING
					_MESSAGE("  M1: stored link: '%s' => '%s' (%d)",
						     actorBaseLink.from_hkx_file.c_str(),
						     actorBaseLink.to_hkx_file.c_str(),
						     actorBaseLink.actorBaseID);
#endif
				}
			}
		}
	}

	void loadDARMaps_Conditional(DARProject& darProj,
		                         TESDataHandler* dh, std::string darDir)
	{
		// ====================================================================
		//         METHOD 2: Assignment depending on custom conditions
		// --------------------------------------------------------------------
		// Loads all valid DAR M2 animation file links for the given project.
		// The data is loaded into darProj.conditionLinks.
		// 
		// Method 2 description from the DAR documentation:
		// 
		// " Set the conditions yourself. Assigns animations accordingly. Place
		//   the animation files and _conditions.txt in the following folder.
		// 
		//      meshes\actors\(project folder)\animations\
		//      DynamicAnimationReplacer\_CustomConditions\<Priority>\
	    //      (animation folders and files and _conditions.txt)
		//
		//   <Priority> is a decimal number excluding 0 in the range of
		//   -2,147,483,648 to 2,147,483,647. The higher the number, the higher
		//   the priority. Assignments that depend on ActorBase mentioned above
		//   have a priority 0.
		// 
		//   _conditions.txt is a file with a text format named _conditions.
		//   Specify functions to set conditions in this file. Multiple
		//   conditions can be linked with AND and OR. You can use NOT to
		//   negate a condition.
		// 
		//     (NOT) Function name("esp name" | formID, ...) (AND or OR)
		// 
		//   The esp name is enclosed in "". Prefix with 0x to specify the
		//   FormID in hexadecimal. Remove or replace the first two digits
		//   indicating the load order. 
		//      
		//     Example: 0xAA123456 -> 0x00123456
		// 
		//   Specify esp name and FormID as the arguments of the function as
		//   follows.
		//   
		//     IsEquippedRight(Form item) ->
		//         IsEquippedRight("aaa.esp" | 0x00123456)
		// 
		//   It is also possible to specify the number directly to
		//   GlobalVariable.
		//     
		//     IsEquippedRightType(GlobalVariable type) ->
		//         IsEquippedRightType(3)
		// 
		//   Example:
		//    
		//     Actors who have an iron dagger equipped to the right hand and
		//     are in the exterior.
		// 
		//	     IsEquippedRight("Skyrim.esm" | 0x0001397E) AND
		//       NOT IsInInterior()
		// 
		//   Logical operation is performed in the same way as CK.
		// 
		//     (A || B) && C
		//      = A || B && C                                                 "
		// ====================================================================
		std::string customCondDir = darDir + "\\" + "_CustomConditions";
		// ... i.e:
		//     "data\meshes\actors\(project folder)\animations\
		//      DynamicAnimationReplacer\_CustomConditions"

		// --------------------------------------------------------------------
		//           Load data from each of the priority subfolders.
		// --------------------------------------------------------------------
		std::vector<std::string> sPriorities;
		if (!findMatchingFiles(customCondDir, sPriorities, 0, 0,
			std::string(""), std::string("")))
		{
			// No subfolders found. No mappings to load for this project.
			return;
		}

		// We have at least one subfolder.
		for (auto& sPriority : sPriorities)
		{
			// Check that priority string is valid.
			if (sPriority.size() == 0
				|| sPriority.at(0) == '0'
				|| (!isNumber(sPriority) &&
					(sPriority.size() < 2
						|| sPriority.at(0) != '-'
						|| sPriority.at(1) == '0'
						|| !isNumber(sPriority.substr(1))
						)
					)
				)
			{
				// *** USER ERROR ***
				// Invalid priority string.
				continue;    //  Skip to next priority subfolder.
			}

			// Convert priority string to integer.
			int iPriority = strtol(sPriority.c_str(), nullptr, 0);
			if (iPriority == 0)
			{
				// *** USER ERROR ***
				// We only process <Priority> != 0.
				// As per spec, conditions with priority == 0 are ignored.
				continue;    //  Skip to next priority subfolder.
			}
			std::string priorityDir = customCondDir + "\\" + sPriority;
			// ... i.e:
			//     "data\meshes\actors\(project folder)\animations\
			//      DynamicAnimationReplacer\_CustomConditions\(Priority)"

			// ----------------------------------------------------------------
			//                Parse the _conditions.txt file.
			// ----------------------------------------------------------------
			std::string condFilePath = priorityDir + "\\_conditions.txt";
			std::ifstream fConditions;
			fConditions.open(condFilePath);   // by default opens read-only
			if (!fConditions.is_open())
			{
				// *** WARNING ***
				// Can't open the conditions file.
				_WARNING("couldn't find %s\\animations\\DynamicAnimationReplacer\\_CustomConditions\\%s\\_conditions.txt",
					darProj.projFolder.c_str(), sPriority);
				continue;    //  Skip to next priority subfolder.
			}

			// Read lines of "_conditions.txt" file into our string vector.
			std::vector<std::string> vLines;
			std::string sLine;
			while (std::getline(fConditions, sLine))
			{
				sLine = trim(sLine);
				if (sLine.size() > 0 && sLine.at(0) != ';')
				{
					vLines.push_back(sLine);
				}
			}
			fConditions.close();

			// Process each line.
			// Parsed results are stored in vFuncData.
			// If parsing fails, this stores the line where it happened:
			std::string lineWithError = "";
			std::vector<ConditionLinkFunc> conditions;
			for (int iLineNum = 0; iLineNum < vLines.size(); ++iLineNum)
			{
				std::string full_line = vLines.at(iLineNum);
				std::string chomped_line = vLines.at(iLineNum);

				bool bNot = false;
				bool bAnd = true;
				bool bESPNotLoaded = false;

				// Does this line start with a "NOT"?
				// Must have at least one trailing space or one trailing tab.
				if (startsWith(chomped_line, "NOT ") ||
					startsWith(chomped_line, "NOT\t"))
				{
					bNot = true;
					chomped_line = chomped_line.substr(3);   // chomp
				}

				// Get the position of the opening bracket.
				std::size_t posLB = chomped_line.find_first_of("(");
				if (posLB == std::string::npos)
				{
					// *** USER ERROR ***
					// There is no opening bracket on this line.
					lineWithError.assign(full_line);
					break;    //  Stop parsing conditions file.
				}

				// Get the function name.
				// We assume this is all the text up to the opening bracket,
				// with optional lead and trailing whitespace.
				std::string funcName = chomped_line.substr(0, posLB);
				funcName = trim(funcName);
				std::size_t posRB = chomped_line.find_first_of(")");

				// Look up the function address from the name.
				auto& funcInfoPair = g_DARConditionFuncs.find(funcName);
				if (funcInfoPair == g_DARConditionFuncs.end()
					|| posRB == std::string::npos || posRB < posLB)
				{
					// *** USER ERROR ***
					// Specified function name was not found
					// (see Conditions.cpp), or there was a
					// function name parsing error.
					lineWithError.assign(full_line);
					break;    //  Stop parsing conditions file.
				}

				if (posRB == chomped_line.size() - 1)
				{
					// Nothing after the closing bracket on this line.
					if (iLineNum < vLines.size() - 1)
					{
						// *** USER ERROR ***
						// There are no additional characters on this line
						// after the ")", but there are still more lines
						// in the file. Which means we don't know how to
						// interpret those additional conditions (should they
						// be ANDed or ORed to this one?)
						lineWithError.assign(full_line);
						funcName = "";
						break;    //  Stop parsing conditions file.
					}
				}
				else
				{
					// There are further characters after the ")".
					// (could just be whitespace).
					std::string rest =
						chomped_line.substr(
							posRB + 1,
							chomped_line.size() - (posRB + 1)
						);
					rest = trim(rest);
					if (rest.size() > 0)
					{
						// The further characters are not just whitespace.
						if (startsWith(rest, "AND"))
						{
							bAnd = true;
						}
						else
						{
							if (!startsWith(rest, "OR"))
							{
								// *** USER ERROR ***
								// The user has written non-whitespace
								// characters at the end of this line with
								// something other than "AND" or "OR".
								lineWithError.assign(full_line);
								break;    //  Stop parsing conditions file.
							}
							bAnd = false;
						}
					}
					else if (iLineNum < vLines.size() - 1)
					{
						// *** USER ERROR ***
						// The last few characters on this line are all
						// whitespace, but there are still further lines
						// in the file. Which means we don't know how to
						// interpret those additional conditions (should
						// they be ANDed or ORed to this one?)
						lineWithError.assign(full_line);
						break;    //  Stop parsing the conditions file.
					}
				}

				// ------------------------------------------------------------
				//                   Parse any arguments.
				// ------------------------------------------------------------
				// 'bmArgIsFloat' is a bitmask; if a bit is set it indicates
				// that the corresponding arg is global variable or float.
				std::vector<std::variant<uint32_t, float>> vArgs;
				uint32_t bmArgIsFloat = 0;
				if (posLB != posRB - 1)
				{
					// There is something between the brackets
					// (presumably the arguments...)
					std::string commaSepArgs =
						chomped_line.substr(posLB + 1, posRB - (posLB + 1));
					commaSepArgs = trim(commaSepArgs);
					if (commaSepArgs.size() == 0)
					{
						// No arguments to process.
						continue;    //  Skip to next priority subfolder.
					}

					// Parse the arguments.
					std::vector<std::string> sArgs;
					splitOnCommas(sArgs, commaSepArgs);
					for (auto& sArg : sArgs)
					{
						sArg = trim(sArg);
						if (sArg.size() == 0 || sArg.at(0) == '"')
						{
							// ------------------------------------------------
							//    Argument should be "esp name" | formID
							// ------------------------------------------------
							// Note for newbies: Global variables are also 
							// specified with formIDs. For more on formIDs and
							// how to recreate them, see note earlier above.
							// ------------------------------------------------
							std::vector<std::string> sArgTokens;
							splitOnPipes(sArgTokens, sArg);
							if (sArgTokens.size() != 2)
							{
								// *** USER ERROR ***
								// User hasn't specified exactly two items in
								// the pipe-delimited list comprising this arg.
								lineWithError.assign(full_line);
								break;    //  Stop parsing arguments.
							}

							// ------------------------------------------------
							//          Process token 1: "esp name".
							// ------------------------------------------------
							std::string espName = sArgTokens.at(0);
							espName = trim(espName);

							if (espName.size() <= 2 || espName.at(0) != '"'
								|| espName.at(espName.size() - 1) != '"')
							{
								// *** USER ERROR ***
								// User hasn't specified the mod name with the
								// correct syntax ("<mod name>").
								lineWithError.assign(full_line);
								break;    //  Stop parsing arguments.
							}

							// Remove the surrounding quotes
							espName = espName.substr(1, espName.size() - 2);
							if (!endsWith(espName, ".esp") &&
								!endsWith(espName, ".esm") &&
								!endsWith(espName, ".esl"))
							{
								// *** USER ERROR ***
								// User has provided an invalid mod name
								// (doesn't end in extension ".esp", ".esm"
								// or ".esl")
								lineWithError.assign(full_line);
								break;    //  Stop parsing arguments.
							}

							// Ok, this is an ESP, ESM or ESL. Is it active?
							uint32_t modIndex = 0;
							bool bIsESL = false;
							const TESFile* modinfo =
								LookupModByName(dh, espName.c_str());
							if (!modinfo)
							{
								// Mod is not active.
								// *DON'T* break... these cases actually do
								// still get added to the conditions list,
								// with a special flag.
								_WARNING("esp file not loaded: %s",
									     espName.c_str());
								bESPNotLoaded = true;
							}
							else
							{
								bIsESL = ((modinfo->recordFlags & 0x200) != 0);
								modIndex =
									(modinfo->compileIndex << 24) +
									(modinfo->smallFileCompileIndex << 12);
							}

							// ------------------------------------------------
							//            Process token 2: form ID.
							// ------------------------------------------------
							std::string sFormBaseID = sArgTokens.at(1);
							sFormBaseID = trim(sFormBaseID);
							uint32_t iFormBaseID =
								std::stoi(sFormBaseID, nullptr, 0);

							// Ensure that the supplied actorBaseID is valid,
							// depending on whether the given mod is light or
							// not. If valid, add to our list.
							bool bIsValidBaseID = false;
							if (bIsESL)
							{
								// .esl (light mod):
								// As said earlier, user should provide a valid
								// hex string of no more than yyy digits.
								bIsValidBaseID = (iFormBaseID <= 0xFFF);
							}
							else
							{
								// .esp or .esm:
								// As said earlier, user should provide a valid
								// hex string of no more than yyyyyyyy digits.
								bIsValidBaseID = (iFormBaseID <= 0xFFFFFF);
							}

							if (!bIsValidBaseID)
							{
								// *** USER ERROR ***
								// User hasn't provided a valid base form ID
								// for the given mod type (ESL or non-ESL).
								lineWithError.assign(full_line);
								break;    //  Stop parsing the arguments.
							}

							// Append evaluated arg to the vector.
							iFormBaseID = modIndex + iFormBaseID;
							vArgs.push_back(iFormBaseID);
						}
						else
						{
							// ------------------------------------------------
							//           Argument should be a float.
							// ------------------------------------------------
							uint32_t flagArgIsFloat = 1 << vArgs.size();
							float fVal = std::stof(sArg);

							// Check the actual arg type against the expected
							// arg mask in 'funcInfoPair'. This mask has the
							// corresponding bit set when the argument can be
							// a float (args can always be specified as formIds,
							// i.e. "esp name" | formID).
							if ((flagArgIsFloat &
								funcInfoPair->second.bmArgIsFloat) == 0
								|| std::isnan(fVal))
							{
								// *** USER ERROR ***
								// User has either provided a value that is NaN
								// or their float value is valid, but this arg
								// in the corresponding function must be a
								// form ID.
								lineWithError.assign(full_line);
								break;    //  Stop parsing arguments.
							}

							// Record the arg type and append evaluated arg
							// to the vector.
							bmArgIsFloat |= flagArgIsFloat;
							vArgs.push_back(fVal);
						}
					} // for (auto& sArg : vArgsAsStr)
				} // if (posLB != posRB - 1)

				if (vArgs.size() != funcInfoPair->second.nArgs)
				{
					// *** USER ERROR ***
					// User hasn't provided the required number of arguments
					// for the specified function.
					lineWithError.assign(full_line);
					break;    //  Stop parsing conditions file.
				}

				// All validation checks passed - store the condition data.
				ConditionLinkFunc condition;
				condition.funcPtr = funcInfoPair->second.funcPtr;
				condition.bmArgIsFloat = bmArgIsFloat;
				condition.args = vArgs;
				condition.bNot = bNot;
				condition.bAnd = bAnd;
				condition.bESPNotLoaded = bESPNotLoaded;
				conditions.push_back(condition);
			}  // for (int i = 0; i < lines.size(); ++i)

			// We've finished parsing the conditions file - did we have a
			// parsing error?
			if (lineWithError.size() > 0)
			{
				// Yes. Log the error and skip this conditions file.
				_ERROR("error: %s\\animations\\DynamicAnimationReplacer\\_CustomConditions\\%s\\_conditions.txt",
					   darProj.projFolder.c_str(), sPriority);
				_ERROR("   %s", lineWithError.c_str());
				continue;    //  Skip to next priority subfolder.
			}

			// No errors.
			// Find and store all the animation HKX mappings in the directory
			// (including its sub-directories, if any).
			std::vector<std::string> hkxFiles;
			findMatchingFiles(priorityDir, hkxFiles, 1, 1,
				              std::string(".hkx"), std::string(""));
			for (auto& hkxFile : hkxFiles)
			{
				std::string fromHkx =
					"Animations\\" + hkxFile;
				std::transform(fromHkx.begin(), fromHkx.end(),
					           fromHkx.begin(), tolower);
				// ... i.e.
				//     "animations\\<hkx file>"
				// all lower case, what to map FROM.

				std::string toHkx =
					"Animations\\DynamicAnimationReplacer\\_CustomConditions\\" +
					sPriority + "\\" + hkxFile;
				// ... i.e.:
				//     "Animations\DynamicAnimationReplacer\_CustomConditions\
				//      (mod name)\<Priority>\<subdir path to hkx file>"
				// not lower-cased, what to map TO.

				ConditionLink conditionLink;
				conditionLink.from_hkx_file = fromHkx;
				conditionLink.to_hkx_file = toHkx;
				conditionLink.priority = iPriority;
				conditionLink.conditions = conditions;
				darProj.conditionLinks.push_back(conditionLink);

				// Debug message:
#ifdef DEBUG_TRACE_DAR_LOADING
				_MESSAGE("  M2: stored link: '%s' => '%s' (priority: %d)",
					     conditionLink.from_hkx_file.c_str(),
					     conditionLink.to_hkx_file.c_str(),
					     conditionLink.priority);
#endif
			} // for (auto& hkxFile : hkxFiles)		
		} // for (auto& sPriority : sPriorities)
	}
}