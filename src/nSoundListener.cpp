#include "nSoundListener.h"
#include "nSoundSystem.h"
#ifdef NEIA
#include "../scene/nSceneCamera.h"
#include "OgreCamera.h"
#endif

#include "AL/al.h"

nSoundListener::nSoundListener(nSoundSystem * parent) :
    QObject(parent)
{
#ifdef NEIA
    m_camera = 0;
#endif
    m_updating = true;
}

#ifdef NEIA
void nSoundListener::setSourceCamera(nSceneCamera * cam)
{
    if(m_camera)
    {
        disconnect(this, SLOT(cameraDestroyed()));
    }
    m_camera = cam;
    if(m_camera) //camera can be set to null
    {
        m_camLastPos = m_camera->ogreCamera()->getPosition();
        connect(m_camera, SIGNAL(destroyed()), SLOT(cameraDestroyed()));
    }
}
void nSoundListener::cameraDestroyed()
{
    disconnect(this, SLOT(cameraDestroyed()));
    m_camera = 0;
}
#endif

void nSoundListener::update(qreal frameTime)
{
#ifdef NEIA
    if(m_updating && m_camera)
    {
        nVec3 pos(m_camera->ogreCamera()->getRealPosition()),
                dir(m_camera->ogreCamera()->getRealDirection()),
                up(m_camera->ogreCamera()->getRealUp());
        nVec3 vel = pos - m_camLastPos;
        m_camLastPos = pos;
        vel/=frameTime;


        updateManual(QVector3D());
    }
#endif

}

void nSoundListener::updateManual(QVector3D pos, QVector3D dir, QVector3D up, QVector3D vel)
{
    float orient[6];
    orient[0] = dir[0];
    orient[1] = dir[1];
    orient[2] = dir[2];
    orient[3] = up[0];
    orient[4] = up[1];
    orient[5] = up[2];

    alGetError();
    alListener3f(AL_POSITION, pos[0], pos[1], pos[2]);
    alListenerfv(AL_ORIENTATION, orient);
    alListener3f(AL_VELOCITY, vel[0], vel[1], vel[2]);

    if(alGetError()!=AL_NO_ERROR)
        throw QString("nSoundListener: Failed to update listener transformations.");
}
