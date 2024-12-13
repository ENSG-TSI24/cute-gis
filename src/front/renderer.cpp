#include "renderer.h"
#include "renderer2d.h"
#include "renderer3d.h"

#include <QMatrix4x4>
#include <QDebug>
#include <QOpenGLFunctions>
#include <glm/vec3.hpp>



Renderer::Renderer(QWidget* parent)
    : QOpenGLWidget(parent) {
    controller = new Controller(this);
    qDebug() << "Controller created: " << controller;
    renderer2d = new Renderer2D();
    renderer3d = new Renderer3D();
    setFocusPolicy(Qt::StrongFocus);
    is3D = false;
    this->getRenderer2d()->lst_layers2d;
}

Renderer::~Renderer() {
    delete controller;
    delete renderer2d;
    delete renderer3d;
}

void Renderer::keyPressEvent(QKeyEvent *event){
    this->controller->ControllerkeyPressEvent(event);
}

void Renderer::wheelEvent(QWheelEvent* event) {
    this->controller->ControllerwheelEvent(event);
}

void Renderer::mousePressEvent(QMouseEvent* event) {
    controller->ControllerMousePressEvent(event);
}

void Renderer::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    controller->getCamera().setRHeight(height());
    controller->getCamera().setRWidth(width());
}

void Renderer::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);

    controller->getCamera().setRHeight(h);
    controller->getCamera().setRWidth(w);
    controller->getCamera().update();

    QMatrix4x4 projectionMatrix;
    projectionMatrix.perspective(45.0f, float(w) / float(h), 0.1f, 100.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(projectionMatrix.constData());
}


void Renderer::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (!is3D) {
        controller->getCamera().update();
        renderer2d->paintGl2D();
    } else {
        if (renderer3d->getObjectLoader()) {
            controller->set3DMode(true);
            QMatrix4x4 modelMatrix = renderer3d->getModelMatrix();
            QMatrix4x4 modelViewMatrix = controller->getCamera().getModelViewMatrix(modelMatrix);

            renderer3d->paintGl3D(modelViewMatrix);
        }
    };
}

Renderer2D* Renderer::getRenderer2d() {
    return renderer2d;
}

Renderer3D* Renderer::getRenderer3d() {
    return renderer3d;
}

void Renderer::reset() {
    renderer2d->reset2D();
    renderer3d->reset3D();
    controller->getCamera().resetCamera();
    update();
}

void Renderer::reset2D() {
    renderer2d->reset2D();
    controller->getCamera().resetCamera();
    update();
}

void Renderer::reset3D() {
    renderer3d->reset3D();
    controller->getCamera().resetCamera();
    update();
}

void Renderer::setIs3D(bool enabled) {
    is3D = enabled;
    controller->set3DMode(enabled);
}

bool Renderer::getIs3D() {
    return controller->get3DMode();
}

Renderer2D* Renderer::getRenderer2D() {
    return renderer2d;
}

Renderer3D* Renderer::getRenderer3D() {
    return renderer3d;
}

void Renderer::mouseReleaseEvent(QMouseEvent* event) {
    controller->ControllerMouseReleaseEvent(event);
}

void Renderer::mouseMoveEvent(QMouseEvent* event) {
    controller->ControllerMouseMoveEvent(event);
    update(); // Redessine la scène après un déplacement
}
