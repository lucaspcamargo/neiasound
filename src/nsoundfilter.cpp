#include "nsoundfilter.h"
#include "util/nEfxHelper.h"

nSoundFilter::nSoundFilter(QString name, nSoundFilterType type, QObject *parent) : QObject(parent)
{
    setObjectName(name);

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

