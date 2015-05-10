#ifndef NSOUNDSCRIPTMETATYPES_H
#define NSOUNDSCRIPTMETATYPES_H

#include "nSoundBag.h"
#include "nSoundBuffer.h"
#include "nSoundEffectParameters.h"
#include "nSoundFormat.h"
#include "nSoundListener.h"
#include "nSoundSource.h"
#include "nSoundStream.h"
#include "nSoundStreamer.h"
#include "nSoundStreamerPlaylist.h"
#include "nSoundSystem.h"

#ifdef NEIA
#include "../util/nScriptMetatypeMacros.h"

Q_DECLARE_METATYPE(nSoundBag*)
Q_DECLARE_METATYPE(nSoundBuffer*)
Q_DECLARE_METATYPE(nSoundEffectParameters*)
Q_DECLARE_METATYPE(nSoundFormat*)
Q_DECLARE_METATYPE(nSoundListener*)
Q_DECLARE_METATYPE(nSoundSource*)
Q_DECLARE_METATYPE(nSoundStream*)
Q_DECLARE_METATYPE(nSoundStreamer*)
Q_DECLARE_METATYPE(nSoundStreamerPlaylist*)
Q_DECLARE_METATYPE(nSoundSystem*)

void nSoundScriptMetatypes_registerAll(QScriptEngine & engine);
#endif

#endif // NSOUNDSCRIPTMETATYPES_H
