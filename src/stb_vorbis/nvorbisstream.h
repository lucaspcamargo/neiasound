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

#include "../neiasound_global.h"
#ifndef NVORBISSTREAM_H
#define NVORBISSTREAM_H

#include "../nSoundStream.h"

class QIODevice;
class stb_vorbis;

class NEIASOUNDSHARED_EXPORT nVorbisStream : public nSoundStream
{
    Q_OBJECT    
    Q_PROPERTY(bool error READ error CONSTANT)
public:
    explicit nVorbisStream(QIODevice * device, QObject *parent = 0);
    ~nVorbisStream();



    bool error() const
    {
        return m_error;
    }

signals:

public slots:
    virtual quint64 frames() { return _totalFrames; }
    virtual int channels() { return _channels; }
    virtual int frequency() { return _frequency; }

    virtual nSoundFormat format() { return _format; }
    virtual bool suggestStreaming() { return _totalFrames > 88200; }

    virtual quint64 read(void* data, unsigned long frames);
    virtual void rewind();

private:
    QIODevice * _device;
    QByteArray _qtBuf;
    char * _buf;
    int _bufSize;

    stb_vorbis * _vorbis;

    quint64 _totalFrames;
    int _channels;
    int _frequency;
    nSoundFormat _format;

    bool m_error;
};

#endif // NVORBISSTREAM_H
