#include "renderer2d.h"
#include "src/back/vectordata.h"
#include <iostream>

Renderer2D::Renderer2D(){
    lst_layers2d = std::vector<Layer2d>();
    session = Session();

    auto layers2d_json = session.getLayers();
    for (auto& layer_path : layers2d_json) {
        const char* layer_path_cstr = layer_path.c_str();
        lst_layers2d.push_back(Layer2d(VectorData(layer_path_cstr)));
    }
    
}

Renderer2D::~Renderer2D() {}

void Renderer2D::paintGl2D() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    renderRasters();
    renderLayers2d();
}

void Renderer2D::renderLayers2d() {
    for (int i = lst_layers2d.size() - 1; i >= 0; --i) {
        auto& layer = lst_layers2d[i];
        if (layer.isVisible) {
            layer.renderPolygons();
            layer.renderLinestrings();
            layer.renderPoints();
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
        lst_layersraster.clear();
    }
}
