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
#ifndef NSOUNDSOURCE_H
#define NSOUNDSOURCE_H

#include "neiasound_global.h"
#include <QObject>
#include <QVector3D>
#include "nSoundSourceRole.h"

class nSoundSystem;
class nSoundBuffer;
class nSoundFilter;

class NEIASOUNDSHARED_EXPORT nSoundSource : public QObject
{
    Q_OBJECT
    Q_ENUMS(nSoundSourceState)
    Q_ENUMS(nSoundSourceRole)

    Q_PROPERTY(unsigned int openalHandle READ openalHandle)
    Q_PROPERTY(nSoundSourceRole role READ role)
    Q_PROPERTY(nSoundSourceState state READ state)
    Q_PROPERTY(qreal gain READ gain WRITE setGain)
    Q_PROPERTY(qreal pitch READ pitch WRITE setPitch)
    Q_PROPERTY(qreal rolloffFactor READ rolloffFactor WRITE setRolloffFactor)
    Q_PROPERTY(bool loop READ loop WRITE setLoop)
    Q_PROPERTY(QVector3D position READ position WRITE setPosition)
    Q_PROPERTY(bool destroyAfterStopped READ destroyAfterStopped WRITE setDestroyAfterStopped NOTIFY onDestroyAfterStoppedChanged )

    Q_PROPERTY(qreal fading READ isFading)
    Q_PROPERTY(qreal fadeTarget READ fadeTarget)
    Q_PROPERTY(qreal fadeDeltaPerSecond READ fadeDeltaPerSecond)


public:
    enum nSoundSourceState
    {
        SSS_UNKNOWN,
        SSS_PLAYING,
        SSS_PAUSED,
        SSS_STOPPED,
        SSS_INITIAL
    };

    explicit nSoundSource(QString name, nSoundSourceRole role, nSoundSystem * parent);
    virtual ~nSoundSource();

    unsigned int openalHandle(){return m_handle;}
    nSoundSourceState state();
    nSoundSourceRole role(){return m_role;}
    qreal gain();
    qreal pitch();
    qreal rolloffFactor();
    bool loop();
    QVector3D position();

    bool isFading() const {return m_fading;}
    qreal fadeTarget() const {return m_fadeTarget;}
    qreal fadeDeltaPerSecond() const {return m_fadeDeltaPerSecond;}

    // GAIN CFG SUPPORT
    static qreal _roleGainVolume(nSoundSourceRole role);
    static void _resetRoleGains();

    bool destroyAfterStopped() const
    {
        return m_destroyAfterStopped;
    }

signals:

    void onDestroyAfterStoppedChanged(bool arg);

public slots:
    bool update(qreal frameTime);

    void attachBuffer(nSoundBuffer * buffer);
    void attachDirectFilter(nSoundFilter * filter);
    void detachDirectFilter();

    void setGain(qreal);
    void setPitch(qreal);
    void setRolloffFactor(qreal);
    void setLoop(bool);
    void setPosition(QVector3D pos);

    void fade(qreal to, qreal duration, qreal from = -1.0f);

    void play();
    void pause();
    void stop();
    void rewind();

    void setDestroyAfterStopped(bool arg)
    {
        if (m_destroyAfterStopped == arg)
            return;

        m_destroyAfterStopped = arg;
        emit onDestroyAfterStoppedChanged(arg);
    }

private:
    unsigned int m_handle;
    nSoundSourceRole m_role;

    qreal m_gainMirror;

    bool m_fading;
    qreal m_fadeTarget;
    qreal m_fadeDeltaPerSecond;

    //GAIN CFG SUPPORT
    static bool _m_gainsInitialized;
    static qreal _m_musicGain;
    static qreal _m_sfxGain;
    static qreal _m_voiceGain;
    static qreal _m_ambienceGain;

    bool m_destroyAfterStopped;
};

#endif // NSOUNDSOURCE_H
