#include "nSndfileStream.h"
#include "sndfile.h"
#include <cstdio>
#include <QDataStream>
#include "../nSoundBag.h"

// SF_VIRTUAL_IO handler functions

sf_count_t nSndfileStream_vio_filelen(void * userData)
{
    QIODevice * device = ((QIODevice*)userData);
    sf_count_t size = device->size();
    return size;
}

sf_count_t nSndfileStream_vio_seek(sf_count_t offset, int whence, void * userData)
{
    QIODevice * device = ((QIODevice*)userData);
    switch(whence)
    {
    case SEEK_SET:
        device->seek(offset);
        return 0;

    case SEEK_CUR:
        device->seek(device->pos()+offset);
        return 0;

    case SEEK_END:
        device->seek(device->size()-offset);
        return 0;
    }

    return -1;
}

sf_count_t nSndfileStream_vio_read(void * ptr, sf_count_t count, void * userData)
{
    QIODevice * device = ((QIODevice*)userData);
    return device->read((char*)ptr, count);
}

sf_count_t nSndfileStream_vio_write(const void * ptr, sf_count_t count, void * userData)
{
    // WRITING UNSUPPORTED
    return -1;
}

sf_count_t nSndfileStream_vio_tell(void * userData)
{
    QIODevice * device = ((QIODevice*)userData);
    sf_count_t pos = device->pos();
    return pos;
}






// ------------------------
// class nSndfileStream
// ------------------------

nSndfileStream::nSndfileStream(QString filename, QObject * parent)
    :nSoundStream(parent)
{
    m_iodevice = 0;
    m_ownsDevice = false;
    m_virtualio = 0;
    m_sndinfo = new SF_INFO();
    ((SF_INFO*)m_sndinfo)->format = 0;

    m_sndfile = sf_open(filename.toLocal8Bit(), SFM_READ, (SF_INFO*)m_sndinfo);
    if(!m_sndfile)
        throw QString("nSndfileStream::nSndfileStream(QString): Failed to open file: ")+filename;

    fillInfo();
}

nSndfileStream::nSndfileStream(QIODevice * device, QObject * parent, bool ownsDevice)
    :nSoundStream(parent), m_ownsDevice(ownsDevice)
{
    m_iodevice = device;
    if(!m_iodevice->isOpen())
        if(!m_iodevice->open(QIODevice::ReadOnly))
            throw QString("nSndfileStream::nSndfileStream(QIODevice*): Failed to open device for reading.");

    m_sndinfo = new SF_INFO();
    ((SF_INFO*)m_sndinfo)->format = 0;
    m_virtualio = new SF_VIRTUAL_IO();

    //setup function pointers
    SF_VIRTUAL_IO & vio = *((SF_VIRTUAL_IO*)m_virtualio);
    vio.get_filelen = nSndfileStream_vio_filelen;
    vio.read = nSndfileStream_vio_read;
    vio.write = nSndfileStream_vio_write;
    vio.seek = nSndfileStream_vio_seek;
    vio.tell = nSndfileStream_vio_tell;

    m_sndfile = sf_open_virtual((SF_VIRTUAL_IO*)m_virtualio, SFM_READ, (SF_INFO*)m_sndinfo, m_iodevice);

    if(!m_sndfile)
        throw QString("nSndfileStream::nSndfileStream(OgreStreamEtc...): Failed to open virtual stream.");

    fillInfo();
}

nSndfileStream::~nSndfileStream()
{
    if(m_virtualio) delete ((SF_VIRTUAL_IO*)m_virtualio);

    sf_close((SNDFILE*)m_sndfile);
    delete m_sndinfo;

    if(m_ownsDevice && m_iodevice)
        delete m_iodevice;
}

nSoundBag * nSndfileStream::createSoundBag(QObject * parent)
{
    nSoundBag * bag = new nSoundBag(format(), m_info_frames, m_info_samplerate, parent);
    read(bag->m_data, m_info_frames);
    return bag;
}

void nSndfileStream::fillInfo()
{
    m_info_frames = (((SF_INFO*)(m_sndinfo)))->frames;
    m_info_format = (((SF_INFO*)(m_sndinfo)))->format;
    m_info_samplerate = (((SF_INFO*)(m_sndinfo)))->samplerate;
    m_info_channels = (((SF_INFO*)(m_sndinfo)))->channels;
    // nLog::defaultLog(QString("nSndFileStream: Created new stream, %1 frames, %2hz, %3s, %4 channels.").arg(m_info_frames).arg(m_info_samplerate).arg(((double)m_info_frames)/m_info_samplerate).arg(m_info_channels), nLog::LL_WHOGIVESAFUCKANYWAY);

}

nSoundFormat nSndfileStream::format()
{
    if(m_info_channels==1)
        return SF_16BIT_MONO;

    if(m_info_channels==2)
        return SF_16BIT_STEREO;

    return SF_UNDEFINED;
}

bool nSndfileStream::suggestStreaming()
{
    if( ((m_info_format & SF_FORMAT_VORBIS) != 0) || (m_info_frames > m_info_samplerate*5))
        return true; //suggest stream if ogg vorbis or if larger than 5 seconds
}


void nSndfileStream::rewind()
{
    sf_seek((SNDFILE*)m_sndfile, 0, SEEK_SET);
}

quint64 nSndfileStream::read(void* data, unsigned long frames)
{
    return sf_readf_short((SNDFILE*)m_sndfile, (short*)data, frames);
}
