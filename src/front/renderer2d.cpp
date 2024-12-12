#include "renderer2d.h"

Renderer2D::Renderer2D(){}

Renderer2D::~Renderer2D() {}

void Renderer2D::paintGl2D() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    renderRasters();
    renderLayers2d();
}

void Renderer2D::renderLayers2d() {
    for (int i = lst_layersvector.size() - 1; i >= 0; --i) {
        auto& vector = lst_layersvector[i];
        if (vector->isVisible) {
            vector->renderPolygons();
            vector->renderLinestrings();
            vector->renderPoints();
        }
    }
}

void Renderer2D::renderRasters() {
    for (int i = lst_layersraster.size() - 1; i >= 0; --i) {
        auto& raster = lst_layersraster[i];
        if (raster->isVisible) {
            raster->renderRasters();
        }
    }
}

void Renderer2D::reset2D(){
    if(lst_layersvector.size()!=0 ){
        lst_layersvector.clear();
        lst_layersraster.clear();
    }
}
