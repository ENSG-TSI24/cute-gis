#include "renderer.h"
#include <QMatrix4x4>
#include <QDebug>
#include <QOpenGLFunctions>
#include <glm/vec3.hpp>



Renderer::Renderer(QWidget* parent)
    : QOpenGLWidget(parent), objectLoader(nullptr) {
    controller = new Controller(this);
    setFocusPolicy(Qt::StrongFocus);
    is3D = false;
}

Renderer::~Renderer() {
    if (objectLoader) {
        delete objectLoader;
    }
    delete controller;
}

void Renderer::keyPressEvent(QKeyEvent *event){
    this->controller->ControllerkeyPressEvent(event);
}

void Renderer::wheelEvent(QWheelEvent* event) {
    this->controller->ControllerwheelEvent(event);
}
//void  Renderer::mousePressEvent(QMouseEvent *event){
  //  this->controller->ControllerQMouseEvent(event);
//}

void Renderer::mousePressEvent(QMouseEvent* event) {
    controller->ControllerMousePressEvent(event);
}

void Renderer::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
}

void Renderer::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);

    controller->getCamera().update();

    QMatrix4x4 projectionMatrix;
    projectionMatrix.perspective(45.0f, float(w) / float(h), 0.1f, 100.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(projectionMatrix.constData());
}

void Renderer::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!objectLoader) {
        controller->getCamera().update();
        renderLayers2d();
    } else if (objectLoader) {
        QMatrix4x4 viewMatrix;

        QVector3D cameraPosition(controller->getCamera().getX(),
                                 controller->getCamera().getY(),
                                 controller->getCamera().getZoom());
        QVector3D target(0.0f, 0.0f, 0.0f);
        QVector3D upVector(0.0f, 1.0f, 0.0f);
        viewMatrix.lookAt(cameraPosition, target, upVector);

        QMatrix4x4 modelMatrix;
        modelMatrix.translate(0.0f, 0.0f, -3.0f);
        modelMatrix.rotate(objectLoader->getAngle(), 0.0f, 1.0f, 0.0f);
        modelMatrix.scale(0.005f);

        QMatrix4x4 modelViewMatrix = viewMatrix * modelMatrix;

        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(modelViewMatrix.constData());

        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_TRIANGLES);

        const auto& vertices = objectLoader->getVertices();
        for (const auto& vertex : vertices) {
            glVertex3f(vertex.x, vertex.y, vertex.z);
        }

        glEnd();
    } else {
        qWarning() << "No ObjectLoader assigned for 3D rendering.";
    }
}


void Renderer::renderLayers2d(){
    int i = 0;
    for (auto& layer: lst_layers2d){
        std::cout<<"------------ Layer : "<<i<< " ------------\n";
        layer.renderPoints();
        layer.renderLinestrings();
        ++i;
    }
}


void Renderer::setObjectLoader(ObjectLoader* loader) {
    if (objectLoader) {
        delete objectLoader;
    }
    objectLoader = loader;
}

void Renderer::setIs3D(bool enabled) {
    is3D = enabled;
}

void Renderer::reset() {

    if (objectLoader) {
        delete objectLoader;
        objectLoader = nullptr;
    }

    is3D = false;

    update();
}






void Renderer::mouseReleaseEvent(QMouseEvent* event) {
    controller->ControllerMouseReleaseEvent(event);
}

void Renderer::mouseMoveEvent(QMouseEvent* event) {
    controller->ControllerMouseMoveEvent(event);
    update(); // Redessine la scène après un déplacement
}
