#ifndef NSOUNDFORMAT_H
#define NSOUNDFORMAT_H

#include <QtGlobal>

enum nSoundFormat
{
    SF_UNDEFINED,
    SF_8BIT_MONO,
    SF_8BIT_STEREO,
    SF_16BIT_MONO,
    SF_16BIT_STEREO,
    SF_WAVE_HEADER
};

inline quint64 nSoundFormat_getFramesize(nSoundFormat sf)
{
    switch(sf)
    {
    case SF_UNDEFINED:
        return 0;

    case SF_8BIT_MONO:
        return 1;

    case SF_8BIT_STEREO:
        return 2;

    case SF_16BIT_MONO:
        return 2;

    case SF_16BIT_STEREO:
        return 4;

    default:
        return 0;
    }
}


inline int nSoundFormat_getChannelCount(nSoundFormat sf)
{
    switch(sf)
    {
    case SF_UNDEFINED:
        return 0;

    case SF_8BIT_MONO:
    case SF_16BIT_MONO:
        return 1;

    case SF_8BIT_STEREO:
    case SF_16BIT_STEREO:
        return 2;

    default:
        return 0;
    }
}
#endif // NSOUNDFORMAT_H
