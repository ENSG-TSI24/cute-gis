#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H


class BoundingBox
{
public:
    BoundingBox();
    BoundingBox(float minX, float maxX, float minY, float maxY);
    ~BoundingBox();

    float minX, maxX, minY, maxY;
};

#endif // BOUNDINGBOX_H
