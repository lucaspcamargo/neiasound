#ifndef NSOUNDFILTER_H
#define NSOUNDFILTER_H

#include "neiasound_global.h"
#include "nsoundenums.h"

#include <QObject>
#include <QVariantMap>


class NEIASOUNDSHARED_EXPORT nSoundFilter : public QObject
{
    Q_OBJECT
    Q_ENUMS(FilterType)
    Q_PROPERTY(unsigned int openalHandle READ openalHandle CONSTANT)
    Q_PROPERTY(nSoundFilterType type READ type CONSTANT)

public:
    explicit nSoundFilter(QString name, nSoundFilterType type, QObject *parent = 0);
    virtual ~nSoundFilter();


    unsigned int openalHandle(){return m_handle;}
    nSoundFilterType type() const
    {
        return m_type;
    }

signals:


public slots:


private:
    unsigned int m_handle;

    nSoundFilterType m_type;
};

#endif // NSOUNDFILTER_H
