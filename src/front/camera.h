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


#include <glm/gtc/quaternion.hpp>

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
    void moveFront3D(float step);
    void moveBack3D(float step);
    void moveLeft3D(float step);
    void moveRight3D(float step);
    void moveDown3D(float step);
    void moveUp3D(float step);

    void rotateVertAng(float angle);
    void rotateHorAng(float angle);

    void update();
    float speedFactor;
    float getZ();
    void setZ(float z);
    glm::vec3 getPosition();
    QMatrix4x4 getViewMatrix(); // Matrice de vue pour le mode 3D
    QMatrix4x4 getProjectionMatrix(int screenWidth, int screenHeight); // Projection 2D/3D

    void resetCamera();
    void centerOnBoundingBox(const BoundingBox& bbox);
    int getRWidth();
    int getRHeight();
    float getHorAng();
    float getVertAng();
    void setRWidth(int width);
    void setRHeight(int height);

private:
    glm::vec3 position;
    int renderer_width;
    int renderer_height;
    glm::vec3 forward;
    glm::vec3 right;
    glm::vec3 up;
    float VertAng=0;
    float HorAng=0;
};

#endif // CAMERA_H
