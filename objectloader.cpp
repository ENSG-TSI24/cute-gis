#include "objectloader.h"
#include <QDebug>
#include <tiny_obj_loader.h>
#include <QTimer>

ObjectLoader::ObjectLoader(QObject* parent)
    : m_angle(0.0f) {
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, [this]() { updateRotation(); });
    m_timer->start(16); // 60 FPS

    if (!loadOBJWithTinyObjLoader("/home/formation/Documents/projet/cute-gis/city.obj", vertices, uvs, normals)) {
        qWarning("Failed to load OBJ file!");
    } else {
        qDebug() << "OBJ vertex count:" << vertices.size();
    }
}

void ObjectLoader::updateRotation() {
    // angle animation
    // can setup the rotate speed
    m_angle += 0.1f;

    // reset angle after complete rotate
    if (m_angle >= 360.0f) {
        m_angle -= 360.0f;
    }
}

bool ObjectLoader::loadOBJWithTinyObjLoader(
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

float ObjectLoader::getAngle() const {
    return m_angle;
}

const std::vector<glm::vec3>& ObjectLoader::getVertices() const {
    return vertices;
}

const std::vector<glm::vec3>& ObjectLoader::getNormals() const {
    return normals;
}
