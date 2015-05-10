#ifndef NSOUNDBAG_H
#define NSOUNDBAG_H

#include <QObject>
#include "nSoundFormat.h"


class nSoundBag : public QObject
{
    Q_OBJECT
    Q_ENUMS(nSoundFormat)
public:
    nSoundBag(nSoundFormat format, quint64 frames, int frequency, QObject * parent = 0);
    virtual ~nSoundBag();

    quint64 m_frames;
    uchar * m_data;
    quint64 m_data_size;
    int m_frequency;
    nSoundFormat m_format;

signals:

public slots:

private:

};

#endif // NSOUNDBAG_H
