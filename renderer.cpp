#include "renderer.h"

Renderer::Renderer(QWidget* parent)
    : QOpenGLWidget(parent) {
    controller = new Controller(this); // Contrôleur enfant de Renderer
    setFocusPolicy(Qt::NoFocus);

}

void Renderer::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Fond blanc
}

void Renderer::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    controller->getCamera().update(); // Met à jour la caméra via le contrôleur
}

void Renderer::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
    controller->getCamera().update(); // Met à jour la caméra via le contrôleur
    renderPoints();
}

void Renderer::renderPoints() {
    glColor3f(0.0f, 0.0f, 1.0f); // Bleu
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    for (const auto& coord : coordinates) {
        glVertex2f(coord.first, coord.second);
    }
    glEnd();
}


void Renderer::setCoordinates( std::vector<std::pair<float, float>> coordinates){
    this->coordinates = coordinates;
}
