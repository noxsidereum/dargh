// ============================================================================
//                               Plugin.cpp
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
#include "Plugin.h"
#include "DARProjectRegistry.h"
#include "DARProject.h"
#include "Utilities.h"

#include "RE/S/SettingCollectionList.h"
#include "RE/S/Setting.h"
#include "RE/Offsets.h"

namespace Plugin
{
	uint32_t g_MAX_ANIMATION_FILES = 16384;

	void HandleSKSEMessage(SKSEMessagingInterface::Message * msg)
	{
		if (msg->type != SKSEMessagingInterface::kMessage_DataLoaded) return;
		
		// --------------------------------------------------------------------
		//  1. Get the data handler.
		// --------------------------------------------------------------------
		TESDataHandler* dh = *RE::g_dataHandler;
		if (!dh)
		{
			_ERROR("couldn't get DataHandler");
			return;
		}

		// --------------------------------------------------------------------
		//  2. Register two projects (male & female) for each loaded race.
		// --------------------------------------------------------------------
		std::string sProjName;
		for (int i = 0; i < dh->formArrays[FormType::Race].count; ++i)
		{
			TESRace* race = (TESRace*)dh->formArrays[FormType::Race].entries[i];
			if (race)
			{
				// First for the males, second for the females.
				// E.g.
				//    [0] => 'Actors\Character\DefaultMale.hkx'
				//    [1] => 'Actors\Character\DefaultFemale.hkx'
				for (int j = 0; j < 2; ++j)
				{
					char* projName = race->behaviorGraphs[j].model.model;
					if (*projName)
					{
						sProjName.assign(projName);
						DARGH::registerDARProject(sProjName);
					}
				}
			}
		}

		// --------------------------------------------------------------------
		//  3. Register the 1st person project specified in Skyrim's INI file.
		// --------------------------------------------------------------------
		Setting* iniSetting =
			RE::SettingCollectionList_Get(
				*RE::g_iniSettingCollection,
				"strPlayerCharacterBehavior1stPGraph:Animation"
			);
		if (iniSetting 
			&& Setting_GetType(iniSetting) == SettingType::kType_String)
		{
			char** projName = (char**)&iniSetting->data.s;
			if (*projName)
			{
				sProjName.assign(*projName);
				DARGH::registerDARProject(sProjName);
			}
		}	
	
		// --------------------------------------------------------------------
		//  4. Load applicable DAR mappings for the registered projects.
		// --------------------------------------------------------------------
		std::string meshesDir = getSkyrimDirectory() + "data\\meshes\\";
		for (auto& entry : DARGH::g_DARProjectRegistry)
		{
			DARProject& darProj = entry.second;

			std::string darDir =
				meshesDir + darProj.projFolder + 
				"\\animations\\DynamicAnimationReplacer";
			// ... i.e.
			//     "data\meshes\actors\(project folder)\
			//        animations\DynamicAnimationReplacer"

			DARGH::loadDARMaps_ActorBase(darProj, dh, darDir);
			DARGH::loadDARMaps_Conditional(darProj, dh, darDir);
		}
		DARGH::g_isDARDataLoaded = true;
	}
}