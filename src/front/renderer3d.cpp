#include "renderer3d.h"

Renderer3D::Renderer3D() : objectLoader(nullptr)
{qDebug() << "Renderer3D created: " << this;}

Renderer3D::~Renderer3D() {
    if (objectLoader) {
        delete objectLoader;
    }
}



void Renderer3D::paintGl3D(QMatrix4x4 modelViewMatrix){
    if (objectLoader) {

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

void Renderer3D::reset3D(){
    if (objectLoader) {
        delete objectLoader;
        objectLoader = nullptr;
    }
}

void Renderer3D::setObjectLoader(ObjectLoader* loader) {
    if (objectLoader) {
        delete objectLoader;
    }
    objectLoader = loader;
}

QMatrix4x4 Renderer3D::getModelMatrix() {
    QMatrix4x4 modelMatrix;
    modelMatrix.translate(0.0f, 0.0f, -3.0f);
    modelMatrix.rotate(objectLoader->getAngle(), 0.0f, 1.0f, 0.0f);
    modelMatrix.scale(0.005f);
    return modelMatrix;
}



