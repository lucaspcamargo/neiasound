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
#include "nSoundSource.h"
#include "nSoundSystem.h"
#include "nSoundBuffer.h"
#include "nsoundfilter.h"

#include "neiasound_al.h"


nSoundSource::nSoundSource(QString name, nSoundSourceRole role, nSoundSystem * parent) :
    QObject(parent)
{
    setObjectName(name);
    m_role = role;
    m_gainMirror = _roleGainVolume(role);
    m_fading = false;
    m_fadeTarget = 0.0f;
    m_fadeDeltaPerSecond = 0.0f;
    m_destroyAfterStopped = false;

    alGetError(); //reset error state

    unsigned int hnd;
    alGenSources(1, &hnd);
    m_handle = hnd;

    ALenum error = alGetError();
    if(error!=AL_NO_ERROR)
    {
        qWarning("nSoundSource: alGenSources() failed.");
    }

    alSourcef(m_handle, AL_GAIN, m_gainMirror);
    alSource3f(m_handle, AL_POSITION, 0.0f ,0.0f ,0.0f);
    alSource3f(m_handle, AL_VELOCITY, 0.0f ,0.0f ,0.0f);
    alSource3f(m_handle, AL_DIRECTION, 0.0f, 0.0f, 0.0f);
    if(error!=AL_NO_ERROR)
        qWarning("nSoundSource: Setting of source properties failed.");

}

nSoundSource::~nSoundSource()
{
    alDeleteSources(1, &m_handle);
}

// PROPERTIES

nSoundSource::nSoundSourceState nSoundSource::state()
{
    alGetError();

    int state;
    alGetSourcei(m_handle, AL_SOURCE_STATE, &state);

    switch (state)
    {
    case AL_INITIAL:
        return SSS_INITIAL;

    case AL_PLAYING:
        return SSS_PLAYING;

    case AL_PAUSED:
        return SSS_PAUSED;

    case AL_STOPPED:
        return SSS_STOPPED;
    }

    return SSS_UNKNOWN;

}

qreal nSoundSource::gain()
{
    return m_gainMirror;
}

void nSoundSource::setGain(qreal f)
{
    if(f<0.0f)
        f=0.0f;
    alSourcef(m_handle, AL_GAIN, f);    
    m_gainMirror = f;
}


qreal nSoundSource::pitch()
{
    ALfloat pitch;
    alGetSourcef(m_handle, AL_PITCH, &pitch);
    return pitch;
}

void nSoundSource::setPitch(qreal f)
{
    if(f<0.0f)
        f=0.0f;
    alSourcef(m_handle, AL_PITCH, f);
}


qreal nSoundSource::rolloffFactor()
{
    ALfloat rolloff;
    alGetSourcef(m_handle, AL_ROLLOFF_FACTOR, &rolloff);
    return rolloff;
}

void nSoundSource::setRolloffFactor(qreal f)
{
    if(f<0.0f)
        f=0.0f;
    alSourcef(m_handle, AL_ROLLOFF_FACTOR, f);
}


bool nSoundSource::loop()
{
    ALint loop;
    alGetSourcei(m_handle, AL_LOOPING, &loop);
    return (loop?true:false);
}

void nSoundSource::setLoop(bool b)
{
    alSourcei(m_handle, AL_LOOPING, (b?AL_TRUE:AL_FALSE));
}

QVector3D nSoundSource::position()
{
    ALfloat x,y,z;
    alGetSource3f(m_handle, AL_POSITION, &x, &y, &z);
    return QVector3D(x,y,z);
}

void nSoundSource::setPosition(QVector3D pos)
{
    alSource3f(m_handle, AL_POSITION, pos.x(), pos.y(), pos.z() );
}


// METHODS

bool nSoundSource::update(qreal frameTime)
{
    if(m_fading)
    {
        qreal newGain = m_gainMirror + frameTime*m_fadeDeltaPerSecond;
        if( ((m_fadeDeltaPerSecond < 0.0f)&&(newGain<m_fadeTarget)) ||
            ((m_fadeDeltaPerSecond > 0.0f)&&(newGain>m_fadeTarget)) )
        {
            newGain = m_fadeTarget;
            m_fading = false;
        }
        setGain(newGain);
    }if(destroyAfterStopped() && state() == SSS_STOPPED)
    {
        return false;
    }

    return true;
}

void nSoundSource::fade(qreal to, qreal duration, qreal from)
{
    if(to==from)
    {
        setGain(to);
        m_fading = false;
        return;
    }

    qreal newGain;
    if(from >= 0.0f)
    {
        setGain(from);
        newGain = from;
    }else newGain = gain();

    m_fadeTarget = to;
    m_fadeDeltaPerSecond = (to - newGain)/duration;
    m_fading = true;

}

void nSoundSource::attachBuffer(nSoundBuffer * buffer)
{
    if(!buffer) return;
    alGetError();

    ALint sourceType;
    alGetSourcei(m_handle, AL_SOURCE_TYPE, &sourceType);
    if(sourceType == AL_STREAMING)
        qWarning("nSoundSource: Tried to play a single buffer on a streaming source.");

    alSourcei(m_handle, AL_BUFFER, buffer->openalHandle());

    ALenum err = alGetError();
    if(err!=AL_NO_ERROR)
        qWarning("nSoundSource: failed to bind buffer to source");
}

void nSoundSource::attachDirectFilter(nSoundFilter *filter)
{
    alSourcei(m_handle, AL_DIRECT_FILTER, filter? filter->openalHandle() : AL_FILTER_NULL);
}

void nSoundSource::detachDirectFilter()
{
    attachDirectFilter(0);
}

void nSoundSource::play()
{
    alGetError();
    alSourcePlay(m_handle);
    if(alGetError()!=AL_NO_ERROR)
        qWarning("nSoundSource: Failed to play source.");
}

void nSoundSource::pause()
{
    alGetError();
    alSourcePause(m_handle);
    if(alGetError()!=AL_NO_ERROR)
        qWarning("nSoundSource: Failed to pause source.");
}

void nSoundSource::stop()
{
    alGetError();
    alSourceStop(m_handle);
    if(alGetError()!=AL_NO_ERROR)
        qWarning("nSoundSource: Failed to stop source.");
}

void nSoundSource::rewind()
{
    alGetError();
    alSourceRewind(m_handle);
    if(alGetError()!=AL_NO_ERROR)
        qWarning("nSoundSource: Failed to rewind source.");
}

// GAIN CFG SUPPORT

bool nSoundSource::_m_gainsInitialized = false;
qreal nSoundSource::_m_musicGain = 0.0f;
qreal nSoundSource::_m_sfxGain = 0.0f;
qreal nSoundSource::_m_voiceGain = 0.0f;
qreal nSoundSource::_m_ambienceGain = 0.0f;

#include <QSettings>

void nSoundSource::_resetRoleGains()
{
    QSettings settings;
    settings.beginGroup("Audio");
    _m_musicGain = settings.value("MusicGain", 60).toFloat()/100.0f;
    _m_sfxGain = settings.value("SfxGain", 90).toFloat()/100.0f;
    _m_voiceGain = settings.value("VoiceGain", 70).toFloat()/100.0f;
    _m_ambienceGain = settings.value("AmbienceGain", 60).toFloat()/100.0f;
    _m_gainsInitialized = true;
    settings.endGroup();

    _m_gainsInitialized = true;
}

qreal nSoundSource::_roleGainVolume(nSoundSourceRole role)
{
    if(!_m_gainsInitialized)
        nSoundSource::_resetRoleGains();

    switch(role)
    {
    case SSR_MUSIC:
        return _m_musicGain;
    case SSR_SFX:
        return _m_sfxGain;
    case SSR_AMBIENCE:
        return _m_ambienceGain;
    case SSR_VOICE:
        return _m_voiceGain;
    default:
        return 1.0f;
    }

}

