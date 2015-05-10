#include "nEfxHelper.h"
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
