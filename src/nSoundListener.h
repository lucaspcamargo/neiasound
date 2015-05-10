#ifndef NSOUNDLISTENER_H
#define NSOUNDLISTENER_H

#include <QObject>
#include <QVector3D>

class nSceneCamera;
class nSoundSystem;

class nSoundListener : public QObject
{
    Q_OBJECT
#ifdef NEIA
    Q_PROPERTY(nSceneCamera * sourceCamera READ sourceCamera WRITE setSourceCamera)
#endif
    Q_PROPERTY(bool updating READ isUpdating WRITE setUpdating)
public:
    explicit nSoundListener(nSoundSystem * parent);

#ifdef NEIA
    nSceneCamera * sourceCamera(){return m_camera;}
    void setSourceCamera(nSceneCamera * cam);
#endif

    bool isUpdating(){return m_updating;}
    void setUpdating(bool b){m_updating = b;}

signals:

public slots:
#ifdef NEIA
    void cameraDestroyed();
#endif
    void update(qreal frameTime);
    void updateManual(QVector3D position, QVector3D direction, QVector3D up, QVector3D velocity);

private:
#ifdef NEIA
    nSceneCamera * m_camera;
    nVec3 m_camLastPos;
#endif
    bool m_updating;
};

#endif // NSOUNDLISTENER_H
