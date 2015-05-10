#ifndef NVORBISSTREAM_H
#define NVORBISSTREAM_H

#include "../nSoundStream.h"

class QIODevice;
class stb_vorbis;

class nVorbisStream : public nSoundStream
{
    Q_OBJECT    
    Q_PROPERTY(bool error READ error CONSTANT)
public:
    explicit nVorbisStream(QIODevice * device, QObject *parent = 0);
    ~nVorbisStream();



    bool error() const
    {
        return m_error;
    }

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
    QByteArray _qtBuf;
    char * _buf;
    int _bufSize;

    stb_vorbis * _vorbis;

    quint64 _totalFrames;
    int _channels;
    int _frequency;
    nSoundFormat _format;


    bool m_error;
};

#endif // NVORBISSTREAM_H
