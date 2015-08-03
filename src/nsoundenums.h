#ifndef NSOUNDENUMS_H
#define NSOUNDENUMS_H

#include "neiasound_global.h"

// defined separately
#include "nsoundformat.h"
#include "nsoundsourcerole.h"

enum NEIASOUNDSHARED_EXPORT nSoundFilterType {
    SFT_NULL =       0x00,
    SFT_LOWPASS =    0x01,
    SFT_HIGHPASS =   0x02,
    SFT_BANDPASS =   0x03
};

#endif // NSOUNDENUMS_H

