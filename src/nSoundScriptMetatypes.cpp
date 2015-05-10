#include "nSoundScriptMetatypes.h"

#ifdef NEIA

N_SCRIPT_METATYPE_QOBJECT_PTR_FUNCTIONS(nSoundBag)
N_SCRIPT_METATYPE_QOBJECT_PTR_FUNCTIONS(nSoundBuffer)
N_SCRIPT_METATYPE_QOBJECT_PTR_FUNCTIONS(nSoundEffectParameters)
N_SCRIPT_METATYPE_QOBJECT_PTR_FUNCTIONS(nSoundListener)
N_SCRIPT_METATYPE_QOBJECT_PTR_FUNCTIONS(nSoundSource)
N_SCRIPT_METATYPE_QOBJECT_PTR_FUNCTIONS(nSoundStream)
N_SCRIPT_METATYPE_QOBJECT_PTR_FUNCTIONS(nSoundStreamer)
N_SCRIPT_METATYPE_QOBJECT_PTR_FUNCTIONS(nSoundStreamerPlaylist)
N_SCRIPT_METATYPE_QOBJECT_PTR_FUNCTIONS(nSoundSystem)

void nSoundScriptMetatypes_registerAll(QScriptEngine & engine)
{
    N_SCRIPT_METATYPE_REGISTER( engine, nSoundBag);
    N_SCRIPT_METATYPE_REGISTER( engine, nSoundBuffer);
    N_SCRIPT_METATYPE_REGISTER( engine, nSoundEffectParameters);
    N_SCRIPT_METATYPE_REGISTER( engine, nSoundListener);
    N_SCRIPT_METATYPE_REGISTER( engine, nSoundSource);
    N_SCRIPT_METATYPE_REGISTER( engine, nSoundStream);
    N_SCRIPT_METATYPE_REGISTER( engine, nSoundStreamer);
    N_SCRIPT_METATYPE_REGISTER( engine, nSoundStreamerPlaylist);
    N_SCRIPT_METATYPE_REGISTER( engine, nSoundSystem);
}
#endif
