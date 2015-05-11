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
#include "nvorbisstream.h"

#include <QIODevice>
#include "../nSoundBag.h"


#define STB_VORBIS_MAX_CHANNELS     2
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wtype-limits"
#pragma GCC diagnostic ignored "-Wparentheses"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-value"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#include "stb_vorbis.c"
#pragma GCC diagnostic pop


nVorbisStream::nVorbisStream(QIODevice * dev, QObject *parent) : nSoundStream(parent),
    _device(dev),
    _totalFrames(0),
    _channels(0),
    _frequency(0),
    _format(SF_16BIT_STEREO),
    m_error(false)
{
    _vorbis = 0;

    if(!dev->isOpen())
        dev->open(QIODevice::ReadOnly);

    _qtBuf = dev->readAll();

    dev->close();

    if(_qtBuf.size())
    {
        _bufSize = _qtBuf.size();
        _buf = _qtBuf.data();


        int err = VORBIS__no_error;
        _vorbis = stb_vorbis_open_memory((unsigned char*)_buf, _bufSize, &err,  0 );
        if(!_vorbis || err != VORBIS__no_error)
        {
            qDebug("[nVorbisStream] Error initializing vorbis stream");
        }

        //stb_vorbis_info info = stb_vorbis_get_info(_vorbis);

        _totalFrames = stb_vorbis_stream_length_in_samples(_vorbis);
        _channels = _vorbis->channels;
        _frequency = _vorbis->sample_rate;

        switch (channels()) {
        case 1:
            _format = SF_16BIT_MONO;
            break;
        case 2:
            _format = SF_16BIT_STEREO;
            break;
        default:
            _format = SF_UNDEFINED;
            break;
        }
    }
    else
    {
        qDebug("[nVorbisStream] Error reading QIODevice");
    }

}

nVorbisStream::~nVorbisStream()
{
    stb_vorbis_close(_vorbis);
    _qtBuf.clear();
}

void nVorbisStream::rewind()
{
    if(_vorbis) stb_vorbis_seek_start(_vorbis);
}

quint64 nVorbisStream::read(void *data, unsigned long frames)
{
    if(!frames) return 0;
    return stb_vorbis_get_samples_short_interleaved(_vorbis, _channels, (short*) data, frames * _channels );

}

