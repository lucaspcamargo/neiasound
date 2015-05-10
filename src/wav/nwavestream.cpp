#include "nwavestream.h"
#include "../nSoundBag.h"
#include <QIODevice>


nWaveStream::nWaveStream(QIODevice *device, nSoundFormat format, int frequency, int channels, QObject *parent)
    : nSoundStream(parent),
      _device(device),
      _format(format),
      _frequency(frequency),
      _channels(channels)
{

    if(format == SF_WAVE_HEADER)
    {

        unsigned char header[44];

        device->read(reinterpret_cast<char*>(header), 44);
        _channels = header[22];
        _frequency = header[24] + (((int)header[25])<<8) + (((int)header[26])<<16) + (((int)header[27])<<24);
        _format = (_channels == 1?
                       (header[34]==16? SF_16BIT_MONO : SF_8BIT_MONO):
                       (header[34]==16? SF_16BIT_STEREO : SF_8BIT_STEREO)
                       );
        int chunkSize = header[40] + (((int)header[41])<<8) + (((int)header[42])<<16) + (((int)header[43])<<24);
        _totalFrames = chunkSize / nSoundFormat_getFramesize(_format);
    }
    else
    {
        if(channels < 0) channels = (format == SF_8BIT_STEREO || format  == SF_16BIT_STEREO)? 2 : 1;
    }
}

nSoundBag *nWaveStream::createSoundBag(QObject *parent)
{
    nSoundBag * bag = new nSoundBag( _format, _totalFrames, _frequency );
    read(bag->m_data, _totalFrames);
    return bag;
}


void nWaveStream::rewind()
{
    _device->reset();
}

quint64 nWaveStream::read(void *data, unsigned long frames)
{
    return _device->read( (char *) data, nSoundFormat_getFramesize(_format) * frames ) / nSoundFormat_getFramesize(_format);
}
