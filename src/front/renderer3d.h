#ifndef RENDERER3D_H
#define RENDERER3D_H

#include "renderer.h"
#include "objectloader.h"

class Renderer3D {
public:
    Renderer3D(Renderer& renderer);
    ~Renderer3D();
    void paintGl3D();
    void reset3D();
    void setObjectLoader(ObjectLoader* loader);

    Renderer& parent;

private:
    ObjectLoader* objectLoader;

};

#endif // RENDERER3D_H
