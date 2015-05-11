// Copyright (C) 2015 Lucas Pires Camargo
// 
// This file is part of neiasound - Qt-style OpenAL wrapper for games.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// 
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE FREEBSD PROJECT ``AS IS'' AND ANY EXPRESS
// OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
// NO EVENT SHALL THE FREEBSD PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#ifndef NSOUNDSTREAMERPLAYLIST_H
#define NSOUNDSTREAMERPLAYLIST_H

#include "neiasound_global.h"
#include <QObject>
#include <QList>

class nSoundStream;

typedef struct NEIASOUNDSHARED_EXPORT _nSoundStreamerItem
{
    nSoundStream * m_soundStream;
    bool m_loop;
} nSoundStreamerItem;

class NEIASOUNDSHARED_EXPORT nSoundStreamerPlaylist : public QObject
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
    bool m_loopPlaylist;
    QList<nSoundStreamerItem> m_items;

};

#endif // NSOUNDSTREAMERPLAYLIST_H
