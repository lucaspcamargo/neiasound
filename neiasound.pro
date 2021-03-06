#
# neiasound Main Project File
#
# Copyright (c) 2015, Lucas Pires Camargo
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation and/or
# other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


QT += gui

TARGET = neiasound
TEMPLATE = lib
VERSION = 0.1

DEFINES += NEIASOUND_LIBRARY
CONFIG += NEIASOUND_USE_STB_VORBIS NEIASOUND_USE_WAVE_STREAM NEIASOUND_USE_SNDFILE

SOURCES += \
    src/nsoundbag.cpp \
    src/nsoundbuffer.cpp \
    src/nsoundeffectparameters.cpp \
    src/nsoundlistener.cpp \
    src/nsoundsource.cpp \
    src/nsoundstream.cpp \
    src/nsoundstreamer.cpp \
    src/nsoundstreamerplaylist.cpp \
    src/nsoundsystem.cpp \
    src/util/nefxhelper.cpp \
    src/nsoundfilter.cpp

HEADERS += \
    src/neiasound_global.h \
    src/nsoundbag.h \
    src/nsoundbuffer.h \
    src/nsoundeffectparameters.h \
    src/nsoundformat.h \
    src/nsoundlistener.h \
    src/nsoundsource.h \
    src/nsoundsourcerole.h \
    src/nsoundstream.h \
    src/nsoundstreamer.h \
    src/nsoundstreamerplaylist.h \
    src/nsoundsystem.h \
    src/util/efx-util.h \
    src/nsoundfilter.h \
    src/neiasound_al.h \
    src/nsoundenums.h
    #src/util/nefxHelper.h \

unix: CONFIG += link_pkgconfig

CONFIG(NEIASOUND_USE_STB_VORBIS) {
    SOURCES += src/stb_vorbis/nvorbisstream.cpp
    HEADERS += src/stb_vorbis/nvorbisstream.h
}

CONFIG(NEIASOUND_USE_WAVE_STREAM) {
    SOURCES += src/wav/nwavestream.cpp
    HEADERS += src/wav/nwavestream.h
}

CONFIG(NEIASOUND_USE_SNDFILE) {
    SOURCES += src/sndfile/nsndfilestream.cpp
    HEADERS += src/sndfile/nsndfilestream.h

    unix: PKGCONFIG += sndfile
    !unix {
        INCLUDEPATH += $$LIBSNDFILE_INCLUDEPATH
        LIBS += -L$$LIBSDFILE_LIBPATH -l$$LIBSNDFILE_LIB
    }
}


unix {
    target.path = /usr/lib
    INSTALLS += target

    INSTALL_PREFIX = /usr/include/neiasound
    INSTALL_HEADERS = $$HEADERS
    include(headerinstall.pri)

    CONFIG += create_pc create_prl no_install_prl

    QMAKE_PKGCONFIG_NAME = neiasound
    QMAKE_PKGCONFIG_DESCRIPTION = Qt-like wrapper and utilities for OpenAL
    QMAKE_PKGCONFIG_PREFIX = $$INSTALLBASE
    QMAKE_PKGCONFIG_LIBDIR = $$target.path
    QMAKE_PKGCONFIG_INCDIR = $$INSTALL_PREFIX
    QMAKE_PKGCONFIG_VERSION = $$VERSION
}
