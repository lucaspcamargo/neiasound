#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QVariantMap>

#include <neiasound/nsoundsystem.h>
#include <neiasound/nsoundstream.h>
#include <neiasound/nsoundsource.h>
#include <neiasound/nsoundbuffer.h>
#include <neiasound/nsoundfilter.h>

#include <neiasound/wav/nwavestream.h>
#include <neiasound/nsoundformat.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    nSoundSystem system;

    qDebug() << "Creating IODevice";
    QFile file(":/wind.wav");

    if(!file.open(QFile::ReadOnly))
    {
        qWarning("Could not open resource. Bad compile?");
        return -1;
    }

    qDebug() << "Creating stream";
    nSoundStream * stream = new nWaveStream(&file, SF_WAVE_HEADER, -1, -1, &system);


    qDebug() << "Creating buffer";
    nSoundBuffer * buf = system.createBuffer("buffer");
    buf->setData(stream);

    qDebug() << "Creating source";
    nSoundSource * src = system.createSource("source");
    src->attachBuffer(buf);

    qDebug() << "Creating filter";
    nSoundFilter * filter = system.createFilter("filter", SFT_LOWPASS);

    // you can set a map of properties
    QVariantMap filterProperties;
    filterProperties.insert("gain", 0.8);
    filter->setProperties(filterProperties);
    src->attachDirectFilter(filter);


    qDebug() << "Playback";
    src->setLoop(true);
    src->play();

    qDebug() << "Press ctrl+C to quit";

    return a.exec();
}
