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


/*struct BoundingBox {
    float minX, maxX, minY, maxY;
};*/

struct BoundingBox {
    float minX, maxX, minY, maxY;

    // Surcharge de l'opérateur []
    float& operator[](size_t index) {
        switch (index) {
            case 0: return minX;
            case 1: return maxX;
            case 2: return minY;
            case 3: return maxY;
            default: throw std::out_of_range("Index out of range");
        }
    }

    const float& operator[](size_t index) const {
        switch (index) {
            case 0: return minX;
            case 1: return maxX;
            case 2: return minY;
            case 3: return maxY;
            default: throw std::out_of_range("Index out of range");
        }
    }
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

private:
    float x;
    float y;
    float zoom;
};

#endif // CAMERA_H
