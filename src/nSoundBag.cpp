#include "nSoundBag.h"
#include <cstdio>
#include <cstdlib>

nSoundBag::nSoundBag(nSoundFormat format, quint64 frames, int freq, QObject * parent):
    QObject(parent)
{
    m_frames = frames;
    m_data_size = nSoundFormat_getFramesize(format)*frames;
    m_data = new unsigned char[m_data_size];
    m_frequency = freq;
    m_format = format;
}

nSoundBag::~nSoundBag()
{
    delete[] m_data;
}
