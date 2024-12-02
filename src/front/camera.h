#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

#include <fstream>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <utility>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>

struct BoundingBox {
    float minX, maxX, minY, maxY;
};

/**
 * \class Camera
 * The aim of this class is to provide a Camera to manage the vue of the user.
 */


class Camera
{
public:
    Camera();

    /**
    * @brief Get the value of the x coordonate
    * @return float
    */
    float getX();
    /**
    * @brief Get the value of the y coordonate
    * @return float
    */
    float getY();
    /**
    * @brief Set the value to the x coordonate
    *
    * @param The  float to assign to the coordonate
    */
    void setX(float x);
    /**
    * @brief Set the value to the y coordonate
    *
    * @param The  float to assign to the coordonate
    */

    void setY(float y);
    /**
    * @brief Add to the y coordonate
    *
    * @param The  float to add  to the coordonate
    */

    void moveUp(float step);
    /**
    * @brief Minus the value to the y coordonate
    *
    * @param The float to substract to the coordonate
    */
    void moveDown(float step);
    /**
    * @brief Add to the x coordonate
    *
    * @param The  The  float to add  to the coordonate
    */
    void moveLeft(float step);
    /**
    * @brief Minus the value to the x coordonate
    *
    * @param The float to substract to the coordonate
    */
    void moveRight(float step);
    /**
    * @brief Update the the camera with a matrix this matrix contains the edge of the vue and
    * the fov
    *
    */

    void update();

    float getZoom();
    void setZoom(float zoom);

    void centerOnBoundingBox(const BoundingBox& bbox);
    glm::vec3 getPosition();
    QMatrix4x4 getViewMatrix(); // Matrice de vue pour le mode 3D
    QMatrix4x4 getProjectionMatrix(int screenWidth, int screenHeight); // Projection 2D/3D
    QMatrix4x4 getModelViewMatrix(const QMatrix4x4& modelMatrix);
private:
    float x;
    float y;
    float zoom;
};

#endif // CAMERA_H
