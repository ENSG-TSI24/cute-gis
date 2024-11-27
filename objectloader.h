#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>
#include <glm/glm.hpp>

class ObjectLoader : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit ObjectLoader(QWidget* parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    QTimer* m_timer;
    float m_angle;

     std::vector<glm::vec3> vertices;
     std::vector<glm::vec3> normals;
     std::vector<glm::vec2> uvs;

     bool loadOBJ(
             const char* path,
             std::vector<glm::vec3>& out_vertices,
             std::vector<glm::vec2>& out_uvs,
             std::vector<glm::vec3>& out_normals);
    bool loadOBJWithTinyObjLoader(
            const char* path,
            std::vector<glm::vec3>& out_vertices,
            std::vector<glm::vec2>& out_uvs,
            std::vector<glm::vec3>& out_normals);

private slots:
    void updateRotation();
};


#endif // TRIANGLE_H


