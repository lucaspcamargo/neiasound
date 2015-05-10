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
#ifndef NSOUNDFORMAT_H
#define NSOUNDFORMAT_H

#include <QtGlobal>

enum nSoundFormat
{
    SF_UNDEFINED,
    SF_8BIT_MONO,
    SF_8BIT_STEREO,
    SF_16BIT_MONO,
    SF_16BIT_STEREO,
    SF_WAVE_HEADER
};

inline quint64 nSoundFormat_getFramesize(nSoundFormat sf)
{
    switch(sf)
    {
    case SF_UNDEFINED:
        return 0;

    case SF_8BIT_MONO:
        return 1;

    case SF_8BIT_STEREO:
        return 2;

    case SF_16BIT_MONO:
        return 2;

    case SF_16BIT_STEREO:
        return 4;

    default:
        return 0;
    }
}


inline int nSoundFormat_getChannelCount(nSoundFormat sf)
{
    switch(sf)
    {
    case SF_UNDEFINED:
        return 0;

    case SF_8BIT_MONO:
    case SF_16BIT_MONO:
        return 1;

    case SF_8BIT_STEREO:
    case SF_16BIT_STEREO:
        return 2;

    default:
        return 0;
    }
}
#endif // NSOUNDFORMAT_H
