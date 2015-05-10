#include "nSoundBuffer.h"
#include "nSoundSystem.h"
#include "nSoundBag.h"
#include "nSoundStream.h"
#include "AL/al.h"

nSoundBuffer::nSoundBuffer(QString name, nSoundSystem * parent) :
    QObject(parent)
{
    setObjectName(name);

    alGetError(); //reset error state

    unsigned int hnd;
    alGenBuffers(1, &hnd);
    m_handle = hnd;

    ALenum error = alGetError();
    if(error!=AL_NO_ERROR)
    {
        throw QString("nSoundBuffer: alGenBuffers() failed.");
    }
}

nSoundBuffer::~nSoundBuffer()
{
    alGetError();

    alDeleteBuffers(1, &m_handle);    

    if(alGetError()!=AL_NO_ERROR)
        throw QString("nSoundBuffer::~nSoundBuffer(): Error destroying openal buffer.");
}

void nSoundBuffer::setData(nSoundBag * bag)
{
    alGetError();

    alBufferData(m_handle, openalFormat(bag->m_format), bag->m_data, bag->m_data_size, bag->m_frequency);

    if(alGetError()!=AL_NO_ERROR)
        throw QString("nSoundBuffer::setData(): Error loading buffer data.");
}

void nSoundBuffer::setData(nSoundStream * stream)
{
    nSoundBag * tmp = stream->createSoundBag();
    setData(tmp);
    delete tmp;
}

int nSoundBuffer::openalFormat(nSoundFormat format)
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
    }

    return -1;
}
