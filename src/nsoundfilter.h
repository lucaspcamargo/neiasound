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

    static int getFilterPropertyCode(nSoundFilterType filterType, QString property);

signals:


public slots:
    void setProperties(const QVariantMap &properties);
    void setProperty(const QString &name, const QVariant &value);


private:
    unsigned int m_handle;

    nSoundFilterType m_type;

    typedef QMap<nSoundFilterType, QMap<QString, int> > PropertyMap;

    static PropertyMap ms_propertyMap;
};

#endif // NSOUNDFILTER_H
