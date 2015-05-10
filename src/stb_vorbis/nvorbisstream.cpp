#include "nvorbisstream.h"

#include <QIODevice>
#include "../nSoundBag.h"


#define STB_VORBIS_MAX_CHANNELS     2
#include "stb_vorbis.c"


nVorbisStream::nVorbisStream(QIODevice * dev, QObject *parent) : nSoundStream(parent),
    _device(dev),
    m_error(false),
    _totalFrames(0),
    _channels(0),
    _frequency(0),
    _format(SF_16BIT_STEREO)
{
    _vorbis = 0;

    if(!dev->isOpen())
        dev->open(QIODevice::ReadOnly);

    _qtBuf = dev->readAll();

    dev->close();

    if(_qtBuf.size())
    {
        _bufSize = _qtBuf.size();
        _buf = _qtBuf.data();


        int err = VORBIS__no_error;
        _vorbis = stb_vorbis_open_memory((unsigned char*)_buf, _bufSize, &err,  0 );
        if(!_vorbis || err != VORBIS__no_error)
        {
            qDebug(QStringLiteral("[nVorbisStream] Error initializing vorbis stream: %1").arg(err).toLocal8Bit());
        }

        stb_vorbis_info info = stb_vorbis_get_info(_vorbis);

        _totalFrames = stb_vorbis_stream_length_in_samples(_vorbis);
        _channels = _vorbis->channels;
        _frequency = _vorbis->sample_rate;

        switch (channels()) {
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
    else
    {
        qDebug("[nVorbisStream] Error reading QIODevice");
    }

}

nVorbisStream::~nVorbisStream()
{
    stb_vorbis_close(_vorbis);
    _qtBuf.clear();
}

void nVorbisStream::rewind()
{
    if(_vorbis) stb_vorbis_seek_start(_vorbis);
}

quint64 nVorbisStream::read(void *data, unsigned long frames)
{
    if(!frames) return 0;
    return stb_vorbis_get_samples_short_interleaved(_vorbis, _channels, (short*) data, frames * _channels );

}

