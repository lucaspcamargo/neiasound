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
#include "qtaudiostream.h"

#include <QIODevice>
#include <QAudioDecoder>
#include <QCoreApplication>
#include <QThread>
#include <QtMath>

nQtAudioStream::nQtAudioStream(QIODevice * dev, QObject *parent) :
    nSoundStream(parent),
    _device(dev),
    _totalFrames(0),
    _channels(0),
    _frequency(0),
    _format(SF_16BIT_STEREO)
{
    _decoder = 0;
    _lastBufConsumedFrames = 0;

    if(!dev->isOpen())
        dev->open(QIODevice::ReadOnly);
    _deviceBuf.buffer() = dev->readAll();
    dev->close();

    _decoder = new QAudioDecoder(this);
    _decoder->setSourceDevice(&_deviceBuf);

    _decoder->start();
    QCoreApplication::processEvents();
    while( !_decoder->state() == QAudioDecoder::StoppedState && !_decoder->bufferAvailable()) { QThread::msleep(1); }
    _qtFormat = _decoder->audioFormat();
    if(_qtFormat.sampleType() != QAudioFormat::UnSignedInt || _qtFormat.sampleSize() != 16)
    {
        int a = _qtFormat.sampleType(), b = _qtFormat.sampleSize();
        QString codec = _qtFormat.codec();
        _qtFormat.setSampleType(QAudioFormat::UnSignedInt);
        _qtFormat.setSampleSize(16);
        _decoder->stop();
        _decoder->setAudioFormat(_qtFormat);
        _decoder->start();
    }

    _qtFormat = _decoder->audioFormat();

    _channels = _qtFormat.channelCount();
    _frequency = _qtFormat.sampleRate();
    _totalFrames = (_decoder->duration()) / _qtFormat.durationForFrames(1);

    qDebug(QString("[nQtAudioStream] sample type ").arg(_qtFormat.sampleType()).toLocal8Bit());

    switch (_channels) {
    case 1:
        _format = SF_16BIT_MONO;
        break;
    case 2:
        _format = SF_16BIT_STEREO;
        break;
    default:
        _format = SF_UNDEFINED;
        break;
    }


}

nQtAudioStream::~nQtAudioStream()
{
    _decoder->stop();
    delete _decoder;

}

quint64 nQtAudioStream::read(void *data, unsigned long frames)
{
    quint64 readFrames = 0;

    while (readFrames < frames)
    {
        bool finished = false;

        if( (!_lastBuf.isValid()) || _lastBufConsumedFrames >= _lastBuf.frameCount() )
        {
            // get a new buffer

            while(true)
            {
                if(_decoder->bufferAvailable())
                {
                    _lastBuf = _decoder->read();
                    _lastBufConsumedFrames = 0;
                    break;
                }
                else if(_decoder->state() == QAudioDecoder::StoppedState)
                {
                    finished = true;
                    break;
                }

                QThread::msleep(1);
            }

        }

        if(!finished)
        {
            int framesToConsume = qMin(frames - readFrames, (quint64) (_lastBuf.frameCount() - _lastBufConsumedFrames) );
            const char* src = ((const char*) _lastBuf.data()) + _qtFormat.bytesForFrames(_lastBufConsumedFrames);
            char* dst = ((char*) data) + _qtFormat.bytesForFrames(_lastBufConsumedFrames);

            _lastBufConsumedFrames += framesToConsume;
            readFrames += framesToConsume;
        }
    }

    return readFrames;
}

void nQtAudioStream::rewind()
{

}

