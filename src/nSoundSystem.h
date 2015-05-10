#ifndef NSOUNDSYSTEM_H
#define NSOUNDSYSTEM_H

#include <QObject>
#include <QHash>
#include "nSoundSourceRole.h"
#include "nSoundFormat.h"

// *sigh* ugly but necessary
struct ALCcontext_struct;
struct ALCdevice_struct;
typedef struct ALCcontext_struct ALCcontext;
typedef struct ALCdevice_struct ALCdevice;

class nSoundSource;
class nSoundBuffer;
class nSoundListener;
class nSoundStreamer;
class nSoundStreamerPlaylist;

class nSoundSystem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int supportedAuxiliarySends READ supportedAuxiliarySends)
    Q_PROPERTY(qreal masterGain READ masterGain WRITE setMasterGain NOTIFY masterGainChanged)
    Q_PROPERTY(nSoundListener * listener READ listener)
    Q_PROPERTY(ALCcontext * openalContext READ openalContext)
    Q_PROPERTY(ALCdevice * openalDevice READ openalDevice)
    Q_ENUMS(nSoundFormat)
    Q_ENUMS(nSoundSourceRole)
public:
    explicit nSoundSystem(QObject *parent = 0);
    virtual ~nSoundSystem();

    ALCcontext * openalContext(){return m_context;}
    ALCdevice * openalDevice(){return m_device;}

    nSoundListener * listener(){return m_listener;}

    qreal masterGain();

    int supportedAuxiliarySends(){return m_numSends;}

signals:

    void masterGainChanged(qreal arg);

public slots:

    void update(qreal);

    void setMasterGain(qreal gain);

    nSoundSource * createSource(QString name = "", nSoundSourceRole = SSR_SFX);
    nSoundSource * source(QString name);
    bool destroySource(QString name);
    bool destroySource(nSoundSource * source);

    nSoundBuffer * createBuffer(QString name);
    nSoundBuffer * buffer(QString name);
    bool destroyBuffer(QString name);
    bool destroyBuffer(nSoundBuffer * buffer);


    nSoundStreamer * createStreamer(QString name, nSoundSource * source, nSoundStreamerPlaylist * playlist = 0);
    nSoundStreamerPlaylist * createStreamerPlaylist(QObject * parent);
    nSoundStreamer * streamer(QString name);
    bool destroyStreamer(QString name);
    bool destroyStreamer(nSoundStreamer * streamer);

private:
    QHash<QString, nSoundSource*> m_sources;
    QHash<QString, nSoundBuffer*> m_buffers;
    QHash<QString, nSoundStreamer*> m_streamers;

    nSoundListener * m_listener;

    bool m_success;
    int m_numSends;
    ALCcontext * m_context;
    ALCdevice * m_device;
    qreal m_masterGain;
};

#endif // NSOUNDSYSTEM_H
