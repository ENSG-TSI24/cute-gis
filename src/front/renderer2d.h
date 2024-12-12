#ifndef RENDERER2D_H
#define RENDERER2D_H

#include "layer2d.h"
#include "layerraster.h"
#include "renderer.h"

class Renderer2D {
public:
    Renderer2D();
    ~Renderer2D();
    void paintGl2D();
    void reset2D();

    std::vector<std::shared_ptr<LayerRaster>> lst_layersraster;
    std::vector<std::shared_ptr<Layer2d>> lst_layersvector;

private:
    void renderLayers2d();
    void renderRasters();

};

#endif // RENDERER2D_H
