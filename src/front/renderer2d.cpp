#include "renderer2d.h"

Renderer2D::Renderer2D(Renderer& renderer) : parent(renderer) {}

Renderer2D::~Renderer2D() {}

void Renderer2D::paintGl2D(){
    renderRasters();
    renderLayers2d();
}

void Renderer2D::renderLayers2d(){
    for (auto& layer: lst_layers2d){
        if (layer.isVisible){
            layer.renderPoints();
            layer.renderLinestrings();
            layer.renderPolygons();
        }
    }
}

void Renderer2D::renderRasters() {
    int i = 0;
    for (auto& raster: lst_layersraster){
        std::cout<<"------------ Layer : "<<i<< " ------------"<<std::endl;
        raster.renderRasters();
        ++i;
    }
}

void Renderer2D::reset2D(){
    if(lst_layers2d.size()!=0 ){
        lst_layers2d.clear();
        parent.controller->getCamera().resetCamera();
        parent.update();
    }
}
