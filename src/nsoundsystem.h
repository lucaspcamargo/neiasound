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
#ifndef NSOUNDSYSTEM_H
#define NSOUNDSYSTEM_H

#include <QObject>
#include <QHash>

#include "neiasound_global.h"
#include "nsoundenums.h"

#ifdef ANDROID
#define NEIASOUND_FREQ 22050
#else
#define NEIASOUND_FREQ 48000
#endif

// *sigh* ugly but necessary
struct ALCcontext_struct;
struct ALCdevice_struct;
typedef struct ALCcontext_struct ALCcontext;
typedef struct ALCdevice_struct ALCdevice;

class nSoundSource;
class nSoundBuffer;
class nSoundListener;
class nSoundStreamer;
class nSoundStreamerPlaylist;
class nSoundFilter;

class NEIASOUNDSHARED_EXPORT nSoundSystem : public QObject
{
    Q_OBJECT
    Q_ENUMS(nSoundFormat)
    Q_ENUMS(nSoundSourceRole)

    Q_PROPERTY(int supportedAuxiliarySends READ supportedAuxiliarySends)
    Q_PROPERTY(qreal masterGain READ masterGain WRITE setMasterGain NOTIFY masterGainChanged)
    Q_PROPERTY(nSoundListener * listener READ listener)
    Q_PROPERTY(ALCcontext * openalContext READ openalContext)
    Q_PROPERTY(ALCdevice * openalDevice READ openalDevice)
public:
    explicit nSoundSystem(QObject *parent = 0);
    virtual ~nSoundSystem();

    ALCcontext * openalContext(){return m_context;}
    ALCdevice * openalDevice(){return m_device;}

    nSoundListener * listener(){return m_listener;}

    qreal masterGain();

    int supportedAuxiliarySends(){return m_numSends;}

signals:

    void masterGainChanged(qreal arg);

public slots:

    void update(qreal);

    void setMasterGain(qreal gain);

    nSoundSource * createSource(QString name = "", nSoundSourceRole = SSR_SFX);
    nSoundSource * source(QString name);
    bool destroySource(QString name);
    bool destroySource(nSoundSource * source);

    nSoundBuffer * createBuffer(QString name = "");
    nSoundBuffer * buffer(QString name);
    bool destroyBuffer(QString name);
    bool destroyBuffer(nSoundBuffer * buffer);


    nSoundStreamer * createStreamer(QString name, nSoundSource * source, nSoundStreamerPlaylist * playlist = 0);
    nSoundStreamerPlaylist * createStreamerPlaylist(QObject * parent);
    nSoundStreamer * streamer(QString name);
    bool destroyStreamer(QString name);
    bool destroyStreamer(nSoundStreamer * streamer);

    nSoundFilter * createFilter(QString name = "", nSoundFilterType type = SFT_LOWPASS);
    nSoundFilter * filter(QString name);
    bool destroyFilter(QString name);
    bool destroyFilter(nSoundFilter * Filter);

private:
    QHash<QString, nSoundSource*> m_sources;
    QHash<QString, nSoundBuffer*> m_buffers;
    QHash<QString, nSoundStreamer*> m_streamers;
    QHash<QString, nSoundFilter*> m_filters;

    nSoundListener * m_listener;

    bool m_success;
    int m_numSends;
    ALCcontext * m_context;
    ALCdevice * m_device;
    qreal m_masterGain;
};

#endif // NSOUNDSYSTEM_H
