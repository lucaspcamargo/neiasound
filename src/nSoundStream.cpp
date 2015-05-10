#include "nSoundStream.h"
#include "nSoundBag.h"

nSoundStream::nSoundStream(QObject *parent) :
    QObject(parent)
{
}

nSoundBag *nSoundStream::createSoundBag(QObject *parent)
{
    nSoundBag * bag = new nSoundBag( format(), frames(), frequency() );
    read(bag->m_data, frames());
    return bag;
}
