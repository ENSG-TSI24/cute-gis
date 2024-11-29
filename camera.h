#ifndef CAMERA_H
#define CAMERA_H

struct BoundingBox {
    float minX, maxX, minY, maxY;
};

class Camera {
public:
    Camera();

    float getX() const;
    float getY() const;
    void setX(float x);
    void setY(float y);

    float getZoomLevel() const;
    void setZoomLevel(float zoom);

    void moveUp(float step);
    void moveDown(float step);
    void moveLeft(float step);
    void moveRight(float step);

    void centerOnBoundingBox(const BoundingBox& bbox);
    void apply(); // Applique les transformations de la caméra

private:
    float x;
    float y;
    float zoomLevel;
};

#endif // CAMERA_H
