// Copyright (C) 2015 Lucas Pires Camargo
// 
// This file is part of neiasound - Qt-style OpenAL wrapper for games.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// 
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE FREEBSD PROJECT ``AS IS'' AND ANY EXPRESS
// OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
// NO EVENT SHALL THE FREEBSD PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#include "nefxhelper.h"
#include <QString>

bool nEfxHelper::sm_initialized = false;

LPALGENAUXILIARYEFFECTSLOTS nEfxHelper::alGenAuxiliaryEffectSlots = 0;
LPALISAUXILIARYEFFECTSLOT nEfxHelper::alIsAuxiliaryEffectSlot = 0;
LPALDELETEAUXILIARYEFFECTSLOTS nEfxHelper::alDeleteAuxiliaryEffectSlots = 0;
LPALAUXILIARYEFFECTSLOTI nEfxHelper::alAuxiliaryEffectSloti = 0;
LPALAUXILIARYEFFECTSLOTIV nEfxHelper::alAuxiliaryEffectSlotiv = 0;
LPALAUXILIARYEFFECTSLOTF nEfxHelper::alAuxiliaryEffectSlotf = 0;
LPALAUXILIARYEFFECTSLOTFV nEfxHelper::alAuxiliaryEffectSlotfv = 0;
LPALGETAUXILIARYEFFECTSLOTI nEfxHelper::alGetAuxiliaryEffectSloti = 0;
LPALGETAUXILIARYEFFECTSLOTIV nEfxHelper::alGetAuxiliaryEffectSlotiv = 0;
LPALGETAUXILIARYEFFECTSLOTF nEfxHelper::alGetAuxiliaryEffectSlotf = 0;
LPALGETAUXILIARYEFFECTSLOTFV nEfxHelper::alGetAuxiliaryEffectSlotfv = 0;

LPALGENEFFECTS nEfxHelper::alGenEffects = 0;
LPALISEFFECT nEfxHelper::alIsEffect = 0;
LPALDELETEEFFECTS nEfxHelper::alDeleteEffects = 0;
LPALEFFECTI nEfxHelper::alEffecti = 0;
LPALEFFECTIV nEfxHelper::alEffectiv = 0;
LPALEFFECTF nEfxHelper::alEffectf = 0;
LPALEFFECTFV nEfxHelper::alEffectfv = 0;
LPALGETEFFECTI nEfxHelper::alGetEffecti = 0;
LPALGETEFFECTIV nEfxHelper::alGetEffectiv = 0;
LPALGETEFFECTF nEfxHelper::alGetEffectf = 0;
LPALGETEFFECTFV nEfxHelper::alGetEffectfv = 0;

LPALGENFILTERS nEfxHelper::alGenFilters = 0;
LPALISFILTER nEfxHelper::alIsFilter = 0;
LPALDELETEFILTERS nEfxHelper::alDeleteFilters = 0;
LPALFILTERI nEfxHelper::alFilteri = 0;
LPALFILTERIV nEfxHelper::alFilteriv = 0;
LPALFILTERF nEfxHelper::alFilterf = 0;
LPALFILTERFV nEfxHelper::alFilterfv = 0;
LPALGETFILTERI nEfxHelper::alGetFilteri = 0;
LPALGETFILTERIV nEfxHelper::alGetFilteriv = 0;
LPALGETFILTERF nEfxHelper::alGetFilterf = 0;
LPALGETFILTERFV nEfxHelper::alGetFilterfv = 0;

nEfxHelper::nEfxHelper()
{
}

bool nEfxHelper::initialize(ALCdevice * device)
{
    if(sm_initialized) return true;

    if(alcIsExtensionPresent(device, "ALC_EXT_EFX")==AL_FALSE)
        return false;


    alGenAuxiliaryEffectSlots = (LPALGENAUXILIARYEFFECTSLOTS) alGetProcAddress("alGenAuxiliaryEffectSlots");
    alIsAuxiliaryEffectSlot = (LPALISAUXILIARYEFFECTSLOT) alGetProcAddress("alIsAuxiliaryEffectSlot");
    alDeleteAuxiliaryEffectSlots = (LPALDELETEAUXILIARYEFFECTSLOTS) alGetProcAddress("alDeleteAuxiliaryEffectSlots");
    alAuxiliaryEffectSloti = (LPALAUXILIARYEFFECTSLOTI) alGetProcAddress("alAuxiliaryEffectSloti");
    alAuxiliaryEffectSlotiv = (LPALAUXILIARYEFFECTSLOTIV) alGetProcAddress("alAuxiliaryEffectSlotiv");
    alAuxiliaryEffectSlotf = (LPALAUXILIARYEFFECTSLOTF) alGetProcAddress("alAuxiliaryEffectSlotf");
    alAuxiliaryEffectSlotfv = (LPALAUXILIARYEFFECTSLOTFV) alGetProcAddress("alAuxiliaryEffectSlotfv");
    alGetAuxiliaryEffectSloti = (LPALGETAUXILIARYEFFECTSLOTI) alGetProcAddress("alGetAuxiliaryEffectSloti");
    alGetAuxiliaryEffectSlotiv = (LPALGETAUXILIARYEFFECTSLOTIV) alGetProcAddress("alGetAuxiliaryEffectSlotiv");
    alGetAuxiliaryEffectSlotf = (LPALGETAUXILIARYEFFECTSLOTF) alGetProcAddress("alGetAuxiliaryEffectSlotf");
    alGetAuxiliaryEffectSlotfv = (LPALGETAUXILIARYEFFECTSLOTFV) alGetProcAddress("alGetAuxiliaryEffectSlotfv");

    bool alAuxiliaryEffectSlotCheck = (alGenAuxiliaryEffectSlots && alIsAuxiliaryEffectSlot && alDeleteAuxiliaryEffectSlots && alAuxiliaryEffectSloti &&
                          alAuxiliaryEffectSlotiv && alAuxiliaryEffectSlotf && alAuxiliaryEffectSlotfv && alGetAuxiliaryEffectSloti &&
                          alGetAuxiliaryEffectSlotiv && alGetAuxiliaryEffectSlotf && alGetAuxiliaryEffectSlotfv);



    alGenEffects = (LPALGENEFFECTS) alGetProcAddress("alGenEffects");
    alIsEffect = (LPALISEFFECT) alGetProcAddress("alIsEffect");
    alDeleteEffects = (LPALDELETEEFFECTS) alGetProcAddress("alDeleteEffects");
    alEffecti = (LPALEFFECTI) alGetProcAddress("alEffecti");
    alEffectiv = (LPALEFFECTIV) alGetProcAddress("alEffectiv");
    alEffectf = (LPALEFFECTF) alGetProcAddress("alEffectf");
    alEffectfv = (LPALEFFECTFV) alGetProcAddress("alEffectfv");
    alGetEffecti = (LPALGETEFFECTI) alGetProcAddress("alGetEffecti");
    alGetEffectiv = (LPALGETEFFECTIV) alGetProcAddress("alGetEffectiv");
    alGetEffectf = (LPALGETEFFECTF) alGetProcAddress("alGetEffectf");
    alGetEffectfv = (LPALGETEFFECTFV) alGetProcAddress("alGetEffectfv");

    bool alEffectCheck = (alGenEffects && alIsEffect && alDeleteEffects && alEffecti &&
                          alEffectiv && alEffectf && alEffectfv && alGetEffecti &&
                          alGetEffectiv && alGetEffectf && alGetEffectfv);

    alGenFilters = (LPALGENFILTERS) alGetProcAddress("alGenFilters");
    alIsFilter = (LPALISFILTER) alGetProcAddress("alIsFilter");
    alDeleteFilters = (LPALDELETEFILTERS) alGetProcAddress("alDeleteFilters");
    alFilteri = (LPALFILTERI) alGetProcAddress("alFilteri");
    alFilteriv = (LPALFILTERIV) alGetProcAddress("alFilteriv");
    alFilterf = (LPALFILTERF) alGetProcAddress("alFilterf");
    alFilterfv = (LPALFILTERFV) alGetProcAddress("alFilterfv");
    alGetFilteri = (LPALGETFILTERI) alGetProcAddress("alGetFilteri");
    alGetFilteriv = (LPALGETFILTERIV) alGetProcAddress("alGetFilteriv");
    alGetFilterf = (LPALGETFILTERF) alGetProcAddress("alGetFilterf");
    alGetFilterfv = (LPALGETFILTERFV) alGetProcAddress("alGetFilterfv");

    bool alFilterCheck = (alGenFilters && alIsFilter && alDeleteFilters && alFilteri &&
                          alFilteriv && alFilterf && alFilterfv && alGetFilteri &&
                          alGetFilteriv && alGetFilterf && alGetFilterfv);


    if(!(alAuxiliaryEffectSlotCheck && alEffectCheck && alFilterCheck)) return false;

    sm_initialized = true;
    return true;
}
