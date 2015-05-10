#ifndef NEFXHELPER_H
#define NEFXHELPER_H

#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alext.h"

class nEfxHelper
{

public:

    static bool initialize(ALCdevice*);
    static bool isInitialized(){return sm_initialized;}

    static LPALGENAUXILIARYEFFECTSLOTS alGenAuxiliaryEffectSlots;
    static LPALISAUXILIARYEFFECTSLOT alIsAuxiliaryEffectSlot;
    static LPALDELETEAUXILIARYEFFECTSLOTS alDeleteAuxiliaryEffectSlots;
    static LPALAUXILIARYEFFECTSLOTI alAuxiliaryEffectSloti;
    static LPALAUXILIARYEFFECTSLOTIV alAuxiliaryEffectSlotiv;
    static LPALAUXILIARYEFFECTSLOTF alAuxiliaryEffectSlotf;
    static LPALAUXILIARYEFFECTSLOTFV alAuxiliaryEffectSlotfv;
    static LPALGETAUXILIARYEFFECTSLOTI alGetAuxiliaryEffectSloti;
    static LPALGETAUXILIARYEFFECTSLOTIV alGetAuxiliaryEffectSlotiv;
    static LPALGETAUXILIARYEFFECTSLOTF alGetAuxiliaryEffectSlotf;
    static LPALGETAUXILIARYEFFECTSLOTFV alGetAuxiliaryEffectSlotfv;

    static LPALGENEFFECTS alGenEffects;
    static LPALISEFFECT alIsEffect;
    static LPALDELETEEFFECTS alDeleteEffects;
    static LPALEFFECTI alEffecti;
    static LPALEFFECTIV alEffectiv;
    static LPALEFFECTF alEffectf;
    static LPALEFFECTFV alEffectfv;
    static LPALGETEFFECTI alGetEffecti;
    static LPALGETEFFECTIV alGetEffectiv;
    static LPALGETEFFECTF alGetEffectf;
    static LPALGETEFFECTFV alGetEffectfv;

    static LPALGENFILTERS alGenFilters;
    static LPALISFILTER alIsFilter;
    static LPALDELETEFILTERS alDeleteFilters;
    static LPALFILTERI alFilteri;
    static LPALFILTERIV alFilteriv;
    static LPALFILTERF alFilterf;
    static LPALFILTERFV alFilterfv;
    static LPALGETFILTERI alGetFilteri;
    static LPALGETFILTERIV alGetFilteriv;
    static LPALGETFILTERF alGetFilterf;
    static LPALGETFILTERFV alGetFilterfv;

private:
    nEfxHelper();
    static bool sm_initialized;
};

#endif // NEFXHELPER_H
