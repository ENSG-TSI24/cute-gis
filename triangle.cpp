#include "triangle.h"
#include <QMatrix4x4>
#include <QTimer>
#include <fstream>
#include <sstream>

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

    if (!loadOBJ("/home/laurent/Documents/m2_tsi/cute-gis/data/cube.obj", vertices, uvs, normals)) {
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

    QMatrix4x4 modelMatrix;
    modelMatrix.translate(0.0f, 0.0f, -3.0f);
    modelMatrix.rotate(m_angle, 0.0f, 1.0f, 0.0f);

    modelMatrix.scale(0.5f);

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(modelMatrix.constData());

    glColor3f(1.0f, 1.0f, 0.0f);

    // create the object
    glBegin(GL_TRIANGLES);
    for (size_t i = 0; i < vertices.size(); i++) {
        if (!normals.empty()) {
            const glm::vec3& normal = normals[i];
            glNormal3f(normal.x, normal.y, normal.z);
        }
        const glm::vec3& vertex = vertices[i];
        // create each vertex
        glVertex3f(vertex.x, vertex.y, vertex.z);
    }

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

bool Triangle::loadOBJ(
        const char* path,
        std::vector<glm::vec3>& out_vertices,
        std::vector<glm::vec2>& out_uvs,
        std::vector<glm::vec3>& out_normals) {

    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;

    std::ifstream file(path);
    if (!file.is_open()) {
        qDebug() << "impossible to open the file!";
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream lineStream(line);
        std::string lineHeader;
        lineStream >> lineHeader;

        // extract each vertex start v
        if (lineHeader == "v") {
            glm::vec3 vertex;
            lineStream >> vertex.x >> vertex.y >> vertex.z;
            temp_vertices.push_back(vertex);
        }

        // extract each vertex start vt
        else if (lineHeader == "vt") {
            glm::vec2 uv;
            lineStream >> uv.x >> uv.y;
            temp_uvs.push_back(uv);
        }

        // extract each vertex start vn
        else if (lineHeader == "vn") {
            glm::vec3 normal;
            lineStream >> normal.x >> normal.y >> normal.z;
            temp_normals.push_back(normal);
        }

        // extract indices of the object
        else if (lineHeader == "f") {
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            char slash1, slash2;
            for (int i = 0; i < 3; i++) {
                lineStream >> vertexIndex[i] >> slash1 >> uvIndex[i] >> slash2 >> normalIndex[i];
                vertexIndices.push_back(vertexIndex[i]);
                uvIndices.push_back(uvIndex[i]);
                normalIndices.push_back(normalIndex[i]);
            }
        }
    }

    file.close();

    for (unsigned int i = 0; i < vertexIndices.size(); i++) {
        unsigned int vertexIndex = vertexIndices[i];
        out_vertices.push_back(temp_vertices[vertexIndex - 1]);
    }
    for (unsigned int i = 0; i < uvIndices.size(); i++) {
        unsigned int uvIndex = uvIndices[i];
        out_uvs.push_back(temp_uvs[uvIndex - 1]);
    }
    for (unsigned int i = 0; i < normalIndices.size(); i++) {
        unsigned int normalIndex = normalIndices[i];
        out_normals.push_back(temp_normals[normalIndex - 1]);
    }

    return true;
}

