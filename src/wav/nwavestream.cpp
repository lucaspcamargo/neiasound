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
#include "nwavestream.h"
#include "../nSoundBag.h"
#include <QIODevice>


nWaveStream::nWaveStream(QIODevice *device, nSoundFormat format, int frequency, int channels, QObject *parent)
    : nSoundStream(parent),
      _device(device),
      _format(format),
      _frequency(frequency),
      _channels(channels)
{

    if(format == SF_WAVE_HEADER)
    {

        unsigned char header[44];

        device->read(reinterpret_cast<char*>(header), 44);
        _channels = header[22];
        _frequency = header[24] + (((int)header[25])<<8) + (((int)header[26])<<16) + (((int)header[27])<<24);
        _format = (_channels == 1?
                       (header[34]==16? SF_16BIT_MONO : SF_8BIT_MONO):
                       (header[34]==16? SF_16BIT_STEREO : SF_8BIT_STEREO)
                       );
        int chunkSize = header[40] + (((int)header[41])<<8) + (((int)header[42])<<16) + (((int)header[43])<<24);
        _totalFrames = chunkSize / nSoundFormat_getFramesize(_format);
    }
    else
    {
        if(channels < 0) channels = (format == SF_8BIT_STEREO || format  == SF_16BIT_STEREO)? 2 : 1;
    }
}

nSoundBag *nWaveStream::createSoundBag(QObject *parent)
{
    nSoundBag * bag = new nSoundBag( _format, _totalFrames, _frequency );
    read(bag->m_data, _totalFrames);
    return bag;
}


void nWaveStream::rewind()
{
    _device->reset();
}

quint64 nWaveStream::read(void *data, unsigned long frames)
{
    return _device->read( (char *) data, nSoundFormat_getFramesize(_format) * frames ) / nSoundFormat_getFramesize(_format);
}
