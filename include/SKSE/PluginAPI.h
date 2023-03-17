#pragma once

typedef uint32_t	PluginHandle;	// treat this as an opaque type

struct PluginInfo
{
	enum
	{
		kInfoVersion = 1
	};

	uint32_t			infoVersion;
	const char* name;
	uint32_t			version;
};

enum
{
	kPluginHandle_Invalid = 0xFFFFFFFF
};

enum
{
	kInterface_Invalid = 0,
	kInterface_Scaleform,
	kInterface_Papyrus,
	kInterface_Serialization,
	kInterface_Task,
	kInterface_Messaging,
	kInterface_Object,
	kInterface_Trampoline,
	kInterface_Max,
};

struct SKSEInterface
{
	uint32_t	skseVersion;
	uint32_t	runtimeVersion;
	uint32_t	editorVersion;
	uint32_t	isEditor;
	void* (*QueryInterface)(uint32_t id);

	// call during your Query or Load functions to get a PluginHandle uniquely identifying your plugin
	// invalid if called at any other time, so call it once and save the result
	PluginHandle(*GetPluginHandle)(void);

	// returns the SKSE build's release index
	uint32_t(*GetReleaseIndex)(void);

	// Minimum SKSE version 2.0.18
	// returns the plugin info structure for a plugin by name, only valid to be called after PostLoad message
	//const PluginInfo* (*GetPluginInfo)(const char* name);
};

/**** Messaging API docs ********************************************************************
 *
 *	Messaging API allows inter-plugin communication at run-time. A plugin may register
 *	one callback for each plugin from which it wishes to receive messages, specifying
 *	the sender by name in the call to RegisterListener(). RegisterListener returns false
 *	if the specified plugin is not loaded, true otherwise. Any messages dispatched by
 *	the specified plugin will then be forwarded to the listener as they occur. Passing NULL as 
 *	the sender registers the calling plugin as a listener to every loaded plugin.
 *
 *	Messages may be dispatched via Dispatch() to either a specific listener (specified
 *	by name) or to all listeners (with NULL passed as the receiver). The contents and format of
 *	messageData are left up to the sender, and the receiver is responsible for casting the message
 *	to the expected type. If no plugins are registered as listeners for the sender, 
 *	Dispatch() returns false, otherwise it returns true.
 *
 *	Calling RegisterListener() or Dispatch() during plugin load is not advised as the requested plugin
 *	may not yet be loaded at that point. Instead, if you wish to register as a listener or dispatch a
 *	message immediately after plugin load, use RegisterListener() during load to register to receive
 *	messages from SKSE (sender name: "SKSE"). You will then receive a message from SKSE once 
 *	all plugins have been loaded, at which point it is safe to establish communications between
 *	plugins.
 *
 *	Some plugin authors may wish to use strings instead of integers to denote message type. In
 *	that case the receiver can pass the address of the string as an integer and require the receiver
 *	to cast it back to a char* on receipt.
 *
 *********************************************************************************************/

struct SKSEMessagingInterface
{
	struct Message {
		const char	* sender;
		uint32_t		type;
		uint32_t		dataLen;
		void		* data;
	};

	typedef void (* EventCallback)(Message* msg);

	enum {
		kInterfaceVersion = 2
	};

	// SKSE messages
	enum {
		kMessage_PostLoad,		// sent to registered plugins once all plugins have been loaded (no data)
		kMessage_PostPostLoad,	// sent right after kMessage_PostLoad to facilitate the correct dispatching/registering of messages/listeners
		kMessage_PreLoadGame,	// dispatched immediately before savegame is read by Fallout
								// dataLen: length of file path, data: char* file path of .ess savegame file
		kMessage_PostLoadGame,	//dispatched after an attempt to load a saved game has finished (the game's LoadGame() routine
								//has returned). You will probably want to handle this event if your plugin uses a Preload callback
								//as there is a chance that after that callback is invoked the game will encounter an error
								//while loading the saved game (eg. corrupted save) which may require you to reset some of your
								//plugin state.
								//data: bool, true if game successfully loaded, false otherwise
								// plugins may register as listeners during the first callback while deferring dispatches until the next
		kMessage_SaveGame,
		kMessage_DeleteGame,	// sent right before deleting the .skse cosave and the .ess save.
								// dataLen: length of file path, data: char* file path of .ess savegame file
		kMessage_InputLoaded,	// sent right after game input is loaded, right before the main menu initializes
		kMessage_NewGame,		// sent after a new game is created, before the game has loaded (Sends CharGen TESQuest pointer)
		kMessage_DataLoaded		// send after the data handler has loaded all its forms
	};

	uint32_t	interfaceVersion;
	bool	(* RegisterListener)(PluginHandle listener, const char* sender, EventCallback handler);
	bool	(* Dispatch)(PluginHandle sender, uint32_t messageType, void * data, uint32_t dataLen, const char* receiver);

	enum
	{
		kDispatcher_ModEvent = 0,
		kDispatcher_CameraEvent,
		kDispatcher_CrosshairEvent,
		kDispatcher_ActionEvent,
		kDispatcher_NiNodeUpdateEvent
	};

	// Use this to acquire SKSE's internal EventDispatchers so that you can sink to them
	void	* (* GetEventDispatcher)(uint32_t dispatcherId);
};

struct SKSEPluginVersionData
{
	enum
	{
		kVersion = 1,
	};

	enum
	{
		// set this if you are using a post-AE version of the Address Library
		kVersionIndependent_AddressLibraryPostAE = 1 << 0,
		// set this if you exclusively use signature matching to find your addresses and have NO HARDCODED ADDRESSES
		kVersionIndependent_Signatures = 1 << 1,
		// set this if you are using 1.6.629+ compatible structure layout (likely provided by CommonLib/SKSE)
		// this also marks you as incompatible with pre-1.6.629. see kVersionIndependentEx_NoStructUse if you have a
		// plugin that only does code patches and works across many versions
		kVersionIndependent_StructsPost629 = 1 << 2,
	};

	enum
	{
		// set this if your plugin either doesn't use any game structures or has put in extraordinary effort
		// to work with pre and post 1.6.629 structure layout
		kVersionIndependentEx_NoStructUse = 1 << 0,
	};

	uint32_t	dataVersion;			// set to kVersion

	uint32_t	pluginVersion;			// version number of your plugin
	char	name[256];				    // null-terminated ASCII plugin name

	char	author[256];			    // null-terminated ASCII plugin author name (can be empty)
	char	supportEmail[252];		    // null-terminated ASCII support email address (can be empty)
	                                    // this is not for showing to users, it's in case I need to contact you about
	                                    // a compatibility problem with your plugin

    // version compatibility
	uint32_t	versionIndependenceEx;	// set to one of the kVersionIndependentEx_ enums or zero
	uint32_t	versionIndependence;	// set to one of the kVersionIndependent_ enums or zero
	uint32_t	compatibleVersions[16];	// zero-terminated list of RUNTIME_VERSION_ defines your plugin is compatible with

	uint32_t	seVersionRequired;		// minimum version of the script extender required, compared against PACKED_SKSE_VERSION
	                                    // you probably should just set this to 0 unless you know what you are doing
};
