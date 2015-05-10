#include "nSoundStreamerPlaylist.h"

nSoundStreamerPlaylist::nSoundStreamerPlaylist(QObject *parent) :
    QObject(parent),
    m_loopPlaylist(false),
    m_items(QList<nSoundStreamerItem>())
{
}

nSoundStreamerPlaylist::~nSoundStreamerPlaylist()
{
}

void nSoundStreamerPlaylist::createItem(nSoundStream * stream, bool loop)
{
    qDebug("Item");

    if(!stream)
    {
        qWarning("Attempted to create nSoundStreamerPlaylist item with null stream pointer.");
        return;
    }

    nSoundStreamerItem item;
    item.m_soundStream = stream;
    item.m_loop = loop;
    m_items.append(item);

}

nSoundStreamerItem nSoundStreamerPlaylist::item(int index)
{
    return m_items[index];
}

void nSoundStreamerPlaylist::clearItems()
{
    m_items.clear();
}
