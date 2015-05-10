#ifndef NSOUNDSTREAMERPLAYLIST_H
#define NSOUNDSTREAMERPLAYLIST_H

#include <QObject>
#include <QList>

class nSoundStream;

typedef struct _nSoundStreamerItem
{
    nSoundStream * m_soundStream = 0;
    bool m_loop;
} nSoundStreamerItem;

class nSoundStreamerPlaylist : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool loopPlaylist READ loopPlaylist WRITE setLoopPlaylist) 
public:
    explicit nSoundStreamerPlaylist(QObject *parent = 0);
    virtual ~nSoundStreamerPlaylist();
    nSoundStreamerItem item(int index);

    bool loopPlaylist(){return m_loopPlaylist;}
    void setLoopPlaylist(bool b){m_loopPlaylist = b;}

signals:

public slots:
    void createItem(nSoundStream * stream, bool loop);
    int itemCount() { return m_items.count(); }
    void clearItems();

private:
    friend class nSoundStreamer;
    QList<nSoundStreamerItem> m_items;
    bool m_loopPlaylist;

};

#endif // NSOUNDSTREAMERPLAYLIST_H
