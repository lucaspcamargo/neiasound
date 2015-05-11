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
#ifndef NSOUNDEFFECTPARAMETERS_H
#define NSOUNDEFFECTPARAMETERS_H

#include "neiasound_global.h"
#include <QObject>
#include <QVariantMap>

enum NEIASOUNDSHARED_EXPORT nSoundEffectType
{
    SET_NONE,
    SET_REVERB,
    SET_HIGHPASS,
    SET_EAXREVERB,
    SET_CHORUS,
    SET_ECHO,
    SET_FLANGER,
    SET_AUTOWAH,
    SET_COMPRESSOR,
    SET_UNDEFINED
};

quint32 nSoundEffectType_toOpenalType();

class NEIASOUNDSHARED_EXPORT nSoundEffectParameters : public QObject
{
    Q_OBJECT
    Q_ENUMS(nSoundEffectType)
    Q_PROPERTY(nSoundEffectType type READ type WRITE setType )

public:
    nSoundEffectParameters(QObject * parent = 0);

    nSoundEffectType type();
    void setType(nSoundEffectType type);

    QVariant parameter(QString name);
    void setParameter(QString name, QVariant param);
    void clearParameters();

private:
    quint32 m_handle;
    nSoundEffectType m_type;
    QVariantMap m_parameters;

};

#endif // NSOUNDEFFECTPARAMETERS_H
