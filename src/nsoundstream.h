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
#ifndef NSOUNDSTREAM_H
#define NSOUNDSTREAM_H


#include "neiasound_global.h"
#include <QObject>
#include "nsoundformat.h"

class nSoundBag;

class NEIASOUNDSHARED_EXPORT nSoundStream : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int channels READ channels)
    Q_PROPERTY(quint64 frames READ frames)
    Q_PROPERTY(int frequency READ frequency)
    Q_PROPERTY(nSoundFormat format READ format)
    Q_PROPERTY(bool suggestStreaming READ suggestStreaming)
public:
    explicit nSoundStream(QObject *parent = 0);

    virtual quint64 frames()=0;
    virtual int channels()=0;
    virtual int frequency()=0;

    virtual nSoundBag * createSoundBag(QObject * parent = 0);
    virtual nSoundFormat format() = 0;
    virtual bool suggestStreaming() = 0;

    virtual void rewind() = 0;
    virtual quint64 read(void* data, unsigned long frames) = 0;
signals:

public slots:

};

#endif // NSOUNDSTREAM_H
