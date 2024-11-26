#include "triangle.h"
#include <QMatrix4x4>
#include <QTimer>

Triangle::Triangle(QWidget* parent)
    : QOpenGLWidget(parent), m_angle(0.0f) {

    // instance timer
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &Triangle::updateRotation);
    m_timer->start(16); // config for 16 ms (=60fps)
}

void Triangle::initializeGL() {
    initializeOpenGLFunctions();

    // background color
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    // deep 3d
    glEnable(GL_DEPTH_TEST);
}

void Triangle::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);

    // projection matrix with perspective
    QMatrix4x4 projectionMatrix;
    projectionMatrix.perspective(45.0f, float(w) / float(h), 0.1f, 100.0f);

    // load ma matrice
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(projectionMatrix.constData());
}

void Triangle::paintGL() {

    // remove buffer and deep
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // create circular rotate of my triangle
    QMatrix4x4 modelMatrix;
    modelMatrix.translate(0.0f, 0.0f, -3.0f);
    modelMatrix.rotate(m_angle, 0.0f, 1.0f, 0.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(modelMatrix.constData());

    // red color for triangle
    glColor3f(1.0f, 0.0f, 0.0f); // Couleur rouge
    glBegin(GL_TRIANGLES);
    glVertex3f(-0.6f,  0.3f, 0.0f);
    glVertex3f(-0.9f, -0.3f, 0.0f);
    glVertex3f(-0.3f, -0.3f, 0.0f);
    glEnd();
}

void Triangle::updateRotation() {
    // angle animation
    m_angle += 1.0f;

    // reset angle after complete rotate
    if (m_angle >= 360.0f) {
        m_angle -= 360.0f;
    }

    // update the scene
    update();
}
