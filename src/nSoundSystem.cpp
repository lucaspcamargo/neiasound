#include "nSoundSystem.h"
#include "nSoundSource.h"
#include "nSoundBuffer.h"
#include "nSoundListener.h"
#include "nSoundStreamer.h"
#include "nSoundStreamerPlaylist.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#include "util/nEfxHelper.h"
#include <QSettings>
#include <QUuid>

nSoundSystem::nSoundSystem(QObject *parent) :
    QObject(parent), m_success(false)
{
    qDebug("nSoundSystem initializing.");

    m_context = 0;
    m_device = 0;

    m_device = alcOpenDevice(0); //default device
    if(!m_device)
        throw QString("Error creating OpenAL device.");

    int attributes[] = {ALC_FREQUENCY, 44100, 0};
    m_context = alcCreateContext(m_device, attributes);
    if(!m_context)
        throw QString("Error creating OpenAL context.");

    alcMakeContextCurrent(m_context);
    if( alGetError()!=AL_NO_ERROR)
        qWarning("nSoundSystem::nSoundSystem(): Failed to set context as current.");

    if(!nEfxHelper::initialize(m_device))
        qWarning("nSoundSystem::nSoundSystem(): nEfxHelper initialization failed.");

    m_listener = new nSoundListener(this);
    m_success = true;

#ifdef ANDROID
#ifdef MOB
    alcDeviceEnableHrtfMOB( m_device, false );
#endif
#endif

    qDebug("nSoundSystem initialized successfully.");

    alcGetIntegerv(m_device, ALC_MAX_AUXILIARY_SENDS, 1, &m_numSends);
    qDebug(QString("OpenAL device supports %1 auxiliary sends per source.").arg(m_numSends).toLocal8Bit());

    nSoundSource::_resetRoleGains();
    QSettings settings;
    settings.beginGroup("Audio");
    setMasterGain(settings.value("MasterGain", 100).toFloat()/100.0f);
    settings.endGroup();

}

nSoundSystem::~nSoundSystem()
{
    // ensure deletion of resources in right order
    QHash<QString, nSoundSource*> sources = m_sources;
    QHash<QString, nSoundBuffer*> buffers = m_buffers;
    QHash<QString, nSoundStreamer*> streamers = m_streamers;

    foreach(nSoundStreamer * streamer, streamers)
    {
        destroyStreamer(streamer);
    }

    foreach(nSoundSource * source, sources)
    {
        destroySource(source);
    }

    foreach(nSoundBuffer * buffer, buffers)
    {
        destroyBuffer(buffer);
    }


    alcMakeContextCurrent(0);
    alcDestroyContext(m_context);
    alcCloseDevice(m_device);
}

void nSoundSystem::update(qreal frameTime)
{

    m_listener->update(frameTime);

//    Not necessary anymore, streamers update themselves in owned threads.

//    foreach(nSoundStreamer * streamer, m_streamers)
//    {
//        streamer->update(frameTime);
//    }

    QList<nSoundSource*> toDestroy;
    foreach(nSoundSource * source, m_sources)
    {
        if(!source->update(frameTime))
            toDestroy.append(source);
    }

    foreach (nSoundSource * source, toDestroy) {
        destroySource(source);
    }
}

// SOUND STUFF METHODS

qreal nSoundSystem::masterGain()
{
    float ret;
    alGetListenerf(AL_GAIN, &ret);
    return ret;
}

void nSoundSystem::setMasterGain(qreal gain)
{
    alListenerf(AL_GAIN, gain);

    emit masterGainChanged(gain);
}

// SOURCE METHODS

nSoundSource * nSoundSystem::createSource(QString name, nSoundSourceRole role)
{
    if(name.isEmpty())
    {
        name = QString("nSoundSource_%1").arg(QUuid::createUuid().toString());
    }

    nSoundSource * src;
    try{
        src = new nSoundSource(name, role, this);
        m_sources.insert(name, src);
        return src;
    }catch(...)
    {
        return 0;
    }
}

nSoundSource * nSoundSystem::source(QString name)
{
    return m_sources.value(name, 0);
}

bool nSoundSystem::destroySource(QString name)
{
    nSoundSource * src = m_sources.value(name, 0);
    if(!src) return false;
    m_sources.remove(name);
    delete src;
    return true;
}

bool nSoundSystem::destroySource(nSoundSource * source)
{
    nSoundSource * src = m_sources.value(source->objectName(), 0);
    if(!src) return false;
    m_sources.remove(source->objectName());
    delete src;
    return true;
}


// BUFFER METHODS

nSoundBuffer * nSoundSystem::createBuffer(QString name)
{
    if(name.isEmpty()) qWarning("Creating nSoundBuffer with empty name.");

    nSoundBuffer * buf = new nSoundBuffer(name, this);
    m_buffers.insert(name, buf);
    return buf;
}

nSoundBuffer * nSoundSystem::buffer(QString name)
{
    return m_buffers.value(name, 0);
}

bool nSoundSystem::destroyBuffer(QString name)
{
    nSoundBuffer * buf = m_buffers.value(name, 0);
    if(!buf) return false;
    m_buffers.remove(name);
    delete buf;
    return true;
}

bool nSoundSystem::destroyBuffer(nSoundBuffer * buffer)
{
    nSoundBuffer * buf = m_buffers.value(buffer->objectName(), 0);
    if(!buf) return false;
    m_buffers.remove(buffer->objectName());
    delete buf;
    return true;
}


// STREAMER METHODS

nSoundStreamer * nSoundSystem::createStreamer(QString name, nSoundSource * source, nSoundStreamerPlaylist * playlist)
{
    if(name.isEmpty()) qWarning("Creating nSoundStreamer with an empty name.");

    nSoundStreamer * streamer = new nSoundStreamer(name, source, playlist, this);
    playlist->setParent(streamer);
    m_streamers.insert(name, streamer);
    return streamer;
}

nSoundStreamerPlaylist *nSoundSystem::createStreamerPlaylist(QObject *parent)
{
    return new nSoundStreamerPlaylist(parent);
}

nSoundStreamer * nSoundSystem::streamer(QString name)
{
    return m_streamers.value(name, 0);
}

bool nSoundSystem::destroyStreamer(QString name)
{
    nSoundStreamer * streamer = m_streamers.value(name, 0);
    if(!streamer) return false;
    m_streamers.remove(name);
    delete streamer;
    return true;
}

bool nSoundSystem::destroyStreamer(nSoundStreamer * streamer)
{
    nSoundStreamer * strm = m_streamers.value(streamer->objectName(), 0);
    if(!strm) return false;
    m_streamers.remove(strm->objectName());
    delete strm;
    return true;
}
