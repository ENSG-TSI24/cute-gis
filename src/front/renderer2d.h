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
    void highlightGeometry(const std::string& layerName, int rowIndex);

    std::vector<std::shared_ptr<LayerBase>> lst_layers2d;

private:
    void renderLayers2d();
};

#endif // RENDERER2D_H
