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

