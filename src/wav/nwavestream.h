#ifndef DWSOUNDRAWSTREAM_H
#define DWSOUNDRAWSTREAM_H

#include "../nSoundStream.h"

class QIODevice;

class nWaveStream : public nSoundStream
{
    Q_OBJECT
public:
    explicit nWaveStream(QIODevice * device, nSoundFormat format, int frequency, int channels = -1, QObject *parent = 0);

signals:

public slots:
    virtual quint64 frames() { return _totalFrames; }
    virtual int channels() { return _channels; }
    virtual int frequency() { return _frequency; }

    virtual nSoundBag * createSoundBag(QObject * parent = 0);

    virtual nSoundFormat format() { return _format; }
    virtual bool suggestStreaming() { return false; }

    virtual quint64 read(void* data, unsigned long frames);

    virtual void rewind();

private:

    QIODevice * _device;
    quint64 _totalFrames;
    int _channels;
    int _frequency;
    int _deviceOffset;
    nSoundFormat _format;

};

#endif // DWSOUNDRAWSTREAM_H
