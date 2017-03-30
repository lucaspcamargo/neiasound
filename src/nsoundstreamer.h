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
#ifndef NSOUNDSTREAMER_H
#define NSOUNDSTREAMER_H

#include "neiasound_global.h"
#include <QObject>
#include <QThread>
#include <QMutex>

#include "nsoundformat.h"

class nSoundSystem;
class nSoundSource;
class nSoundBag;
class nSoundStream;
class nSoundStreamerPlaylist;


class nSoundStreamerUpdater;

class NEIASOUNDSHARED_EXPORT nSoundStreamer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(nSoundSource* source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(nSoundStreamerPlaylist* playlist READ playlist WRITE setPlaylist NOTIFY playlistChanged)
public:
    explicit nSoundStreamer(QString name, nSoundSource * source,  nSoundStreamerPlaylist * playlist, nSoundSystem * parent);
    virtual ~nSoundStreamer();

    nSoundSource* source() const
    {
        return m_source;
    }

    nSoundStreamerPlaylist* playlist() const
    {
        return m_playlist;
    }

signals:


    void sourceChanged(nSoundSource* arg);

    void playlistChanged(nSoundStreamerPlaylist* arg);

public slots:
    void update(float);
    void rewind();

    void setSource(nSoundSource* arg)
    {
        if (m_source == arg)
            return;

        m_source = arg;
        emit sourceChanged(arg);
    }

    void setPlaylist(nSoundStreamerPlaylist* arg)
    {
        if (m_playlist == arg)
            return;

        m_playlist = arg;
        emit playlistChanged(arg);
    }

private:
    friend class nSoundStreamerUpdater;

    bool fillAndQueueBuffer(unsigned int buffer);
    int openalFormat(nSoundFormat format);

    nSoundStreamerPlaylist * m_playlist;
    int m_currentStream;

    nSoundSource * m_source;
    nSoundBag * m_bag;

    bool m_keepStreaming;
    unsigned int m_buffer0, m_buffer1, m_buffer2;

    QMutex _mutex;
    nSoundStreamerUpdater * m_updater;

};


class nSoundStreamerUpdater : public QObject
{
    Q_OBJECT

public:
    nSoundStreamerUpdater(nSoundStreamer * parent);
    ~nSoundStreamerUpdater();

    void setup();
    void timerEvent(QTimerEvent *);
private:
    friend class nSoundStreamer;
    nSoundStreamer * _streamer;
    bool _keepGoing;
};

#endif // NSOUNDSTREAMER_H
