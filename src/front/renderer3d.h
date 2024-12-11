#ifndef RENDERER3D_H
#define RENDERER3D_H

#include "renderer.h"
#include "objectloader.h"

class Renderer3D {
public:
    Renderer3D();
    ~Renderer3D();
    void paintGl3D(QMatrix4x4 modelMatrix);
    void reset3D();
    void setObjectLoader(ObjectLoader* loader);

    Renderer& parent;
    ObjectLoader* getObjectLoader() const;
    QMatrix4x4 getModelMatrix();


};

#endif // RENDERER3D_H
