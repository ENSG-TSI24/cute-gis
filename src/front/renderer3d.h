#ifndef RENDERER3D_H
#define RENDERER3D_H

#include "renderer.h"
#include "objectloader.h"

class Renderer3D {
public:
    Renderer3D();
    ~Renderer3D();
    void paintGl3D(QMatrix4x4 modelViewMatrix);
    void reset3D();
    void setObjectLoader(ObjectLoader* loader);
    QMatrix4x4 getModelMatrix();

    ObjectLoader* getObjectLoader() const;

private:
    ObjectLoader* objectLoader;

};

#endif // RENDERER3D_H
