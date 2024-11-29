#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>
#include <glm/glm.hpp>

class ObjectLoader : public QObject {
    Q_OBJECT

public:
    explicit ObjectLoader(const std::string& filePath, QObject* parent = nullptr);

    bool loadOBJWithTinyObjLoader(
        const char* path,
        std::vector<glm::vec3>& out_vertices,
        std::vector<glm::vec2>& out_uvs,
        std::vector<glm::vec3>& out_normals);

    float getAngle() const;
    const std::vector<glm::vec3>& getVertices() const;
    const std::vector<glm::vec3>& getNormals() const;

private:
    QTimer* m_timer;
    float m_angle;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;

    void updateRotation();
};

#endif // TRIANGLE_H
