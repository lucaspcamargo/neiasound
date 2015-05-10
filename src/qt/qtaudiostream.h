#ifndef QTAUDIOSTREAM_H
#define QTAUDIOSTREAM_H

#include "../nSoundStream.h"
#include <QAudioFormat>
#include <QAudioBuffer>
#include <QBuffer>


class QIODevice;
class QAudioDecoder;

class nQtAudioStream : public nSoundStream
{
    Q_OBJECT
public:
    explicit nQtAudioStream(QIODevice * dev, QObject *parent = 0);
    ~nQtAudioStream();

signals:

public slots:
    virtual quint64 frames() { return _totalFrames; }
    virtual int channels() { return _channels; }
    virtual int frequency() { return _frequency; }

    virtual nSoundFormat format() { return _format; }
    virtual bool suggestStreaming() { return _totalFrames > 88200; }

    virtual quint64 read(void* data, unsigned long frames);
    virtual void rewind();


private:
    QIODevice * _device;
    QBuffer _deviceBuf;

    QAudioDecoder * _decoder;
    QAudioFormat _qtFormat;
    QAudioBuffer _lastBuf;
    int _lastBufConsumedFrames;



    quint64 _totalFrames;
    int _channels;
    int _frequency;
    nSoundFormat _format;
};

#endif // QTAUDIOSTREAM_H
