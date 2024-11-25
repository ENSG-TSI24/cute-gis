#include "triangle.h"

Triangle::Triangle(QWidget* parent) : QOpenGLWidget(parent) {}

void Triangle::initializeGL() {
    initializeOpenGLFunctions();

    // Configuration de base
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void Triangle::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void Triangle::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex3f(-0.6f,  0.3f, -1.0f);
    glVertex3f(-0.9f, -0.3f, -1.0f);
    glVertex3f(-0.3f, -0.3f, -1.0f);
    glEnd();
}
