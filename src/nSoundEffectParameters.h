#ifndef NSOUNDEFFECTPARAMETERS_H
#define NSOUNDEFFECTPARAMETERS_H

#include <QObject>
#include <QVariantMap>

enum nSoundEffectType
{
    SET_NONE,
    SET_REVERB,
    SET_HIGHPASS,
    SET_EAXREVERB,
    SET_CHORUS,
    SET_ECHO,
    SET_FLANGER,
    SET_AUTOWAH,
    SET_COMPRESSOR,
    SET_UNDEFINED
};

quint32 nSoundEffectType_toOpenalType();

class nSoundEffectParameters : public QObject
{
    Q_OBJECT
    Q_ENUMS(nSoundEffectType)
    Q_PROPERTY(nSoundEffectType type READ type WRITE setType )

public:
    nSoundEffectParameters(QObject * parent = 0);

    nSoundEffectType type();
    void setType(nSoundEffectType type);

    QVariant parameter(QString name);
    void setParameter(QString name, QVariant param);
    void clearParameters();

private:
    quint32 m_handle;
    nSoundEffectType m_type;
    QVariantMap m_parameters;

};

#endif // NSOUNDEFFECTPARAMETERS_H
