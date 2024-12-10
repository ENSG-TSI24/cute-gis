#ifndef RENDERER2D_H
#define RENDERER2D_H

#include "layer2d.h"
#include "layerraster.h"
#include "renderer.h"

class Renderer2D {
public:
    Renderer2D(Renderer& renderer);
    ~Renderer2D();
    void paintGl2D();
    void calculateBoundingBox();
    void reset2D();

    Renderer& parent;
    std::vector<LayerRaster> lst_layersraster;
    std::vector<Layer2d> lst_layers2d;
    BoundingBox boundingBox;

private:
    void renderLayers2d();
    void renderRasters();

};

#endif // RENDERER2D_H
