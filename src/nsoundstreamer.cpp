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
#include "nsoundstreamer.h"
#include "nsoundsystem.h"
#include "nsoundsource.h"
#include "nsoundbag.h"
#include "nsoundstream.h"
#include "nsoundstreamerplaylist.h"
#include "neiasound_al.h"


const int NSS_BUFFER_SIZE = 8192;

nSoundStreamer::nSoundStreamer(QString name, nSoundSource * source, nSoundStreamerPlaylist * playlist, nSoundSystem * parent) :
    QObject(parent)
{
    setObjectName(name);
    m_playlist = playlist;

    if(!playlist->itemCount()) {
        qWarning("nSoundStreamer has no items in playlist");
        return;
    }

    // make sure we can stream to source
    alGetError();
    int sourceType;
    alGetSourcei(source->openalHandle(), AL_SOURCE_TYPE, &sourceType);
    if(sourceType == AL_STATIC)
        qWarning("nSoundStreamer::nSoundStreamer(...): Tried to create stream to an AL_STATIC source.");
    m_source = source;

    // create OpenAL buffers
    alGetError();
    unsigned int buffers[3];
    alGenBuffers(3, buffers);
    m_buffer0 = buffers[0];
    m_buffer1 = buffers[1];
    m_buffer2 = buffers[2];
    if(alGetError()!=AL_NO_ERROR)
        qWarning("nSoundStreamer::nSoundStreamer(...): Failed to create streaming buffers");


    //reserve buffer memory
    m_currentStream = 0;
    nSoundStream * stream = m_playlist->m_items[m_currentStream].m_soundStream;
    m_bag = new nSoundBag(stream->format(),
        (stream->frames() < NSS_BUFFER_SIZE? stream->frames() : NSS_BUFFER_SIZE),
        stream->frequency(), this);

    //fill in initial data and queue buffers
    m_keepStreaming = fillAndQueueBuffer(m_buffer0);
    if(m_keepStreaming) m_keepStreaming = fillAndQueueBuffer(m_buffer1);
    if(m_keepStreaming) m_keepStreaming = fillAndQueueBuffer(m_buffer2);

    // start threaded updater
    QThread * updaterThread = new QThread(0);
    updaterThread->setObjectName(objectName() + "_THREAD");
    m_updater = new nSoundStreamerUpdater(this);
    m_updater->moveToThread(updaterThread);
    updaterThread->start();
    updaterThread->setPriority(QThread::LowPriority);
    m_updater->setup();

}

nSoundStreamer::~nSoundStreamer()
{
    if(!m_playlist->itemCount()) {
        return;
    }

    QMutexLocker lock(&_mutex);

    m_updater->_keepGoing = false;
    alGetError();

    m_source->stop();

    int queuedBuffers;
    alGetSourcei(m_source->openalHandle(), AL_BUFFERS_QUEUED, &queuedBuffers);
    while(queuedBuffers--)
    {
        unsigned int buffer;
        alSourceUnqueueBuffers(m_source->openalHandle(), 1, &buffer);
        if(alGetError()!=AL_NO_ERROR)
        {
            qWarning("nSoundStreamer::~nSoundStreamer(): Failed to unqueue buffer");
        }
    }

    unsigned int buffers[3];
    buffers[0] = m_buffer0;
    buffers[1] = m_buffer1;
    buffers[2] = m_buffer2;
    alDeleteBuffers(3, buffers);

    if(alGetError()!=AL_NO_ERROR)
        qWarning("nSoundStreamer::~nSoundStreamer(): Failed to destroy buffers.");

    delete m_bag;
}

void nSoundStreamer::update(float frameTime)
{
    Q_UNUSED(frameTime)

    if(!m_playlist->itemCount()) {
        return;
    }

    QMutexLocker lock(&_mutex);

    if(m_keepStreaming)
    {
        unsigned int sourceHandle = m_source->openalHandle();

        alGetError();
        int processedBuffers;
        alGetSourcei(sourceHandle, AL_BUFFERS_PROCESSED, &processedBuffers);

        while(processedBuffers--)
        {
            unsigned int buffer;
            alSourceUnqueueBuffers(sourceHandle, 1, &buffer);
            if(alGetError()!=AL_NO_ERROR)
                qWarning("nSoundStreamer::update(...): Failed to unqueue buffer.");

            if(m_keepStreaming) m_keepStreaming = fillAndQueueBuffer(buffer);
        }
    }


}

void nSoundStreamer::rewind()
{
    if(!m_playlist->itemCount()) {
        return;
    }

    bool playing = m_source->state() == nSoundSource::SSS_PLAYING;
    m_source->stop();
    m_playlist->m_items[m_currentStream].m_soundStream->rewind();
    m_currentStream = 0;

    update(0);
    if(playing) m_source->play();

}

bool nSoundStreamer::fillAndQueueBuffer(unsigned int buffer)
{
    bool keep = true;

    nSoundBag * bag = m_bag;
    quint64 frames = bag->m_frames;
    quint64 readFrames = 0;
    int byteFactor = nSoundFormat_getFramesize(bag->m_format);

    do
    {
        nSoundStream * stream = m_playlist->m_items[m_currentStream].m_soundStream;

        readFrames += stream->read(m_bag->m_data+(readFrames*byteFactor), m_bag->m_frames - readFrames);

        if(readFrames!=frames)
        {
            stream->rewind();
            if( ! m_playlist->item(m_currentStream).m_loop)
            {
                m_currentStream++;
                if(m_currentStream==m_playlist->m_items.size())
                {
                    if(m_playlist->loopPlaylist())m_currentStream = 0;
                    else keep = false;
                }
            }
        }
    }while ( keep && (readFrames < frames));

    alGetError();
    alBufferData(buffer, openalFormat(m_bag->m_format), m_bag->m_data, readFrames*byteFactor, m_bag->m_frequency);
    if(alGetError()!=AL_NO_ERROR)
        qWarning("nSoundStreamer::fillAndQueueBuffer(...): Failed to refill buffer.");

    alSourceQueueBuffers(m_source->openalHandle(), 1, &buffer);
    if(alGetError()!=AL_NO_ERROR)
        qWarning("nSoundStreamer::fillAndQueueBuffer(...): Failed to queue buffer.");

    return keep;
}

int nSoundStreamer::openalFormat(nSoundFormat format)
{
    switch(format)
    {
    case SF_8BIT_MONO:
        return AL_FORMAT_MONO8;

    case SF_8BIT_STEREO:
        return AL_FORMAT_STEREO8;

    case SF_16BIT_MONO:
        return AL_FORMAT_MONO16;

    case SF_16BIT_STEREO:
        return AL_FORMAT_STEREO16;
    case SF_WAVE_HEADER:
    case SF_UNDEFINED:
        return -1;
    }

    return -1;
}


nSoundStreamerUpdater::nSoundStreamerUpdater(nSoundStreamer *parent) : QObject(0),
    _streamer(parent),
    _keepGoing(true)
{
    startTimer(static_cast<int>(NSS_BUFFER_SIZE / 44100.0 * 1000));
}

nSoundStreamerUpdater::~nSoundStreamerUpdater()
{

}

void nSoundStreamerUpdater::setup()
{
}

void nSoundStreamerUpdater::timerEvent(QTimerEvent * evt)
{
    Q_UNUSED(evt)

    if(_keepGoing)
        _streamer->update(0);
    else {
        deleteLater();
        QThread::currentThread()->deleteLater();
        QThread::currentThread()->exit(0);
    }
}
