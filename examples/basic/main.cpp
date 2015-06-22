#include <QCoreApplication>
#include <QDebug>
#include <QFile>

#include <neiasound/nSoundSystem.h>
#include <neiasound/nSoundStream.h>
#include <neiasound/nSoundSource.h>
#include <neiasound/nSoundBuffer.h>

#include <neiasound/wav/nwavestream.h>
#include <neiasound/nSoundFormat.h>

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


    qDebug() << "Playback";
    src->setLoop(true);
    src->play();

    qDebug() << "Press ctrl+C to quit";

    return a.exec();
}
