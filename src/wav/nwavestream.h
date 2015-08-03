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

#ifndef DWSOUNDRAWSTREAM_H
#define DWSOUNDRAWSTREAM_H

#include "../neiasound_global.h"
#include "../nsoundstream.h"

class QIODevice;

class NEIASOUNDSHARED_EXPORT nWaveStream : public nSoundStream
{
    Q_OBJECT
public:
    explicit nWaveStream(QIODevice * device, nSoundFormat format, int frequency, int channels = -1, QObject *parent = 0);

signals:

public slots:
    virtual quint64 frames() { return _totalFrames; }
    virtual int channels() { return _channels; }
    virtual int frequency() { return _frequency; }

    virtual nSoundBag * createSoundBag(QObject * parent = 0);

    virtual nSoundFormat format() { return _format; }
    virtual bool suggestStreaming() { return false; }

    virtual quint64 read(void* data, unsigned long frames);

    virtual void rewind();

private:

    QIODevice * _device;
    nSoundFormat _format;
    int _frequency;
    quint64 _totalFrames;
    int _channels;
    int _deviceOffset;

};

#endif // DWSOUNDRAWSTREAM_H
