#ifndef NSOUNDBUFFER_H
#define NSOUNDBUFFER_H

#include <QObject>
#include "nSoundFormat.h"

class nSoundSystem;
class nSoundBag;
class nSoundStream;

class nSoundBuffer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(unsigned int openalHandle READ openalHandle)
public:
    explicit nSoundBuffer(QString name, nSoundSystem * parent);
    virtual ~nSoundBuffer();

    unsigned int openalHandle(){return m_handle;}

    void setData(nSoundBag * bag);
    void setData(nSoundStream * stream);

    int openalFormat(nSoundFormat format);

signals:

public slots:

private:
    unsigned int m_handle;
};

#endif // NSOUNDBUFFER_H
