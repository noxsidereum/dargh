// ============================================================================
//                               Main.cpp
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
#include "Plugin.h"
#include "Utilities.h"

#include "RE/Offsets.h"

#include "SKSE/skse_version.h"
#include "SKSE/BranchTrampoline.h"

#include <shlobj.h>
#include <vector>
#include <iostream>
#include <shlwapi.h>
#include <filesystem>
#include <fstream>
#include <sstream>

IDebugLog		        gLog;
PluginHandle	        g_pluginHandle = kPluginHandle_Invalid;
SKSEMessagingInterface* g_msgInterface = NULL;

void ProcessDARIniFile()
{
	char value[256];

	// Get the DAR INI path.
	std::string darINIPath = 
		getSkyrimDirectory() + 
		"data\\SKSE\\Plugins\\DynamicAnimationReplacer.ini";

	// Attempt to open it and read its contents.
	// 
	// N.B. DAR appears to use GetPrivateProfileSectionA, create a string
	// vector from the null-delimited results and then iterate over that
	// to find the value for "AnimationLimit", if it exists. No other
	// keys are used. Given we are only interested in the one key, we
	// instead just use GetPrivateProfileStringA.
	GetPrivateProfileString
	("Main", "AnimationLimit", NULL, value, 256, darINIPath.c_str());

	// Print this just to be completely consistent with DAR output :-)
	_MESSAGE("Main");
	if (strcmp(value, ""))
	{
		int iMaxAnimFiles = std::stoi(value, nullptr, 0);
		if (iMaxAnimFiles >= 0)
		{
			Plugin::g_MAX_ANIMATION_FILES = iMaxAnimFiles;
			_MESSAGE("   AnimationLimit  =  %d", iMaxAnimFiles);
		}
	}
}

extern "C"
{
	// We use the same version settings here as
	// DynamicAnimationReplacerAE v1.1.3 for Skyrim 16629,
	// uploaded 9 Jan 2023
	__declspec(dllexport) SKSEPluginVersionData SKSEPlugin_Version =
	{
		SKSEPluginVersionData::kVersion,                                  // data version
		
		1,                                                                // plugin version
		"dargh",                                                          // plugin name

		"nox sidereum",                                                   // author
		"",                                                               // support email

		0,                                                                // versionIndependenceEx
		SKSEPluginVersionData::kVersionIndependent_StructsPost629 | 
		SKSEPluginVersionData::kVersionIndependent_AddressLibraryPostAE,  // versionIndependence
		{ RUNTIME_VERSION_1_6_640, 0 },                                   // compatible flags

		0                                                                 // seVersionRequired
	};

	__declspec(dllexport) bool SKSEPlugin_Load(const SKSEInterface* skse)
	{
		// --------------------- Initialise the log.  ---------------------
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, 
			              "\\My Games\\Skyrim Special Edition GOG\\SKSE\\dargh.log");  
		gLog.SetPrintLevel(IDebugLog::kLevel_Warning);
		gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);

		_MESSAGE("DynamicAnimationReplacer");                                                  
		g_pluginHandle = skse->GetPluginHandle();                                             

		// -------------------- Don't load in editor. --------------------
		if (skse->isEditor) {                     
			_MESSAGE("loaded in editor, marking as incompatible");
			return false;
		}

		// -------------------- Check runtime version. --------------------
		uint32_t runtimeVersion = skse->runtimeVersion;
		if (runtimeVersion < RUNTIME_VERSION_1_6_629) {
			_MESSAGE("unsupported runtime version %08X", runtimeVersion);
			return false;
		}

		// ---------------- Get SKSE messaging interface. ----------------
		g_msgInterface =
			(SKSEMessagingInterface*)skse->QueryInterface(kInterface_Messaging);
		if (!g_msgInterface) {
			_ERROR("couldn't get messaging interface");
			return false;
		}
		int skseMsgInterfaceVersion = g_msgInterface->interfaceVersion;
		if (skseMsgInterfaceVersion == 0) {
			_ERROR("messaging interface too old (%d expected %d)",
				skseMsgInterfaceVersion, 1);
			return false; 
		}

		// ---------- Initialize offsets with address library. ----------
		if (!RE::InitialiseOffsets()) { 
			return false;
		}
		// Dump the address vector confirm it worked properly
		//Plugin::DumpOffsets();

		// ---------- Initialize the two branch trampolines. ----------
		if (!g_branchTrampoline.Create(static_cast<size_t>(1024) * 64))
		{
			_ERROR("couldn't create branch trampoline. this is fatal. skipping remainder of init process.");
			return false;
		}
		if (!g_localTrampoline.Create(static_cast<size_t>(1024) * 64, nullptr))
		{
			_ERROR("couldn't create codegen buffer. this is fatal. skipping remainder of init process.");
			return false;
		}

		// ---------- Process the DAR INI file. ----------
		ProcessDARIniFile();

		// ---------- Install the hooks and trampolines. ----------
		_MESSAGE("------------- Installing hooks and trampolines -------------");
		if (!install_hooks() || !install_trampolines())
		{
			return false;
		}
		_MESSAGE("------------------------------------------------------------");
		
		// ---------- Register for the dataLoaded SKSE callback. ----------
		g_msgInterface->RegisterListener(g_pluginHandle, "SKSE", Plugin::HandleSKSEMessage);
		_MESSAGE("DynamicAnimationReplacer Loaded");
		return true;
	}
}