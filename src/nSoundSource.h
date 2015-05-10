#ifndef NSOUNDSOURCE_H
#define NSOUNDSOURCE_H

#include <QObject>
#include <QVector3D>
#include "nSoundSourceRole.h"

class nSoundSystem;
class nSoundBuffer;


class nSoundSource : public QObject
{
    Q_OBJECT
    Q_ENUMS(nSoundSourceState)
    Q_ENUMS(nSoundSourceRole)

    Q_PROPERTY(unsigned int openalHandle READ openalHandle)
    Q_PROPERTY(nSoundSourceRole role READ role)
    Q_PROPERTY(nSoundSourceState state READ state)
    Q_PROPERTY(qreal gain READ gain WRITE setGain)
    Q_PROPERTY(qreal pitch READ pitch WRITE setPitch)
    Q_PROPERTY(qreal rolloffFactor READ rolloffFactor WRITE setRolloffFactor)
    Q_PROPERTY(bool loop READ loop WRITE setLoop)
    Q_PROPERTY(QVector3D position READ position WRITE setPosition)
    Q_PROPERTY(bool destroyAfterStopped READ destroyAfterStopped WRITE setDestroyAfterStopped NOTIFY onDestroyAfterStoppedChanged )

    Q_PROPERTY(qreal fading READ isFading)
    Q_PROPERTY(qreal fadeTarget READ fadeTarget)
    Q_PROPERTY(qreal fadeDeltaPerSecond READ fadeDeltaPerSecond)


public:
    enum nSoundSourceState
    {
        SSS_UNKNOWN,
        SSS_PLAYING,
        SSS_PAUSED,
        SSS_STOPPED,
        SSS_INITIAL
    };

    explicit nSoundSource(QString name, nSoundSourceRole role, nSoundSystem * parent);
    virtual ~nSoundSource();

    unsigned int openalHandle(){return m_handle;}
    nSoundSourceState state();
    nSoundSourceRole role(){return m_role;}
    qreal gain();
    qreal pitch();
    qreal rolloffFactor();
    bool loop();
    QVector3D position();

    bool isFading() const {return m_fading;}
    qreal fadeTarget() const {return m_fadeTarget;}
    qreal fadeDeltaPerSecond() const {return m_fadeDeltaPerSecond;}

    // GAIN CFG SUPPORT
    static qreal _roleGainVolume(nSoundSourceRole role);
    static void _resetRoleGains();

    bool destroyAfterStopped() const
    {
        return m_destroyAfterStopped;
    }

signals:

    void onDestroyAfterStoppedChanged(bool arg);

public slots:
    bool update(qreal frameTime);

    void attachBuffer(nSoundBuffer * buffer);

    void setGain(qreal);
    void setPitch(qreal);
    void setRolloffFactor(qreal);
    void setLoop(bool);
    void setPosition(QVector3D pos);

    void fade(qreal to, qreal duration, qreal from = -1.0f);

    void play();
    void pause();
    void stop();
    void rewind();

    void setDestroyAfterStopped(bool arg)
    {
        if (m_destroyAfterStopped == arg)
            return;

        m_destroyAfterStopped = arg;
        emit onDestroyAfterStoppedChanged(arg);
    }

private:
    unsigned int m_handle;
    nSoundSourceRole m_role;

    qreal m_gainMirror;

    bool m_fading;
    qreal m_fadeTarget;
    qreal m_fadeDeltaPerSecond;

    //GAIN CFG SUPPORT
    static bool _m_gainsInitialized;
    static qreal _m_musicGain;
    static qreal _m_sfxGain;
    static qreal _m_voiceGain;
    static qreal _m_ambienceGain;

    bool m_destroyAfterStopped;
};

#endif // NSOUNDSOURCE_H
