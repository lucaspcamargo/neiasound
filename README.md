# neiasound #

## Introduction ##

neiasound is a Qt-style OpenAL wrapper for games. Not yet complete but already very useful.
It is a clean, convenient api that can be plugged directly into your main loop.
It comes with a (very primitive) .wav file reader, and includes [stb_vorbis](http://nothings.org/stb_vorbis/) directly in the source tree. Also supports [libsndfile](http://www.mega-nerd.com/libsndfile/).

There is also support for streaming dynamic playlists with intros and seamless looping.

## How do I get set up? ##

The classic combination of `qmake`, `make`, and `sudo make install` works just fine. This will also generate and install `pkgconfig` files for simplifying the configuration of dependent projects.

Please look into "neiasound.pro" for configuration options.

On Android, it is compatible with the standard OpenAL Soft port, and OpenAL-MOB. I recommend OpenAL-MOB for a better experience and reduced latency. If you disable HRTFs, I suspect performance is the same or better.

### Embedding sources in your project ###

You can also add the soures and headers to your project structure directly, building it into the executable. Please observe the requirements of the BSD license if doing so.

All you need is to add the relevant source files into your project source lists.
It is a good idea to cherry-pick only the nSoundStreamer implementations you actually use.
The Wave and stb_vorbis streams are enough in most cases, so the only hard dependencies are OpenAL itself and Qt.
libsndfile is optional, but it's there if you need flac or one of the many formats it supports.

Do note that this a Qt library, and the sources need to go through moc when included directly.

## FAQ ##

**Q: Does it support MP3? Are there plans for doing so?**
A: No and no. MP3 is a proprietary format and the free libraries that decode it fall into a legal grey-area.
This is the same reason why libsndfile does not directly support it.
As an alternative, the OGG Vorbis format is free from patent restrictions, has a great public domain decoder implementation included in this source tree, and best of all, is technically superior to MP3 in almost every way.
Please consider converting your MP3's to OGG Vorbis.
You can also implement your own subclass of nSoundStreamer that reads MP3 files.
There might be a libav streamer in the future that would do that transparently, off-process, through pipes.


### Who do I talk to? ###

If you run into any problems, I (lpcamargo) will be most pleased to help!