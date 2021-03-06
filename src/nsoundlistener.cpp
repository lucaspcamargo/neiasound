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
#include "nsoundlistener.h"
#include "nsoundsystem.h"

#include "neiasound_al.h"

nSoundListener::nSoundListener(nSoundSystem * parent) :
    QObject(parent)
{
    m_updating = true;
}


void nSoundListener::update(qreal frameTime)
{
    Q_UNUSED(frameTime)

}

void nSoundListener::updateManual(QVector3D pos, QVector3D dir, QVector3D up, QVector3D vel)
{
    float orient[6];
    orient[0] = dir[0];
    orient[1] = dir[1];
    orient[2] = dir[2];
    orient[3] = up[0];
    orient[4] = up[1];
    orient[5] = up[2];

    alGetError();
    alListener3f(AL_POSITION, pos[0], pos[1], pos[2]);
    alListenerfv(AL_ORIENTATION, orient);
    alListener3f(AL_VELOCITY, vel[0], vel[1], vel[2]);

    if(alGetError()!=AL_NO_ERROR)
        qWarning("nSoundListener: Failed to update listener transformations.");
}
