#include "nSoundSource.h"
#include "nSoundSystem.h"
#include "nSoundBuffer.h"

#include "AL/al.h"


nSoundSource::nSoundSource(QString name, nSoundSourceRole role, nSoundSystem * parent) :
    QObject(parent)
{
    setObjectName(name);
    m_role = role;
    m_gainMirror = _roleGainVolume(role);
    m_fading = false;
    m_fadeTarget = 0.0f;
    m_fadeDeltaPerSecond = 0.0f;
    m_destroyAfterStopped = false;

    alGetError(); //reset error state

    unsigned int hnd;
    alGenSources(1, &hnd);
    m_handle = hnd;

    ALenum error = alGetError();
    if(error!=AL_NO_ERROR)
    {
        throw QString("nSoundSource: alGenSources() failed.");
    }

    alSourcef(m_handle, AL_GAIN, m_gainMirror);
    alSource3f(m_handle, AL_POSITION, 0.0f ,0.0f ,0.0f);
    alSource3f(m_handle, AL_VELOCITY, 0.0f ,0.0f ,0.0f);
    alSource3f(m_handle, AL_DIRECTION, 0.0f, 0.0f, 0.0f);
    if(error!=AL_NO_ERROR)
        throw QString("nSoundSource: Setting of source properties failed.");

}

nSoundSource::~nSoundSource()
{
    alDeleteSources(1, &m_handle);
}

// PROPERTIES

nSoundSource::nSoundSourceState nSoundSource::state()
{
    alGetError();

    int state;
    alGetSourcei(m_handle, AL_SOURCE_STATE, &state);

    switch (state)
    {
    case AL_INITIAL:
        return SSS_INITIAL;

    case AL_PLAYING:
        return SSS_PLAYING;

    case AL_PAUSED:
        return SSS_PAUSED;

    case AL_STOPPED:
        return SSS_STOPPED;
    }

    return SSS_UNKNOWN;

}

qreal nSoundSource::gain()
{
    return m_gainMirror;
}

void nSoundSource::setGain(qreal f)
{
    if(f<0.0f)
        f=0.0f;
    alSourcef(m_handle, AL_GAIN, f);    
    m_gainMirror = f;
}


qreal nSoundSource::pitch()
{
    ALfloat pitch;
    alGetSourcef(m_handle, AL_PITCH, &pitch);
    return pitch;
}

void nSoundSource::setPitch(qreal f)
{
    if(f<0.0f)
        f=0.0f;
    alSourcef(m_handle, AL_PITCH, f);
}


qreal nSoundSource::rolloffFactor()
{
    ALfloat rolloff;
    alGetSourcef(m_handle, AL_ROLLOFF_FACTOR, &rolloff);
    return rolloff;
}

void nSoundSource::setRolloffFactor(qreal f)
{
    if(f<0.0f)
        f=0.0f;
    alSourcef(m_handle, AL_ROLLOFF_FACTOR, f);
}


bool nSoundSource::loop()
{
    ALint loop;
    alGetSourcei(m_handle, AL_LOOPING, &loop);
    return (loop?true:false);
}

void nSoundSource::setLoop(bool b)
{
    alSourcei(m_handle, AL_LOOPING, (b?AL_TRUE:AL_FALSE));
}

QVector3D nSoundSource::position()
{
    ALfloat x,y,z;
    alGetSource3f(m_handle, AL_POSITION, &x, &y, &z);
    return QVector3D(x,y,z);
}

void nSoundSource::setPosition(QVector3D pos)
{
    alSource3f(m_handle, AL_POSITION, pos.x(), pos.y(), pos.z() );
}


// METHODS

bool nSoundSource::update(qreal frameTime)
{
    if(m_fading)
    {
        qreal newGain = m_gainMirror + frameTime*m_fadeDeltaPerSecond;
        if( ((m_fadeDeltaPerSecond < 0.0f)&&(newGain<m_fadeTarget)) ||
            ((m_fadeDeltaPerSecond > 0.0f)&&(newGain>m_fadeTarget)) )
        {
            newGain = m_fadeTarget;
            m_fading = false;
        }
        setGain(newGain);
    }if(destroyAfterStopped() && state() == SSS_STOPPED)
    {
        return false;
    }

    return true;
}

void nSoundSource::fade(qreal to, qreal duration, qreal from)
{
    if(to==from)
    {
        setGain(to);
        m_fading = false;
        return;
    }

    qreal newGain;
    if(from >= 0.0f)
    {
        setGain(from);
        newGain = from;
    }else newGain = gain();

    m_fadeTarget = to;
    m_fadeDeltaPerSecond = (to - newGain)/duration;
    m_fading = true;

}

void nSoundSource::attachBuffer(nSoundBuffer * buffer)
{
    if(!buffer) return;
    alGetError();

    ALint sourceType;
    alGetSourcei(m_handle, AL_SOURCE_TYPE, &sourceType);
    if(sourceType == AL_STREAMING)
        throw QString("nSoundSource: Tried to play a single buffer on a streaming source.");

    alSourcei(m_handle, AL_BUFFER, buffer->openalHandle());

    ALenum err = alGetError();
    if(err!=AL_NO_ERROR)
        throw QString("nSoundSource: failed to bind buffer \"")+buffer->objectName()+
                QString("\" to source \"")+objectName()+QString("\".");
}

void nSoundSource::play()
{
    alGetError();
    alSourcePlay(m_handle);
    if(alGetError()!=AL_NO_ERROR)
        throw QString("nSoundSource: Failed to play source.");
}

void nSoundSource::pause()
{
    alGetError();
    alSourcePause(m_handle);
    if(alGetError()!=AL_NO_ERROR)
        throw QString("nSoundSource: Failed to pause source.");
}

void nSoundSource::stop()
{
    alGetError();
    alSourceStop(m_handle);
    if(alGetError()!=AL_NO_ERROR)
        throw QString("nSoundSource: Failed to stop source.");
}

void nSoundSource::rewind()
{
    alGetError();
    alSourceRewind(m_handle);
    if(alGetError()!=AL_NO_ERROR)
        throw QString("nSoundSource: Failed to rewind source.");
}

// GAIN CFG SUPPORT

bool nSoundSource::_m_gainsInitialized = false;
qreal nSoundSource::_m_musicGain = 0.0f;
qreal nSoundSource::_m_sfxGain = 0.0f;
qreal nSoundSource::_m_voiceGain = 0.0f;
qreal nSoundSource::_m_ambienceGain = 0.0f;

#include <QSettings>

void nSoundSource::_resetRoleGains()
{
    QSettings settings;
    settings.beginGroup("Audio");
    _m_musicGain = settings.value("MusicGain", 60).toFloat()/100.0f;
    _m_sfxGain = settings.value("SfxGain", 90).toFloat()/100.0f;
    _m_voiceGain = settings.value("VoiceGain", 70).toFloat()/100.0f;
    _m_ambienceGain = settings.value("AmbienceGain", 60).toFloat()/100.0f;
    _m_gainsInitialized = true;
    settings.endGroup();

    _m_gainsInitialized = true;
}

qreal nSoundSource::_roleGainVolume(nSoundSourceRole role)
{
    if(!_m_gainsInitialized)
        nSoundSource::_resetRoleGains();

    switch(role)
    {
    case SSR_MUSIC:
        return _m_musicGain;
    case SSR_SFX:
        return _m_sfxGain;
    case SSR_AMBIENCE:
        return _m_ambienceGain;
    case SSR_VOICE:
        return _m_voiceGain;
    default:
        return 1.0f;
    }

}

