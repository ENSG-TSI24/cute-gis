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



class Camera
{
public:
    Camera();

    float getX();
    float getY();
    void setX(float x);
    void setY(float y);

    float getZoomLevel();
    void setZoomLevel(float zoom);

    void moveUp(float step);
    void moveDown(float step);
    void moveLeft(float step);
    void moveRight(float step);

    void update();

private:
    float x;
    float y;
};

#endif // CAMERA_H
