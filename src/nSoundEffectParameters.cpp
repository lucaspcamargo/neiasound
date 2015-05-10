#include "nSoundEffectParameters.h"
#include "AL/al.h"
#include "util/nEfxHelper.h"


nSoundEffectParameters::nSoundEffectParameters(QObject * parent)
    : QObject(parent)
{

}

nSoundEffectType nSoundEffectParameters::type()
{
    return m_type;
}

void nSoundEffectParameters::setType(nSoundEffectType type)
{
    m_type=type;
}

QVariant nSoundEffectParameters::parameter(QString name)
{
    return m_parameters.value(name);
}

void nSoundEffectParameters::setParameter(QString name, QVariant param)
{

}

void nSoundEffectParameters::clearParameters()
{
    m_parameters.clear();
}
