#-------------------------------------------------
#
# Project created by QtCreator 2015-05-11T22:43:48
#
#-------------------------------------------------

QT += gui

TARGET = neiasound
TEMPLATE = lib

DEFINES += NEIASOUND_LIBRARY

SOURCES += \
    src/nSoundBag.cpp \
    src/nSoundBuffer.cpp \
    src/nSoundEffectParameters.cpp \
    src/nSoundListener.cpp \
    src/nSoundSource.cpp \
    src/nSoundStream.cpp \
    src/nSoundStreamer.cpp \
    src/nSoundStreamerPlaylist.cpp \
    src/nSoundSystem.cpp \
    src/util/nEfxHelper.cpp \
    src/stb_vorbis/nvorbisstream.cpp \
    src/wav/nwavestream.cpp

HEADERS += \
    src/neiasound_global.h \
    src/nSoundBag.h \
    src/nSoundBuffer.h \
    src/nSoundEffectParameters.h \
    src/nSoundFormat.h \
    src/nSoundListener.h \
    src/nSoundSource.h \
    src/nSoundSourceRole.h \
    src/nSoundStream.h \
    src/nSoundStreamer.h \
    src/nSoundStreamerPlaylist.h \
    src/nSoundSystem.h \
    src/util/efx-util.h \
    src/util/nEfxHelper.h \
    src/stb_vorbis/nvorbisstream.h \
    src/wav/nwavestream.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
