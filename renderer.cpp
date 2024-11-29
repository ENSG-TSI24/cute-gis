#include "renderer.h"

Renderer::Renderer(QWidget* parent)
    : QOpenGLWidget(parent), objectLoader() {
    controller = new Controller(this);
    setFocusPolicy(Qt::StrongFocus);

    is3D = false;
}

void Renderer::keyPressEvent(QKeyEvent *event) {
    this->controller->ControllerkeyPressEvent(event);
}

void Renderer::wheelEvent(QWheelEvent* event) {
    this->controller->ControllerwheelEvent(event);
}

void Renderer::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Fond noir

    // modif 3D
    glEnable(GL_DEPTH_TEST);
}

void Renderer::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);

    controller->getCamera().update(); // Met à jour la caméra via le contrôleur


    // modif 3D

    // projection matrix with perspective
    QMatrix4x4 projectionMatrix;
    projectionMatrix.perspective(45.0f, float(w) / float(h), 0.1f, 100.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(projectionMatrix.constData());
}

void Renderer::paintGL() {

    if (!is3D) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    controller->getCamera().update();
    renderPoints();
    } else {

    // modif 3D

    // create view matrix for the camera
    QMatrix4x4 viewMatrix;

    // setting position of camera
    QVector3D cameraPosition(0.0f, 4.0f, 3.0f);

    // center object position
    QVector3D target(0.0f, 0.0f, 0.0f);

    // set orientation top-bottom view
    QVector3D upVector(0.0f, 1.0f, 0.0f);
    viewMatrix.lookAt(cameraPosition, target, upVector);

    // create model matrix
    QMatrix4x4 modelMatrix;

    // position of the object translate
    modelMatrix.translate(0.0f, 0.0f, -3.0f);

    // position of the object rotate
    modelMatrix.rotate(objectLoader.getAngle(), 0.0f, 1.0f, 0.0f);

    // scale of object
    modelMatrix.scale(0.005f);

    QMatrix4x4 modelViewMatrix = viewMatrix * modelMatrix;

    // load matrix to opengl
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(modelViewMatrix.constData());

    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    const auto& vertices = objectLoader.getVertices();
    for (size_t i = 0; i < vertices.size(); i++) {
        const glm::vec3& vertex = vertices[i];
        glVertex3f(vertex.x, vertex.y, vertex.z);
    }
    glEnd();
    }

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

void Renderer::setCoordinates(std::vector<std::pair<float, float>> coordinates) {
    this->coordinates = coordinates;
}
