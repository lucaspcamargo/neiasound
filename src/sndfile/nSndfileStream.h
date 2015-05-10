#ifndef NSNDFILESTREAM_H
#define NSNDFILESTREAM_H

#include "../nSoundStream.h"
#include "../nSoundFormat.h"

class QIODevice;
class nSoundBag;

class nSndfileStream : public nSoundStream
{
public:
    nSndfileStream(QString filename, QObject * parent = 0);
    nSndfileStream(QIODevice * stream, QObject * parent = 0, bool ownsDevice = true);
    virtual ~nSndfileStream();

    quint64 frames(){return m_info_frames;}
    int channels(){return m_info_channels;}
    int frequency(){return m_info_samplerate;}

    nSoundBag * createSoundBag(QObject * parent = 0);

    nSoundFormat format();
    bool suggestStreaming();

    void rewind();
    quint64 read(void* data, unsigned long frames);

private:
    void fillInfo();

    QIODevice * m_iodevice;
    bool m_ownsDevice;
    void * m_virtualio;

    void * m_sndfile;
    void * m_sndinfo;

    int m_info_format;
    quint64 m_info_frames;
    int m_info_samplerate;
    int m_info_channels;
};

#endif // NSNDFILESTREAM_H
