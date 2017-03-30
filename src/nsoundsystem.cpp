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
#include "nsoundsystem.h"
#include "nsoundsource.h"
#include "nsoundbuffer.h"
#include "nsoundlistener.h"
#include "nsoundstreamer.h"
#include "nsoundstreamerplaylist.h"
#include "nsoundfilter.h"

#include "neiasound_al.h"

#include "util/nefxhelper.h"
#include <QSettings>
#include <QUuid>
#include <QDebug>

nSoundSystem::nSoundSystem(QObject *parent) :
    QObject(parent), m_success(false)
{
    qDebug("nSoundSystem initializing.");

    m_context = 0;
    m_device = 0;

    m_device = alcOpenDevice(0); //default device
    if(!m_device)
        qWarning("Error creating OpenAL device.");

    int attributes[] = {ALC_FREQUENCY, NEIASOUND_FREQ, 0};
    m_context = alcCreateContext(m_device, attributes);
    if(!m_context)
        qWarning("Error creating OpenAL context.");

    alcMakeContextCurrent(m_context);
    if( alGetError()!=AL_NO_ERROR)
        qWarning("nSoundSystem::nSoundSystem(): Failed to set context as current.");

    if(!nEfxHelper::initialize(m_device))
        qWarning("nSoundSystem::nSoundSystem(): nEfxHelper initialization failed.");

    m_listener = new nSoundListener(this);
    m_success = true;

#ifdef ANDROID
#ifdef MOB
    alcDeviceEnableHrtfMOB( m_device, false );
#endif
#endif

    alcGetIntegerv(m_device, ALC_MAX_AUXILIARY_SENDS, 1, &m_numSends);
    
    nSoundSource::_resetRoleGains();
    QSettings settings;
    settings.beginGroup("Audio");
    setMasterGain(settings.value("MasterGain", 100).toFloat()/100.0f);
    settings.endGroup();


    int efxMaj, efxMin;
    alcGetIntegerv(m_device, ALC_EFX_MAJOR_VERSION, 1, &efxMaj);
    alcGetIntegerv(m_device, ALC_EFX_MINOR_VERSION, 1, &efxMin);

    qDebug("nSoundSystem::nSoundSystem(): initialized successfully");
    qDebug() << QStringLiteral("EFX OpenAL Extension version %1.%2").arg(efxMaj).arg(efxMin);

    nEfxHelper::initialize(m_device);

}

nSoundSystem::~nSoundSystem()
{
    // ensure deletion of resources in right order
    QHash<QString, nSoundSource*> sources = m_sources;
    QHash<QString, nSoundBuffer*> buffers = m_buffers;
    QHash<QString, nSoundStreamer*> streamers = m_streamers;

    foreach(nSoundStreamer * streamer, streamers)
    {
        delete (streamer);
    }

    foreach(nSoundSource * source, sources)
    {
        delete (source);
    }

    foreach(nSoundBuffer * buffer, buffers)
    {
        delete (buffer);
    }


    alcMakeContextCurrent(0);
    alcDestroyContext(m_context);
    alcCloseDevice(m_device);
}

void nSoundSystem::update(qreal frameTime)
{

    m_listener->update(frameTime);

//    Not necessary anymore, streamers update themselves in owned threads.

//    foreach(nSoundStreamer * streamer, m_streamers)
//    {
//        streamer->update(frameTime);
//    }

    QList<nSoundSource*> toDestroy;
    foreach(nSoundSource * source, m_sources)
    {
        if(!source->update(frameTime))
            toDestroy.append(source);
    }

    foreach (nSoundSource * source, toDestroy) {
        destroySource(source);
    }
}

// SOUND STUFF METHODS

qreal nSoundSystem::masterGain()
{
    float ret;
    alGetListenerf(AL_GAIN, &ret);
    return ret;
}

void nSoundSystem::setMasterGain(qreal gain)
{
    alListenerf(AL_GAIN, gain);

    emit masterGainChanged(gain);
}

// SOURCE METHODS

nSoundSource * nSoundSystem::createSource(QString name, nSoundSourceRole role)
{
    if(name.isEmpty())
    {
        name = QStringLiteral("nSoundSource_%1").arg(QUuid::createUuid().toString());
    }

    nSoundSource * src;
    try{
        src = new nSoundSource(name, role, this);
        m_sources.insert(name, src);
        return src;
    }catch(...)
    {
        return 0;
    }
}

nSoundSource * nSoundSystem::source(QString name)
{
    return m_sources.value(name, 0);
}

bool nSoundSystem::destroySource(QString name)
{
    nSoundSource * src = m_sources.value(name, 0);
    if(!src) return false;
    m_sources.remove(name);
    delete src;
    return true;
}

bool nSoundSystem::destroySource(nSoundSource * source)
{
    nSoundSource * src = m_sources.value(source->objectName(), 0);
    if(!src) return false;
    m_sources.remove(source->objectName());
    delete src;
    return true;
}


// BUFFER METHODS

nSoundBuffer * nSoundSystem::createBuffer(QString name)
{
    if(name.isEmpty())
    {
        name = QStringLiteral("nSoundBuffer_%1").arg(QUuid::createUuid().toString());
    }

    nSoundBuffer * buf = new nSoundBuffer(name, this);
    m_buffers.insert(name, buf);
    return buf;
}

nSoundBuffer * nSoundSystem::buffer(QString name)
{
    return m_buffers.value(name, 0);
}

bool nSoundSystem::destroyBuffer(QString name)
{
    nSoundBuffer * buf = m_buffers.value(name, 0);
    if(!buf) return false;
    m_buffers.remove(name);
    delete buf;
    return true;
}

bool nSoundSystem::destroyBuffer(nSoundBuffer * buffer)
{
    nSoundBuffer * buf = m_buffers.value(buffer->objectName(), 0);
    if(!buf) return false;
    m_buffers.remove(buffer->objectName());
    delete buf;
    return true;
}


// STREAMER METHODS

nSoundStreamer * nSoundSystem::createStreamer(QString name, nSoundSource * source, nSoundStreamerPlaylist * playlist)
{
    if(name.isEmpty())
    {
        name = QStringLiteral("nSoundStreamer_%1").arg(QUuid::createUuid().toString());
    }

    nSoundStreamer * streamer = new nSoundStreamer(name, source, playlist, this);
    playlist->setParent(streamer);
    m_streamers.insert(name, streamer);
    return streamer;
}

nSoundStreamerPlaylist *nSoundSystem::createStreamerPlaylist(QObject *parent)
{
    return new nSoundStreamerPlaylist(parent);
}

nSoundStreamer * nSoundSystem::streamer(QString name)
{
    return m_streamers.value(name, 0);
}

bool nSoundSystem::destroyStreamer(QString name)
{
    nSoundStreamer * streamer = m_streamers.value(name, 0);
    if(!streamer) return false;
    m_streamers.remove(name);
    delete streamer;
    return true;
}

bool nSoundSystem::destroyStreamer(nSoundStreamer * streamer)
{
    nSoundStreamer * strm = m_streamers.value(streamer->objectName(), 0);
    if(!strm) return false;
    m_streamers.remove(strm->objectName());
    delete strm;
    return true;
}


// FILTER METHODS

nSoundFilter * nSoundSystem::createFilter(QString name, nSoundFilterType type)
{
    if(name.isEmpty())
    {
        name = QStringLiteral("nSoundFilter_%1").arg(QUuid::createUuid().toString());
    }

    nSoundFilter * src;
    try{
        src = new nSoundFilter(name, type, this);
        m_filters.insert(name, src);
        return src;
    }catch(...)
    {
        return 0;
    }
}

nSoundFilter * nSoundSystem::filter(QString name)
{
    return m_filters.value(name, 0);
}

bool nSoundSystem::destroyFilter(QString name)
{
    nSoundFilter * filter = m_filters.value(name, 0);
    if(!filter) return false;
    m_filters.remove(name);
    delete filter;
    return true;
}

bool nSoundSystem::destroyFilter(nSoundFilter * filter)
{
    return destroyFilter(filter->objectName());
}

