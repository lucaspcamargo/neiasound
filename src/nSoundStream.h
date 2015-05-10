#ifndef NSOUNDSTREAM_H
#define NSOUNDSTREAM_H

#include <QObject>
#include "nSoundFormat.h"

class nSoundBag;

class nSoundStream : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int channels READ channels)
    Q_PROPERTY(quint64 frames READ frames)
    Q_PROPERTY(int frequency READ frequency)
    Q_PROPERTY(nSoundFormat format READ format)
    Q_PROPERTY(bool suggestStreaming READ suggestStreaming)
public:
    explicit nSoundStream(QObject *parent = 0);

    virtual quint64 frames()=0;
    virtual int channels()=0;
    virtual int frequency()=0;

    virtual nSoundBag * createSoundBag(QObject * parent = 0);
    virtual nSoundFormat format() = 0;
    virtual bool suggestStreaming() = 0;

    virtual void rewind() = 0;
    virtual quint64 read(void* data, unsigned long frames) = 0;
signals:

public slots:

};

#endif // NSOUNDSTREAM_H
