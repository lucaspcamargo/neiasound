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
#include "nSoundBuffer.h"
#include "nSoundSystem.h"
#include "nSoundBag.h"
#include "nSoundStream.h"
#include "AL/al.h"

nSoundBuffer::nSoundBuffer(QString name, nSoundSystem * parent) :
    QObject(parent)
{
    setObjectName(name);

    alGetError(); //reset error state

    unsigned int hnd;
    alGenBuffers(1, &hnd);
    m_handle = hnd;

    ALenum error = alGetError();
    if(error!=AL_NO_ERROR)
    {
        qWarning("nSoundBuffer: alGenBuffers() failed.");
    }
}

nSoundBuffer::~nSoundBuffer()
{
    alGetError();

    alDeleteBuffers(1, &m_handle);    

    if(alGetError()!=AL_NO_ERROR)
        qWarning("nSoundBuffer::~nSoundBuffer(): Error destroying openal buffer.");
}

void nSoundBuffer::setData(nSoundBag * bag)
{
    alGetError();

    alBufferData(m_handle, openalFormat(bag->m_format), bag->m_data, bag->m_data_size, bag->m_frequency);

    if(alGetError()!=AL_NO_ERROR)
        qWarning("nSoundBuffer::setData(): Error loading buffer data.");
}

void nSoundBuffer::setData(nSoundStream * stream)
{
    nSoundBag * tmp = stream->createSoundBag();
    setData(tmp);
    delete tmp;
}

int nSoundBuffer::openalFormat(nSoundFormat format)
{
    switch(format)
    {
    case SF_8BIT_MONO:
        return AL_FORMAT_MONO8;

    case SF_8BIT_STEREO:
        return AL_FORMAT_STEREO8;

    case SF_16BIT_MONO:
        return AL_FORMAT_MONO16;

    case SF_16BIT_STEREO:
        return AL_FORMAT_STEREO16;

    case SF_WAVE_HEADER:
    case SF_UNDEFINED:
        return -1;
    }

    return -1;
}
