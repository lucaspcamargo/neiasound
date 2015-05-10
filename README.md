# neiasound #

### Introduction ###

neiasound is a Qt-style OpenAL wrapper for games. Not yet complete but already very useful.
It is a very nice, convenient api that can be plugged directly into your main loop.
It comes with a (very primitive) .wav file reader, and includes [stb_vorbis](http://nothings.org/stb_vorbis/) directly in the source tree. Also supports [libsndfile](http://www.mega-nerd.com/libsndfile/).

### How do I get set up? ###

All you need is to add the relevant source files into your project source lists.
It is a good idea to cherry-pick only the nSoundStreamer implementations you actually use.
The Wave and stb_vorbis streams are enough in most cases, so the only hard dependencies are OpenAL itself and Qt. libsndfile is optional, but it's there if you need flac or one of the many formats it supports.

Do note that this a Qt library, and the sources need to go through moc when included directly.

### Who do I talk to? ###

If you run into any problems, I (lpcamargo) will be most pleased to help!