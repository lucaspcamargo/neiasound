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
