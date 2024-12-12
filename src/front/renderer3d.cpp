#include "renderer3d.h"

Renderer3D::Renderer3D() : objectLoader(nullptr)
{qDebug() << "Renderer3D created: " << this;}

Renderer3D::~Renderer3D() {
    if (objectLoader) {
        delete objectLoader;
    }
}

ObjectLoader* Renderer3D::getObjectLoader() const {
    return objectLoader;
}

void Renderer3D::paintGl3D(QMatrix4x4 ViewMatrix, float width, float height) {
    if (objectLoader) {

        const std::vector<glm::vec3>& vertices = objectLoader->getVertices();
        // Configurer la matrice de projection
        QMatrix4x4 projectionMatrix;
        float aspectRatio = width / height;
        projectionMatrix.perspective(45.0f, aspectRatio, 0.1f, 1000.0f);

        // Appliquer la matrice de projection
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(projectionMatrix.constData());

        // Appliquer la matrice de vue
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(ViewMatrix.constData());

        // Dessiner les objets
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_TRIANGLES);

        //camera.centerOn3DModel(vertices);

        // Explicitly declare vertices as a constant reference
        for (const auto& vertex : vertices) {
            glVertex3f(vertex.x, vertex.z, vertex.y);
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



