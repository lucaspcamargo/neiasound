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
#include "nSoundEffectParameters.h"
#include "AL/al.h"
#include "util/nEfxHelper.h"


nSoundEffectParameters::nSoundEffectParameters(QObject * parent)
    : QObject(parent)
{

}

nSoundEffectType nSoundEffectParameters::type()
{
    return m_type;
}

void nSoundEffectParameters::setType(nSoundEffectType type)
{
    m_type=type;
}

QVariant nSoundEffectParameters::parameter(QString name)
{
    return m_parameters.value(name);
}

void nSoundEffectParameters::setParameter(QString name, QVariant param)
{
    Q_UNUSED(name)
    Q_UNUSED(param)

}

void nSoundEffectParameters::clearParameters()
{
    m_parameters.clear();
}
