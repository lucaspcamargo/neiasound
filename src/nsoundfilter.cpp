#include "nsoundfilter.h"
#include "util/nEfxHelper.h"
#include <QVector3D>

nSoundFilter::PropertyMap nSoundFilter::ms_propertyMap;

nSoundFilter::nSoundFilter(QString name, nSoundFilterType type, QObject *parent) : QObject(parent)
{
    setObjectName(name);
    m_type = type;

    alGetError(); //reset error state
    nEfxHelper::alGenFilters(1, &m_handle);

    nEfxHelper::alFilteri(m_handle, AL_FILTER_TYPE, type);

    ALenum error = alGetError();
    if(error!=AL_NO_ERROR)
    {
        qWarning("nSoundFilter: alGenFilters() or alFilteri failed.");
    }
}

nSoundFilter::~nSoundFilter()
{
    nEfxHelper::alDeleteFilters(1, &m_handle);
}

void nSoundFilter::setProperties(const QVariantMap& properties)
{
    for (QVariantMap::const_iterator i = properties.constBegin(); i != properties.end(); ++i)
    {
        setProperty(i.key(), i.value());
    }
}

void nSoundFilter::setProperty(const QString &name, const QVariant &value)
{
    int code = getFilterPropertyCode(m_type, name);

    QVector3D buf;
    ALfloat v[3];

    switch (value.type()) {
    case QVariant::Int:
        nEfxHelper::alFilteri(m_handle, code, value.toInt());
        break;
    case QVariant::Double:
        nEfxHelper::alFilterf(m_handle, code, static_cast<ALfloat>(value.toDouble()));
        break;
    case QVariant::Vector3D:
        buf = value.value<QVector3D>();
        v[0] = static_cast<ALfloat>(buf.x());
        v[1] = static_cast<ALfloat>(buf.y());
        v[2] = static_cast<ALfloat>(buf.z());
        nEfxHelper::alFilterfv(m_handle, code, v);
        break;
    default:
        break;
    }

    ALenum error = alGetError();
    if(error!=AL_NO_ERROR)
    {
        qWarning("nSoundFilter: setting property failed:");
        qWarning(name.toLocal8Bit());
    }
}

int nSoundFilter::getFilterPropertyCode(nSoundFilterType filterType, QString property)
{
    if(!ms_propertyMap.count())
    {
        // init map

        QMap<QString, int> lowpass;
        lowpass.insert("gain", AL_LOWPASS_GAIN);
        lowpass.insert("gainhf", AL_LOWPASS_GAINHF);

        QMap<QString, int> highpass;
        lowpass.insert("gain", AL_HIGHPASS_GAIN);
        lowpass.insert("gainlf", AL_HIGHPASS_GAINLF);

        QMap<QString, int> bandpass;
        lowpass.insert("gain", AL_BANDPASS_GAIN);
        lowpass.insert("gainhf", AL_BANDPASS_GAINHF);
        lowpass.insert("gainlf", AL_BANDPASS_GAINLF);

        ms_propertyMap.insert(SFT_LOWPASS, lowpass);
        ms_propertyMap.insert(SFT_HIGHPASS, highpass);
        ms_propertyMap.insert(SFT_BANDPASS, bandpass);
    }

    if(!ms_propertyMap.contains(filterType)) return 0;

    return ms_propertyMap[filterType].value(property, 0);
}

