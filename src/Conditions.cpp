// ============================================================================
//                            Conditions.cpp
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
#include "Conditions.h"

#include "RE/A/Actor.h"
#include "RE/A/ActorValues.h"
#include "RE/B/BGSLocation.h"
#include "RE/B/BGSLocationRefType.h"
#include "RE/B/BSExtraData.h"
#include "RE/E/ExtraContainerChanges.h"
#include "RE/E/ExtraDataTypes.h"
#include "RE/E/ExtraLocationRefType.h"
#include "RE/E/EffectSetting.h"
#include "RE/F/FormTypes.h"
#include "RE/M/MagicItem.h"
#include "RE/S/Sky.h"
#include "RE/T/TESFaction.h"
#include "RE/T/TESGlobal.h"
#include "RE/T/TESObjectCell.h"
#include "RE/T/TESObjectWEAP.h"
#include "RE/T/TESNPC.h"
#include "RE/T/TESWeather.h"
#include "RE/T/TESWorldSpace.h"
#include "RE/Offsets.h"

#include <corecrt_math_defines.h>   // for M_PI constant
#include <random>
#include <variant>

static double TWO_PI = 2.0 * M_PI;

// Turn this on if you want to trace & debug animation condition calls
// (CAUTION: only use for debugging - this will generate large dargh.log files
//  and degrade performance)
// #define DEBUG_TRACE_CONDITIONS

// ============================================================================
//                        FUNCTION SIGNATURES
// ============================================================================
// Indirect function pointers, e.g. pointers to VFT addresses (so **)
typedef TESObjectREFR*  (** _TESForm_AsReference2)(TESForm* form);
typedef bool            (** _TESForm_IsFormTypeChild)(Actor* actor);
typedef bool            (** _TESObjectREFR_HasKeywordHelper)(TESObjectREFR*, const BGSKeyword*);
typedef bool            (** _Actor_IsInFaction)(Actor* actor, TESForm* faction);
typedef bool            (** _Actor_HasKeyword)(Actor* actor, TESForm* keyword);
typedef bool            (** _Actor_IsInCombat)(Actor* actor);
typedef float           (** _ActorValueOwner_GetActorValue)(ActorValueOwner* owner, uint32_t a_akValue);
typedef float           (** _ActorValueOwner_GetPermanentActorValue)(ActorValueOwner* owner, uint32_t a_akValue);
typedef float           (** _ActorValueOwner_GetBaseActorValue)(ActorValueOwner* owner, uint32_t a_akValue);
typedef bool            (** _BGSKeywordForm_HasKeyword)(BGSKeywordForm* kwForm, const BGSKeyword* keyword);
typedef uint32_t        (** _BSExtraData_GetType)(BSExtraData* data);

// ============================================================================
//                          HELPER FUNCTIONS
// ============================================================================
static const BGSKeyword* g_kwWarhammer;

bool readGlobalVars(float* values, std::variant<uint32_t, float>* args, uint32_t bmArgIsFloat, int nArgs)
{
    // ---------------------------------------------------------------------------------------------
    // Read 'nArgs' global variables or direct values (depending on whether the corresponding bit
    // is set in 'bmArgIsFloat') from 'args' into 'values'. We expect that 'values' has sufficient
    // memory allocated.
    // ---------------------------------------------------------------------------------------------
    // Dereference the global variables as necessary (or store the direct values).
    int powerOfTwo = 1;
    for (int argIndex = 0; argIndex < nArgs; argIndex++)
    {
        if ((powerOfTwo & bmArgIsFloat) != 0)
        {
            // Argument is a float.
            values[argIndex] = std::get<float>(args[argIndex]);
        }
        else
        {
            // Argument should be a global variable.
            // Dereference it and get its current value.
            TESGlobal* form =
                (TESGlobal*)RE::Game_GetForm(std::get<uint32_t>(args[argIndex]));
            if (!form || form->form.formType != FormType::Global)
            {
                return false;
            }
            values[argIndex] = form->value;
        }
        powerOfTwo *= 2;
    }
    return true;
}

int getEquippedFormType(TESForm* form)
{
    // From DAR documentation:
    // 
    // "Item types are as follows.
    //   -1 = Others
    //    0 = Fists
    //    1 = Swords
    //    2 = Daggers
    //    3 = War Axes
    //    4 = Maces
    //    5 = Greatswords
    //    6 = Battleaxes
    //    7 = Bows
    //    8 = Staff
    //    9 = Crossbows
    //    10 = Warhammers
    //    11 = Shields
    //    12 = Alteration Spells
    //    13 = Illusion Spells
    //    14 = Destruction Spells
    //    15 = Conjuration Spells
    //    16 = Restoration Spells
    //    17 = Scrolls
    //    18 = Torches"
    if (!form)
    {
        return 0;
    }

    // Spell form type
    if (form->formType == FormType::Spell)
    {
        EffectSetting* pEffectSetting = ((MagicItem*)form)->avEffectSetting;
        if (pEffectSetting)
        {
            ActorValue assocSkill = pEffectSetting->data.associatedSkill;
            if (assocSkill == ActorValue::kAlteration) {
                // Alteration spells
                return 12;
            }
            if (assocSkill == ActorValue::kConjuration) {
                // Conjuration spells
                return 15;
            }
            if (assocSkill == ActorValue::kDestruction) {
                // Destruction spells
                return 14;
            }
            if (assocSkill == ActorValue::kIllusion) {
                // Illusion spells
                return 13;
            }
            if (assocSkill == ActorValue::kRestoration) {
                // Restoration spells
                return 16;
            }
        }

        // Others
        return -1;
    }

    // Scroll form type
    if (form->formType == FormType::Scroll)
    {
        // Scrolls
        return 17;
    }

    if (form->formType == FormType::Armor)
    {
        if ((*(uint64_t*)(form + 0x1B8) & 0x200) != 0)
        {
            // Shields
            return 11;
        }
    }
    else
    {
        if (form->formType == FormType::Light)
        {
            // Torches
            return 18;
        }
        else
        {
            if (form->formType != FormType::Weapon)
            {
                // Others
                return -1;
            }

            TESObjectWEAP* formWeap = (TESObjectWEAP*)form;

            // What type of weapon?
            uint8_t animType = formWeap->weaponData.animationType;
            if (animType >= WEAPON_TYPE::kTotal_WeapType)
            {
                // Not recognised - set to Others
                return -1;
            }
            if (animType != WEAPON_TYPE::kTwoHandAxe)
            {
                return animType;
            }

            // The base form for warhammers
            g_kwWarhammer = (const BGSKeyword*)RE::Game_GetForm(0x6D930);

            // Have we got a reference to a warhammer?
            // Call the fourth function virtual function in BGSKeywordForm's VFT,
            // i.e. virtual bool HasKeyword(const BGSKeyword* a_keyword) const;  // 04
            if ((*(_BGSKeywordForm_HasKeyword)(formWeap->keywordForm.pVFT + 0x20))
                (&formWeap->keywordForm, g_kwWarhammer))
            {
                // Warhammers
                return 10;
            }
            else
            {
                // No, so return 6 (two-handed axe)
                return animType;
            }
        }
    }

    // Others
    return -1;
}

bool hasKeyword(TESForm* form, const BGSKeyword* keyword)
{
    BGSKeywordForm* keywordForm;
    bool result = false;

    switch (form->formType)
    {
    case Race:
        keywordForm = (BGSKeywordForm*)((char*)form + 0x70);
        break;
    case MagicEffect:
    case Enchantment:
    case Spell:
    case Scroll:
    case Ingredient:
    case AlchemyItem:
        keywordForm = (BGSKeywordForm*)((char*)form + 0x40);
        break;
    case Activator:
    case TalkingActivator:
    case Flora:
    case Furniture:
        keywordForm = (BGSKeywordForm*)((char*)form + 0x90);
        break;
    case Armor:
        keywordForm = (BGSKeywordForm*)((char*)form + 0x1D8);
        break;
    case Book:
    case Ammo:
        keywordForm = (BGSKeywordForm*)((char*)form + 0xF8);
        break;
    case Misc:
    case Apparatus:
    case KeyMaster:
    case SoulGem:
        keywordForm = (BGSKeywordForm*)((char*)form + 0xE8);
        break;
    case Weapon:
        keywordForm = (BGSKeywordForm*)((char*)form + 0x140);
        break;
    case NPC:
        keywordForm = (BGSKeywordForm*)((char*)form + 0x110);
        break;
    case Location:
        keywordForm = (BGSKeywordForm*)((char*)form + 0x30);
        break;
    default:
        keywordForm = 
            (BGSKeywordForm*)RE::Runtime_DynamicCast
            (form, 0, RE::RTTI_TESForm, RE::RTTI_BGSKeywordForm, 0);
        break;
    }
    if (keywordForm)
    {
        // Call the fourth function virtual function in BGSKeywordForm's VFT,
        // i.e. virtual bool HasKeyword(const BGSKeyword* a_keyword) const;  // 04
        result = 
            (*(_BGSKeywordForm_HasKeyword)(keywordForm->pVFT + 0x20))
            (keywordForm, keyword);
    }
    else
    {
        // Call the virtual function in TESForm's VFT at position 0x2C (== 0x160 / 0x8),
        // i.e. virtual const TESObjectREFR* AsReference2() const;   // 2C
        TESObjectREFR *objRefr = 
            (*(_TESForm_AsReference2)(form->pVft + 0x160))(form);
        if (objRefr) 
        {
            // Now call the virtual function in TESObjectREFR's VFT at position 0x40 (== 0x240 / 0x8)
            // i.e. virtual bool HasKeywordHelper(const BGSKeyword* a_keyword) const;  // 0x48
            result = 
                (*(_TESObjectREFR_HasKeywordHelper)(objRefr->form.pVft + 0x240))
                (objRefr, keyword);
        }
    }
    return result;
}

bool hasKeywordBoundObj(TESBoundObject* obj, std::variant<uint32_t, float>* args)
{
    TESForm* form = (TESForm*)&obj->tesObj.form;
    if (form)
    {
        BGSKeyword* keyword =
            (BGSKeyword*)RE::Game_GetForm(std::get<uint32_t>(args[0]));
        if (keyword)
        {
            if (keyword->form.formType == FormType::Keyword
                && hasKeyword(form, keyword))
            {
                return true;
            }
        }
    }
    return false;
}

float getActorValPct(Actor* actor, uint32_t value)
{
    float fActorValue =
        (*(_ActorValueOwner_GetActorValue)(actor->actorValueOwner.pVFT + 8))
        (&actor->actorValueOwner, value);
    float fPermActorValue =
        (*(_ActorValueOwner_GetPermanentActorValue)(actor->actorValueOwner.pVFT + 0x10))
        (&actor->actorValueOwner, value);

    if (fPermActorValue <= 0.0)
    {
        return 1.0;
    }
    if (fActorValue > 0.0)
    {
        if (fActorValue >= fPermActorValue)
        {
            return 1.0;
        }
        return fActorValue / fPermActorValue;
    }
    return 0.0;
}

bool ExtraDataList_HasType(ExtraDataList* dataList, uint32_t extraDataType)
{
    // 'extraDataType' is of type ExtraDataType  (TODO: enforce this properly)
    bool ret = false;
    BSReadWriteLock* lock = &dataList->_lock;
    RE::BSReadWriteLock_LockForRead(&dataList->_lock);
    PresenceBitfield* presence = dataList->_extraData.presence;
    if (presence)
    {
        ret = ((uint8_t)(1 << (extraDataType & 7)) & presence->bits[extraDataType >> 3]) != 0;
    }
    RE::BSReadWriteLock_UnlockRead(lock);
    return ret;
}

BSExtraData* ExtraDataList_GetByTypeImpl(ExtraDataList* dataList, uint32_t extraDataType)
{
    // 'extraDataType' is of type ExtraDataType  (TODO: enforce this properly)
    BSExtraData* data = NULL;
    BSReadWriteLock* lock = &dataList->_lock;
    RE::BSReadWriteLock_LockForRead(&dataList->_lock);
    if (ExtraDataList_HasType(dataList, extraDataType)
        && (data = dataList->_extraData.data) != NULL)
    {
        while ((*(_BSExtraData_GetType)(data->pVFT + 8))(data) != extraDataType)
        {
            data = data->next;
            if (!data)
            {
                break;
            }
        }
    }
    RE::BSReadWriteLock_UnlockRead(lock);
    return data;
}

// ============================================================================
//                          CONDITION FUNCTIONS
// ============================================================================
bool IsEquippedRight(Actor* actor, std::variant<uint32_t, float>* args)
{
    // -------------------------------------------------------------------
    // IsEquippedRight(Form item)
    // Does the actor have the specified item equipped to his right hand?
    // -------------------------------------------------------------------
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsEquippedRight(%08x)", args[0]);
#endif
    AIProcess* currentProcess = actor->currentProcess;
    if (currentProcess)
    {
        // 0 = left, 1 = right
        TESForm* equippedFormRight = currentProcess->equippedObjects[1];
        if (equippedFormRight)
        {
            if (equippedFormRight->formID == std::get<uint32_t>(args[0]))
            {
                return true;
            }
        }
    }
    return false;
}

bool IsEquippedRightType(Actor* actor, std::variant<uint32_t, float>* args, uint32_t bmArgIsFloat)
{
    // IsEquippedRightType(GlobalVariable type)
    // Is the item equipped to the actor's right hand the specified type?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsEquippedRightType(%08x)", args[0]);
#endif
    float fArg0;
    if (!readGlobalVars(&fArg0, args, bmArgIsFloat, 1))
    {
        return false;
    }
    AIProcess* currentProcess = actor->currentProcess;
    TESForm* objEquipped = NULL;
    if (currentProcess)
    {
        // 0 = left, 1 = right
        objEquipped = currentProcess->equippedObjects[1];
    }
    int typeOfObjEquipped = getEquippedFormType(objEquipped);
    return (typeOfObjEquipped == fArg0);
}

bool IsEquippedRightHasKeyword(Actor* actor, std::variant<uint32_t, float>* args)
{
    // IsEquippedRightHasKeyword(Keyword keyword)
    // Does the item equipped to the actor's right hand have the specified keyword?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsEquippedRightHasKeyword(%08x)", args[0]);
#endif
    AIProcess* currentProcess = actor->currentProcess;
    if (!currentProcess) 
    {
        return false;
    }
    // 0 = left, 1 = right
    TESForm* equippedObj = currentProcess->equippedObjects[1];
    if (!equippedObj)
    {
        return false;
    }
    BGSKeyword* keyword = 
        (BGSKeyword*)RE::Game_GetForm(std::get<uint32_t>(args[0]));
    return keyword 
        && keyword->form.formType == Keyword
        && hasKeyword(equippedObj, keyword);
}

bool IsEquippedLeft(Actor* actor, std::variant<uint32_t, float>* args)
{
    // IsEquippedLeft(Form item)
    // Does the actor have the specified item equipped to his left hand?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsEquippedLeft(%08x)", args[0]);
#endif
    AIProcess* currentProcess = actor->currentProcess;
    if (currentProcess)
    {
        // 0 = left, 1 = right
        TESForm* equippedFormLeft = currentProcess->equippedObjects[0];
        if (equippedFormLeft)
        {
            if (equippedFormLeft->formID == std::get<uint32_t>(args[0]))
            {
                return true;
            }
        }
    }
    return false;
}

bool IsEquippedLeftType(Actor* actor, std::variant<uint32_t, float>* args, uint32_t bmArgIsFloat)
{
    // IsEquippedLeftType(GlobalVariable type)
    // Is the item equipped to the actor's left hand the specified type?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsEquippedLeftType(%08x)", args[0]);
#endif
    float fArg0;
    if (!readGlobalVars(&fArg0, args, bmArgIsFloat, 1))
    {
        return false;
    }
    AIProcess* currentProcess = actor->currentProcess;
    TESForm* objEquipped = NULL;
    if (currentProcess)
    {
        // 0 = left, 1 = right
        objEquipped = currentProcess->equippedObjects[0];
    }
    int typeOfObjEquipped = getEquippedFormType(objEquipped);
    return (typeOfObjEquipped == fArg0);
}

bool IsEquippedLeftHasKeyword(Actor* actor, std::variant<uint32_t, float>* args)
{
    // IsEquippedLeftHasKeyword(Keyword keyword)
    // Does the item equipped to the actor's left hand have the specified keyword?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsEquippedLeftHasKeyword(%08x)", args[0]);
#endif
    AIProcess* currentProcess = actor->currentProcess;
    if (!currentProcess)
    {
        return false;
    }
    // 0 = left, 1 = right
    TESForm* equippedObj = currentProcess->equippedObjects[0];
    if (!equippedObj)
    {
        return false;
    }
    BGSKeyword* keyword = 
        (BGSKeyword*)RE::Game_GetForm(std::get<uint32_t>(args[0]));
    return keyword
        && keyword->form.formType == Keyword
        && hasKeyword(equippedObj, keyword);
}

bool IsEquippedShout(Actor* actor, std::variant<uint32_t, float>* args)
{
    // IsEquippedShout(Form shout)
    // Does the actor currently have the specified shout?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsEquippedShout(%08x)", args[0]);
#endif
    TESForm* selectedPower = actor->selectedPower;
    return selectedPower 
        && selectedPower->formID == std::get<uint32_t>(args[0]);
}

bool IsWorn(Actor* actor, std::variant<uint32_t, float>* args, uint32_t bmArgIsFloat)
{
    // IsWorn(Form item)
    // Is the actor wearing the specified item?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsWorn(%08x)", args[0]);
#endif

    // Get the actor's inventory.
    ExtraContainerChanges* extraData = 
        (ExtraContainerChanges*)ExtraDataList_GetByTypeImpl
        (&actor->ref.extraData, ExtraDataType::kContainerChanges);
    if (!extraData)
    {
        return false;
    }
    InventoryChanges* changes = extraData->changes;
    if (!changes)
    {
        return false;
    }
    
    // Now iterate over it and see if we can find the specified item.
    for (BSTSimpleList_pInventoryEntryData* nodeEntryData = changes->entryList; 
         nodeEntryData != NULL; nodeEntryData = nodeEntryData->next)
    {
        InventoryEntryData* entryData = nodeEntryData->item;
        if (!entryData)
        {
            continue;
        }

        TESBoundObject* obj = entryData->object;
        if (obj && obj->tesObj.form.formID == std::get<uint32_t>(args[0]))
        {
            // We've found the specified item. Is it being worn?
            // Compare to commonlibsse's implementation in InventoryEntryData.cpp:
            //     bool InventoryEntryData::IsWorn() const
            BSTSimpleList_pExtraDataList* nodeExtraDataList = entryData->extraLists;
            while (nodeExtraDataList)
            {
                ExtraDataList* dataList = nodeExtraDataList->item;
                if (dataList 
                    && (ExtraDataList_HasType(dataList, ExtraDataType::kWorn)
                        || ExtraDataList_HasType(dataList, ExtraDataType::kWornLeft)))
                {
                    // Yes the specified item is being worn.
                    // Given it's in the actor's inventory, we assume that it is
                    // indeed them who is wearing it (and not a goblin who just happens
                    // to be stowing away in their back pack).
                    return true;
                }
                nodeExtraDataList = nodeExtraDataList->next;
            }
        }
    }
    return false;
}

bool IsWornHasKeyword(Actor* actor, std::variant<uint32_t, float>* args)
{
    // IsWornHasKeyword(Keyword keyword)
    // Is the actor wearing anything with the specified keyword?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsWornHasKeyword(%08x)", args[0]);
#endif

    // Get the actor's inventory.
    ExtraContainerChanges* extraData =
        (ExtraContainerChanges*)ExtraDataList_GetByTypeImpl
        (&actor->ref.extraData, ExtraDataType::kContainerChanges);
    if (!extraData)
    {
        return false;
    }
    InventoryChanges* changes = extraData->changes;
    if (!changes)
    {
        return false;
    }

    // Now iterate over it and see if we can find the specified item.
    for (BSTSimpleList_pInventoryEntryData* nodeEntryData = changes->entryList;
        nodeEntryData != NULL; nodeEntryData = nodeEntryData->next)
    {
        InventoryEntryData* entryData = nodeEntryData->item;
        if (!entryData)
        {
            continue;
        }

        if (hasKeywordBoundObj(entryData->object, args))
        {
            // We've found the specified item. Is it being worn?
            // Compare to commonlibsse's implementation in InventoryEntryData.cpp:
            //     bool InventoryEntryData::IsWorn() const
            BSTSimpleList_pExtraDataList* nodeExtraDataList = entryData->extraLists;
            while (nodeExtraDataList)
            {
                ExtraDataList* dataList = nodeExtraDataList->item;
                if (dataList
                    && (ExtraDataList_HasType(dataList, ExtraDataType::kWorn)
                        || ExtraDataList_HasType(dataList, ExtraDataType::kWornLeft)))
                {
                    // Yes the specified item is being worn.
                    // Given it's in the actor's inventory, we assume that it is
                    // indeed them who is wearing it (and not a goblin who just happens
                    // to be stowing away in their back pack).
                    return true;
                }
                nodeExtraDataList = nodeExtraDataList->next;
            }
        }
    }
    return false;
}

bool IsFemale(Actor* actor)
{
    // IsFemale()
    // Is the actor female?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsFemale()");
#endif
    TESActorBase* actorBase = (TESActorBase*)actor->ref.baseForm;
    return actorBase
        && (uint8_t)RE::ActorBase_GetSex(actorBase);
}

bool Is_Child(Actor* actor)
{
    // IsChild()
    // Is the actor a child?
    // Need to name this function Is_Child rather than IsChild to avoid
    // clobbering Windows IsChild function defined in WinUser.h
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsChild()");
#endif
    return (*(_TESForm_IsFormTypeChild)(actor->ref.form.pVft + 0x2F0))(actor);
}

bool IsPlayerTeammate(Actor* actor)
{
    // IsPlayerTeammate()
    // Is the actor currently a teammate of the player?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsPlayerTeammate()");
#endif
    return (actor->boolBits >> 26) & 1;
}

bool IsInInterior(Actor* actor)
{
    // IsInInterior()
    // Is the actor in an interior cell ?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsInInterior()");
#endif
    TESObjectCELL* parentCell = actor->ref.parentCell;
    return parentCell 
        && (parentCell->cellFlags & TESObjectCELLFlag::kIsInteriorCell) != 0;
}

bool IsInFaction(Actor* actor, std::variant<uint32_t, float>* args)
{
    // IsInFaction(Faction faction)
    // Is the actor in the specified faction?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsInFaction(%08x)", args[0]);
#endif
    TESForm* form = RE::Game_GetForm(std::get<uint32_t>(args[0]));
    return form
        && form->formType == FormType::Faction
        && (*(_Actor_IsInFaction)(actor->ref.form.pVft + 0x7C8))(actor, form);
}

bool HasKeyword(Actor* actor, std::variant<uint32_t, float>* args)
{
    // HasKeyword(Keyword keyword)
    // Does the actor have the specified keyword?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("HasKeyword(%08x)", args[0]);
#endif
    TESForm* form = RE::Game_GetForm(std::get<uint32_t>(args[0]));
    return form
        && form->formType == FormType::Keyword
        && (*(_Actor_HasKeyword)(actor->ref.form.pVft + 0x240))(actor, form);
}

bool HasMagicEffect(Actor* actor, std::variant<uint32_t, float>* args)
{
    // HasMagicEffect(MagicEffect magiceffect)
    // Is the actor currently being affected by the given Magic Effect?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("HasMagicEffect(%08x)", args[0]);
#endif
    TESForm* form = RE::Game_GetForm(std::get<uint32_t>(args[0]));
    return form
        && form->formType == FormType::MagicEffect
        && RE::MagicTarget_HasMagicEffect(&actor->magicTarget, form);
}

bool HasMagicEffectWithKeyword(Actor* actor, std::variant<uint32_t, float>* args)
{
    // HasMagicEffectWithKeyword(Keyword keyword)
    // Is the actor currently being affected by a Magic Effect with the given Keyword?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("HasMagicEffectWithKeyword(% 08x)", args[0]);
#endif
    TESForm* form = RE::Game_GetForm(std::get<uint32_t>(args[0]));
    return form 
        && form->formType == FormType::Keyword
        && RE::MagicTarget_HasMagicEffectWithKeyword
           (&actor->magicTarget, form, 0);
}

bool HasPerk(Actor* actor, std::variant<uint32_t, float>* args)
{
    // HasPerk(Perk perk)
    // Does the actor have the given Perk ?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("HasPerk(%08x)", args[0]);
#endif
    TESForm* form = RE::Game_GetForm(std::get<uint32_t>(args[0]));
    return form
        && form->formType == FormType::Perk
        && RE::Actor_HasPerk(actor, form);
}

bool HasSpell(Actor* actor, std::variant<uint32_t, float>* args)
{
    // HasSpell(Form spell)
    // Does the actor have the given Spell or Shout?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("HasSpell(%08x)", args[0]);
#endif
    TESForm* form = RE::Game_GetForm(std::get<uint32_t>(args[0]));
    if (!form) {
        return false;
    }
    uint8_t formType = form->formType;
    if (formType == FormType::Spell)
    {
        return RE::Actor_HasSpell(actor, form);
    }
    if (formType == FormType::Shout)
    {
        return RE::Actor_HasShout(actor, form);
    }
    return false;
}

bool IsActorValueEqualTo(Actor* actor, std::variant<uint32_t, float>* args, uint32_t bmArgIsFloat)
{
    // IsActorValueEqualTo(GlobalVariable id, GlobalVariable value)
    // Is the ActorValue of the specified ID equal to the value?
    // Temporarily disabled because it's crashing.
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsActorValueEqualTo(%08x, %08x)", args[0], args[1]);
#endif
    float fArgs[2];
    if (!readGlobalVars(fArgs, args, bmArgIsFloat, 2))
    {
        return false;
    }
    float fActorVal = 
        (*(_ActorValueOwner_GetActorValue)(actor->actorValueOwner.pVFT + 8))
        (&actor->actorValueOwner, (uint32_t)fArgs[0]);
    return (fActorVal == fArgs[1]);
}

bool IsActorValueLessThan(Actor* actor, std::variant<uint32_t, float>* args, uint32_t bmArgIsFloat)
{
    // IsActorValueLessThan(GlobalVariable id, GlobalVariable value)
    // Is the ActorValue of the specified ID less than the value?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsActorValueLessThan(%08x, %08x)", args[0], args[1]);
#endif
    float fArgs[2];
    if (!readGlobalVars(fArgs, args, bmArgIsFloat, 2))
    {
        return false;
    }
    float fActorVal =
        (*(_ActorValueOwner_GetActorValue)(actor->actorValueOwner.pVFT + 8))
        (&actor->actorValueOwner, (uint32_t)fArgs[0]);
    return (fActorVal < fArgs[1]);
}

bool IsActorValueBaseEqualTo(Actor* actor, std::variant<uint32_t, float>* args, uint32_t bmArgIsFloat)
{
    // IsActorValueBaseEqualTo(GlobalVariable id, GlobalVariable value)
    // Is the base ActorValue of the specified ID equal to the value?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsActorValueBaseEqualTo(%08x, %08x)", args[0], args[1]);
#endif
    float fArgs[2];
    if (!readGlobalVars(fArgs, args, bmArgIsFloat, 2))
    {
        return false;
    }
    float fActorValBase = 
        (*(_ActorValueOwner_GetBaseActorValue)(actor->actorValueOwner.pVFT + 0x18))
        (&actor->actorValueOwner, fArgs[0]);
    return (fActorValBase == fArgs[1]);
}

bool IsActorValueBaseLessThan(Actor* actor, std::variant<uint32_t, float>* args, uint32_t bmArgIsFloat)
{
    // IsActorValueBaseLessThan(GlobalVariable id, GlobalVariable value)
    // Is the base ActorValue of the specified ID less than the value?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsActorValueBaseLessThan(%08x, %08x)", args[0], args[1]);
#endif
    float fArgs[2];
    if (!readGlobalVars(fArgs, args, bmArgIsFloat, 2))
    {
        return false;
    }
    float fActorValBase =
        (*(_ActorValueOwner_GetBaseActorValue)(actor->actorValueOwner.pVFT + 0x18))
        (&actor->actorValueOwner, fArgs[0]);
    return (fActorValBase < fArgs[1]);
}

bool IsActorValueMaxEqualTo(Actor* actor, std::variant<uint32_t, float>* args, uint32_t bmArgIsFloat)
{
    // IsActorValueMaxEqualTo(GlobalVariable id, GlobalVariable value)
    // Is the max ActorValue of the specified ID equal to the value?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsActorValueMaxEqualTo(%08x, %08x)", args[0], args[1]);
#endif
    float fArgs[2];
    if (!readGlobalVars(fArgs, args, bmArgIsFloat, 2))
    {
        return false;
    }
    float fActorValMax =
        (*(_ActorValueOwner_GetPermanentActorValue)(actor->actorValueOwner.pVFT + 0x10))
        (&actor->actorValueOwner, fArgs[0]);
    return (fActorValMax == fArgs[1]);
}

bool IsActorValueMaxLessThan(Actor* actor, std::variant<uint32_t, float>* args, uint32_t bmArgIsFloat)
{
    // IsActorValueMaxLessThan(GlobalVariable id, GlobalVariable value)
    // Is the max ActorValue of the specified ID less than the value?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsActorValueMaxLessThan(%08x, %08x)", args[0], args[1]);
#endif
    float fArgs[2];
    if (!readGlobalVars(fArgs, args, bmArgIsFloat, 2))
    {
        return false;
    }
    float fActorValMax =
        (*(_ActorValueOwner_GetPermanentActorValue)(actor->actorValueOwner.pVFT + 0x10))
        (&actor->actorValueOwner, fArgs[0]);
    return (fActorValMax < fArgs[1]);
}

bool IsActorValuePercentageEqualTo(Actor* actor, std::variant<uint32_t, float>* args, uint32_t bmArgIsFloat)
{
    // IsActorValuePercentageEqualTo(GlobalVariable id, GlobalVariable value)
    // Is the percentage ActorValue of the specified ID equal to the value?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsActorValuePercentageEqualTo(%08x, %08x)", args[0], args[1]);
#endif
    float fArgs[2];
    if (!readGlobalVars(fArgs, args, bmArgIsFloat, 2))
    {
        return false;
    }
    float fActorValPct = getActorValPct(actor, (uint32_t)fArgs[0]);
    return (fActorValPct == fArgs[1]);
}

bool IsActorValuePercentageLessThan(Actor* actor, std::variant<uint32_t, float>* args, uint32_t bmArgIsFloat)
{
    // IsActorValuePercentageLessThan(GlobalVariable id, GlobalVariable value)
    // Is the percentage ActorValue of the specified ID less than the value?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsActorValuePercentageLessThan(%08x, %08x)", args[0], args[1]);
#endif
    float fArgs[2];
    if (!readGlobalVars(fArgs, args, bmArgIsFloat, 2))
    {
        return false;
    }
    float fActorValPct = getActorValPct(actor, (uint32_t)fArgs[0]);
    return (fActorValPct < fArgs[1]);
}

bool IsLevelLessThan(Actor* actor, std::variant<uint32_t, float>* args, uint32_t bmArgIsFloat)
{
    // IsLevelLessThan(GlobalVariable level)
    // Is the actor's current level less than the specified level?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsLevelLessThan(%08x)", args[0]);
#endif
    float fArg0;
    if (!readGlobalVars(&fArg0, args, bmArgIsFloat, 1))
    {
        return false;
    }
    uint16_t actorLevel = RE::Actor_GetLevel(actor);
    return actorLevel < fArg0;
}

bool IsActorBase(Actor* actor, std::variant<uint32_t, float>* args)
{
    // IsActorBase(ActorBase actorbase)
    // Is the actorbase for the actor the specified actorbase?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsActorBase(%08x)", args[0]);
#endif
    TESForm* baseForm = actor->ref.baseForm;
    return baseForm 
        && baseForm->formID == std::get<uint32_t>(args[0]);
}

bool IsRace(Actor* actor, std::variant<uint32_t, float>* args)
{
    // IsRace(Race race)
    // Is the actor's race the specified race?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsRace(%08x)", args[0]);
#endif
    TESNPC* npc = (TESNPC*)actor->ref.baseForm;
    if (!npc)
    {
        return false;
    }
    TESRace* race = npc->raceForm.race;
    return race 
        && race->form.formID == std::get<uint32_t>(args[0]);
}

bool CurrentWeather(Actor* actor, std::variant<uint32_t, float>* args)
{
    // CurrentWeather(Weather weather)
    // Is the current weather the specified weather?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("CurrentWeather(%08x)", args[0]);
#endif
    Sky* theSky = RE::Sky_GetSingleton();
    if (theSky)
    {
        TESWeather* curWeather = theSky->currentWeather;
        if (curWeather)
        {
            if (curWeather->form.formID == std::get<uint32_t>(args[0]))
            {
                return true;
            }
        }
    }
    return false;
}

bool CurrentGameTimeLessThan(Actor* actor, std::variant<uint32_t, float>* args, uint32_t bmArgIsFloat)
{
    // CurrentGameTimeLessThan(GlobalVariable time)
    // Is the current game time less than the specified time?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("CurrentGameTimeLessThan(% 08x)", args[0]);
#endif
    float fArg0;
    if (!readGlobalVars(&fArg0, args, bmArgIsFloat, 1))
    {
        return false;
    }

    float gameDaysPassed = 
        RE::Calendar_GetCurrentGameTime(*RE::g_theCalendar);

    // This gives the fractional part of the game time, i.e. proportion of the current
    // day that has passed. We then multiply it by the number of game hours per day
    // to get the current time of the day:
    float f = 0.0;
    float pctCurDayPassed = modff(gameDaysPassed, &f);
    return fArg0 > (float)(pctCurDayPassed * *RE::g_gameHoursPerGameDay);
}

bool ValueEqualTo(Actor* actor, std::variant<uint32_t, float>* args, uint32_t bmArgIsFloat)
{
    // ValueEqualTo(GlobalVariable value1, GlobalVariable value2)
    // Is the value1 equal to the value2?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("ValueEqualTo(%08x, %08x)", args[0], args[1]);
#endif
    float fArgs[2];
    if (!readGlobalVars(fArgs, args, bmArgIsFloat, 2))
    {
        return false;
    }
    return (fArgs[0] == fArgs[1]);
}

bool ValueLessThan(Actor* actor, std::variant<uint32_t, float>* args, uint32_t bmArgIsFloat)
{
    // ValueLessThan(GlobalVariable value1, GlobalVariable value2)
    // Is the value1 less than the value2?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("ValueLessThan(%08x, %08x)", args[0], args[1]);
#endif
    float fArgs[2];
    if (!readGlobalVars(fArgs, args, bmArgIsFloat, 2))
    {
        return false;
    }
    return (fArgs[0] < fArgs[1]);
}

bool Random(Actor* actor, std::variant<uint32_t, float>* args, uint32_t bmArgIsFloat)
{
    // Random(GlobalVariable percentage)
    // The probability of the specified percentage (from 0 to 1).
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("Random(%08x)", args[0]);
#endif
    float fArg0;
    if (!readGlobalVars(&fArg0, args, bmArgIsFloat, 1))
    {
        return false;
    }

    // Some sanity checks before we proceed...
    if (fArg0 < 0.0 || fArg0 > 1.0)
    {
        // Invalid values. Just return false.
        return false;
    } 
    else if (fArg0 == 1.0)
    {
        // Just in case, given our random number generator
        // below will never return 1. Although calling this 
        // function with a pct of 1, although valid would be
        // silly as then we have certainty and the condition is
        // thus completely redundant.
        return true;
    }

    // Ok, continue.
    // Generate a random number in the range [0, 1).
    std::random_device r;
    std::mt19937 gen(r());
    float prob = std::generate_canonical<float, 10>(gen);
      
    return (fArg0 > prob);
}

bool IsUnique(Actor* actor)
{
    // IsUnique()
    // Is the actor flagged as unique?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsUnique()");
#endif
    TESNPC* npc = (TESNPC*)actor->ref.baseForm;
    return npc && 
        (npc->actorBase.actorData.actorData.actorBaseFlags &
         ACTOR_BASE_DATA::Flag::kUnique) != 0;
}

bool IsClass(Actor* actor, std::variant<uint32_t, float>* args)
{
    // IsClass(Class class)
    // Is the actor's class the specified class?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsClass(%08x)", args[0]);
#endif
    TESNPC* form = (TESNPC*)actor->ref.baseForm;
    if (form)
    {
        TESClass* npcClass = form->npcClass;
        if (npcClass)
        {
            if (npcClass->form.formID == std::get<uint32_t>(args[0]))
            {
                return true;
            }
        }
    }
    return false;
}

bool IsCombatStyle(Actor* actor, std::variant<uint32_t, float>* args)
{
    // IsCombatStyle(CombatStyle combatStyle)
    // Is the actor's CombatStyle the specified CombatStyle?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsCombatStyle(%08x)", args[0]);
#endif
    TESNPC* npc = (TESNPC*)actor->ref.baseForm;
    if (npc)
    {
        TESCombatStyle* combatStyle = npc->combatStyle;
        if (combatStyle)
        {
            if (combatStyle->form.formID == std::get<uint32_t>(args[0]))
            {
                return true;
            }
        }
    }
    return false;
}

bool IsVoiceType(Actor* actor, std::variant<uint32_t, float>* args)
{
    // IsVoiceType(VoiceType voiceType)
    // Is the actor's VoiceType the specified VoiceType?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsVoiceType(%08x)", args[0]);
#endif
    TESNPC* npc = (TESNPC*)actor->ref.baseForm;
    if (npc)
    {
        BGSVoiceType* voiceType = npc->actorBase.actorData.voiceType;
        if (voiceType)
        {
            if (voiceType->form.formID == std::get<uint32_t>(args[0]))
            {
                return true;
            }
        }
    }
    return false;
}

bool IsAttacking(Actor* actor)
{
    // IsAttacking()
    // Is the actor currently attacking?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsAttacking()");
#endif
    return (actor->actorState.actorState1.meleeAttackState != ATTACK_STATE_ENUM::kNone);
}

bool IsRunning(Actor* actor)
{
    // IsRunning()
    // Is the actor currently running?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsRunning()");
#endif
    return RE::Actor_IsRunning(actor);
}

bool IsSneaking(Actor* actor)
{
    // IsSneaking()
    // Is the actor currently sneaking ?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsSneaking()");
#endif
    return (actor->actorState.actorState1.sneaking
        && !RE::Actor_IsOnMount(actor));
}

bool IsSprinting(Actor* actor)
{
    // IsSprinting()
    // Is the actor currently sprinting?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsSprinting()");
#endif
    return actor->actorState.actorState1.sprinting;
}

bool IsInAir(Actor* actor)
{
    // IsInAir()
    // Is the actor in the air?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsInAir()");
#endif
    return RE::Actor_IsInAir(actor);
}

bool IsInCombat(Actor* actor)
{
    // IsInCombat()
    // Is the actor in combat?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsInCombat()");
#endif
    return (*(_Actor_IsInCombat)(actor->ref.form.pVft + 0x718))(actor);
}

bool IsWeaponDrawn(Actor* actor)
{
    // IsWeaponDrawn()
    // Does the actor have his equipped weapon and/or magic spell drawn?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsWeaponDrawn()");
#endif
    return actor->actorState.actorState2.weaponState >= WEAPON_STATE::kDrawn;
}

bool IsInLocation(Actor* actor, std::variant<uint32_t, float>* args)
{
    // IsInLocation(Location location)
    // Is the actor in the specified location or a child of that location?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsInLocation(%08x)", args[0]);
#endif
    BGSLocation* curLoc;
    TESForm* frm;
    TESForm* frmBGSLocation = RE::Game_GetForm(std::get<uint32_t>(args[0]));
    if (frmBGSLocation
        && frmBGSLocation->formType == FormType::Location
        && (curLoc = (BGSLocation*)RE::ObjectReference_GetCurrentLocation((TESObjectREFR*)actor), 
            (frm = &curLoc->form) != NULL))
    {
        while (true)
        {
            curLoc = curLoc->parentLoc;
            if (curLoc == (BGSLocation*)frmBGSLocation)
            {
                break;
            }
            if (!curLoc)
            {
                if (frm != frmBGSLocation)
                {
                    return false;
                }
                break;
            }
        }
        return true;
    }
    return false;
}

bool HasRefType(Actor* actor, std::variant<uint32_t, float>* args)
{
    // HasRefType(LocationRefType refType)
    // Does the actor have the specified LocationRefType attached?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("HasRefType(%08x)", args[0]);
#endif
    TESForm* frmBGSLocationRefType = RE::Game_GetForm(std::get<uint32_t>(args[0]));
    if (!frmBGSLocationRefType || frmBGSLocationRefType->formType != LocationRefType)
    {
        return false;
    }
    ExtraLocationRefType* locRef = 
        (ExtraLocationRefType*)ExtraDataList_GetByTypeImpl(&actor->ref.extraData, ExtraDataType::kLocationRefType);
    return locRef 
        && locRef->locRefType == (BGSLocationRefType*)frmBGSLocationRefType;
}

bool IsParentCell(Actor* actor, std::variant<uint32_t, float>* args)
{
    // IsParentCell(Cell cell)
    // Is the actor in the specified cell?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsParentCell(%08x)", args[0]);
#endif
    TESObjectCELL* parentCell = actor->ref.parentCell;
    return parentCell 
        && parentCell->form.formID == std::get<uint32_t>(args[0]);
}

bool IsWorldSpace(Actor* actor, std::variant<uint32_t, float>* args)
{
    // IsWorldSpace(WorldSpace worldSpace)
    // Is the actor in the specified WorldSpace?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsWorldSpace(%08x)", args[0]);
#endif
    TESWorldSpace* worldspace = 
        RE::TESObjectREFR_GetWorldSpace(&actor->ref);
    return worldspace 
        && worldspace->form.formID == std::get<uint32_t>(args[0]);
}

bool IsFactionRankEqualTo(Actor* actor, std::variant<uint32_t, float>* args, uint32_t bmArgIsFloat)
{
    // IsFactionRankEqualTo(GlobalVariable rank, Faction faction)
    // Is the actor's rank in the specified faction equal to the specified rank?
    // 
    // From the DAR documentation:
    //   "The actor's rank in the specified faction:
    //      => -2 if the Actor is not in the faction.
    //      => -1 if the Actor is in the faction, with a rank set to -1. 
    //            (By convention, this means they may eventually become a member 
    //             of this faction.)
    //      => A non-negative number equal to the actor's rank in the faction."
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsFactionRankEqualTo(%08x, %08x)", args[0], args[1]);
#endif
    float fArg0;
    if (!readGlobalVars(&fArg0, args, bmArgIsFloat, 1))
    {
        return false;
    }

    TESForm* frmTESFaction = RE::Game_GetForm(std::get<uint32_t>(args[1]));
    if (!frmTESFaction || frmTESFaction->formType != FormType::Faction)
    {
        return false;
    }
  
    bool isPlayer = (actor == *(Actor**)RE::g_thePlayer);
    float actorRank = 
        (float)RE::Actor_GetFactionRank
        (actor, (TESFaction*)frmTESFaction, isPlayer);
    return (actorRank == fArg0);
}

bool IsFactionRankLessThan(Actor* actor, std::variant<uint32_t, float>* args, uint32_t bmArgIsFloat)
{
    // IsFactionRankLessThan(GlobalVariable rank, Faction faction)
    // Is the actor's rank in the specified faction less than the specified rank?
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsFactionRankLessThan(%08x, %08x)", args[0], args[1]);
#endif
    float fArg0;
    if (!readGlobalVars(&fArg0, args, bmArgIsFloat, 1))
    {
        return false;
    }

    TESForm* frmTESFaction = RE::Game_GetForm(std::get<uint32_t>(args[1]));
    if (!frmTESFaction || frmTESFaction->formType != FormType::Faction)
    {
        return false;
    }

    bool isPlayer = (actor == *(Actor**)RE::g_thePlayer);
    float actorRank =
        (float)RE::Actor_GetFactionRank
        (actor, (TESFaction*)frmTESFaction, isPlayer);
    return (actorRank < fArg0);
}

bool IsMovementDirection(Actor* actor, std::variant<uint32_t, float>* args, uint32_t bmArgIsFloat)
{
    // IsMovementDirection(GlobalVariable direction)
    // Is the actor moving in the specified direction?
    //
    // From the DAR documentation:
    //   "Movement direction:
    //      => 0 = Standing Still
    //      => 1 = Forward
    //      => 2 = Right
    //      => 3 = Back
    //      => 4 = Left"
#ifdef DEBUG_TRACE_CONDITIONS
    _MESSAGE("IsMovementDirection(%08x)", args[0]);
#endif
    float fArg0;
    if (!readGlobalVars(&fArg0, args, bmArgIsFloat, 1))
    {
        return false;
    }

    float actorMoveDir = 0.0;
    if (RE::Actor_IsMoving(actor))
    {
        double move_angle = RE::Actor_GetMoveDirRelToFacing(actor);
        // Ensure movement angle (in radians) is within the range [0, 2*PI]
        for (; move_angle < 0.0; move_angle += TWO_PI);
        for (; move_angle > TWO_PI; move_angle -= TWO_PI);

        // Get nearest boundary of circular quadrant containing this angle.
        // Ex. 1: If movement angle is 0.4 * PI (i.e. in 1st quadrant, closest
        //        to 0.5*PI boundary), first calc evaluates to:
        //         y = ((0.4*PI / 2*PI) + 1/8) * 4.0 / 4.0
        //           = [(0.2 + 0.125) * 4.0] / 4.0
        //           = fmodf(1.3, 4.0) = 1.3 - (1.3 / 4.0 = 0.325 => 0) * 4 = 1.3
        //        Second calc then evaluates as 1.0 + 1.0 = 2 (Right)
        // Ex. 2. For movement angle 1.1 * PI (i.e. in 3rd quadrant, closest
        //        to PI boundary), calcs evaluate to:
        //        [1]:  y = ((1.1*PI / 2*PI) + 1/8) * 4.0 / 4.0 
        //                = fmodf(2.7, 4.0) = 2.7 - (2.7 / 4.0 = 0.675 => 0) * 4 = 2.7
        //        [2]:  2.0 + 1.0 = 3 (Back).
        // As expected.
        float y = fmodf((float)((move_angle / TWO_PI) + 0.125) * 4.0, 4.0);
        actorMoveDir = floorf(y) + 1.0;
    }
    return (actorMoveDir == fArg0);
}

// The DAR defined actor state functions
// These all test an actor for a specific state and return TRUE or FALSE.
std::vector<std::pair<std::string, FuncInfo>> v
{
    // ==============================================================================================
    // Function Name                           {  address of function,      nArgs, argTypeMask  }
    // ----------------------------------------------------------------------------------------------
    { "IsEquippedRight",                FuncInfo{ &IsEquippedRight,                1, 0 } },  // 0
    { "IsEquippedRightType",            FuncInfo{ &IsEquippedRightType,            1, 1 } },  // 1
    { "IsEquippedRightHasKeyword",      FuncInfo{ &IsEquippedRightHasKeyword,      1, 0 } },  // 2
    { "IsEquippedLeft",                 FuncInfo{ &IsEquippedLeft,                 1, 0 } },  // 3
    { "IsEquippedLeftType",             FuncInfo{ &IsEquippedLeftType,             1, 1 } },  // 4
    { "IsEquippedLeftHasKeyword",       FuncInfo{ &IsEquippedLeftHasKeyword,       1, 0 } },  // 5
    { "IsEquippedShout",                FuncInfo{ &IsEquippedShout,                1, 0 } },  // 6
    { "IsWorn",                         FuncInfo{ &IsWorn,                         1, 0 } },  // 7
    { "IsWornHasKeyword",               FuncInfo{ &IsWornHasKeyword,               1, 0 } },  // 8
    { "IsFemale",                       FuncInfo{ &IsFemale,                       0, 0 } },  // 9
    { "IsChild",                        FuncInfo{ &Is_Child,                       0, 0 } },  // 10
    { "IsPlayerTeammate",               FuncInfo{ &IsPlayerTeammate,               0, 0 } },  // 11
    { "IsInInterior",                   FuncInfo{ &IsInInterior,                   0, 0 } },  // 12
    { "IsInFaction",                    FuncInfo{ &IsInFaction,                    1, 0 } },  // 13
    { "HasKeyword",                     FuncInfo{ &HasKeyword,                     1, 0 } },  // 14
    { "HasMagicEffect",                 FuncInfo{ &HasMagicEffect,                 1, 0 } },  // 15
    { "HasMagicEffectWithKeyword",      FuncInfo{ &HasMagicEffectWithKeyword,      1, 0 } },  // 16
    { "HasPerk",                        FuncInfo{ &HasPerk,                        1, 0 } },  // 17
    { "HasSpell",                       FuncInfo{ &HasSpell,                       1, 0 } },  // 18
    { "IsActorValueEqualTo",            FuncInfo{ &IsActorValueEqualTo,            2, 3 } },  // 19
    { "IsActorValueLessThan",           FuncInfo{ &IsActorValueLessThan,           2, 3 } },  // 20
    { "IsActorValueBaseEqualTo",        FuncInfo{ &IsActorValueBaseEqualTo,        2, 3 } },  // 21
    { "IsActorValueBaseLessThan",       FuncInfo{ &IsActorValueBaseLessThan,       2, 3 } },  // 22
    { "IsActorValueMaxEqualTo",         FuncInfo{ &IsActorValueMaxEqualTo,         2, 3 } },  // 23
    { "IsActorValueMaxLessThan",        FuncInfo{ &IsActorValueMaxLessThan,        2, 3 } },  // 24
    { "IsActorValuePercentageEqualTo",  FuncInfo{ &IsActorValuePercentageEqualTo,  2, 3 } },  // 25
    { "IsActorValuePercentageLessThan", FuncInfo{ &IsActorValuePercentageLessThan, 2, 3 } },  // 26
    { "IsLevelLessThan",                FuncInfo{ &IsLevelLessThan,                1, 1 } },  // 27
    { "IsActorBase",                    FuncInfo{ &IsActorBase,                    1, 0 } },  // 28
    { "IsRace",                         FuncInfo{ &IsRace,                         1, 0 } },  // 29
    { "CurrentWeather",                 FuncInfo{ &CurrentWeather,                 1, 0 } },  // 30
    { "CurrentGameTimeLessThan",        FuncInfo{ &CurrentGameTimeLessThan,        1, 1 } },  // 31
    { "ValueEqualTo",                   FuncInfo{ &ValueEqualTo,                   2, 3 } },  // 32
    { "ValueLessThan",                  FuncInfo{ &ValueLessThan,                  2, 3 } },  // 33
    { "Random",                         FuncInfo{ &Random,                         1, 1 } },  // 34
    { "IsUnique",                       FuncInfo{ &IsUnique,                       0, 0 } },  // 35
    { "IsClass",                        FuncInfo{ &IsClass,                        1, 0 } },  // 36
    { "IsCombatStyle",                  FuncInfo{ &IsCombatStyle,                  1, 0 } },  // 37
    { "IsVoiceType",                    FuncInfo{ &IsVoiceType,                    1, 0 } },  // 38
    { "IsAttacking",                    FuncInfo{ &IsAttacking,                    0, 0 } },  // 39
    { "IsRunning",                      FuncInfo{ &IsRunning,                      0, 0 } },  // 40
    { "IsSneaking",                     FuncInfo{ &IsSneaking,                     0, 0 } },  // 41
    { "IsSprinting",                    FuncInfo{ &IsSprinting,                    0, 0 } },  // 42
    { "IsInAir",                        FuncInfo{ &IsInAir,                        0, 0 } },  // 43
    { "IsInCombat",                     FuncInfo{ &IsInCombat,                     0, 0 } },  // 44
    { "IsWeaponDrawn",                  FuncInfo{ &IsWeaponDrawn,                  0, 0 } },  // 45
    { "IsInLocation",                   FuncInfo{ &IsInLocation,                   1, 0 } },  // 46
    { "HasRefType",                     FuncInfo{ &HasRefType,                     1, 0 } },  // 47
    { "IsParentCell",                   FuncInfo{ &IsParentCell,                   1, 0 } },  // 48
    { "IsWorldSpace",                   FuncInfo{ &IsWorldSpace,                   1, 0 } },  // 49
    { "IsFactionRankEqualTo",           FuncInfo{ &IsFactionRankEqualTo,           2, 1 } },  // 50
    { "IsFactionRankLessThan",          FuncInfo{ &IsFactionRankLessThan,          2, 1 } },  // 51
    { "IsMovementDirection",            FuncInfo{ &IsMovementDirection,            1, 1 } },  // 52
    // ==============================================================================================
};
std::unordered_map<std::string, FuncInfo> g_DARConditionFuncs(v.begin(), v.end());