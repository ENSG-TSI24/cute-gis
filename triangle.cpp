#include "triangle.h"
#include <QMatrix4x4>
#include <QTimer>
#include <tiny_obj_loader.h>

Triangle::Triangle(QWidget* parent)
    : QOpenGLWidget(parent), m_angle(0.0f) {

    // instance timer
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &Triangle::updateRotation);
    m_timer->start(16); // config for 16 ms (=60fps)
}

void Triangle::initializeGL() {
    initializeOpenGLFunctions();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    if (!loadOBJWithTinyObjLoader("/home/laurent/Documents/m2_tsi/cute-gis/data/city.obj", vertices, uvs, normals)) {
        qWarning("Failed to load OBJ file!");
    } else {
        qDebug() << "OBJ vertex count:" << vertices.size();
    }
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // create view matrix for the camera
    QMatrix4x4 viewMatrix;

    // setting position of camera
    QVector3D cameraPosition(0.0f, 8.0f, 3.0f);

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
    modelMatrix.rotate(m_angle, 0.0f, 1.0f, 0.0f);

    // scale of object
    modelMatrix.scale(0.005f);

    QMatrix4x4 modelViewMatrix = viewMatrix * modelMatrix;

    // load matrix to opengl
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(modelViewMatrix.constData());

    glColor3f(1.0f, 1.0f, 0.0f);

    glBegin(GL_TRIANGLES);
    for (size_t i = 0; i < vertices.size(); i++) {
        if (!normals.empty()) {
            const glm::vec3& normal = normals[i];
            glNormal3f(normal.x, normal.y, normal.z);
        }
        const glm::vec3& vertex = vertices[i];
        glVertex3f(vertex.x, vertex.y, vertex.z);
    }
    glEnd();
}


void Triangle::updateRotation() {
    // angle animation
    // can setup the rotate speed
    m_angle += 0.1f;

    // reset angle after complete rotate
    if (m_angle >= 360.0f) {
        m_angle -= 360.0f;
    }

    // update the scene
    update();
}

bool Triangle::loadOBJWithTinyObjLoader(
    const char* path,
    std::vector<glm::vec3>& out_vertices,
    std::vector<glm::vec2>& out_uvs,
    std::vector<glm::vec3>& out_normals) {

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path)) {
        qWarning() << "TinyObjLoader error:" << QString::fromStdString(warn + err);
        return false;
    }

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            glm::vec3 vertex = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };
            out_vertices.push_back(vertex);

            if (index.texcoord_index >= 0) {
                glm::vec2 uv = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1]
                };
                out_uvs.push_back(uv);
            }

            if (index.normal_index >= 0) {
                glm::vec3 normal = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
                };
                out_normals.push_back(normal);
            }
        }
    }

    return true;
}
