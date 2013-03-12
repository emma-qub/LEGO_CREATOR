#include "BoundingBox.h"

BoundingBox::BoundingBox(int lBFC_x, int lBFC_y, int lBFC_z, int rTBC_x, int rTBC_y, int rTBC_z) :
    _leftBottomFrontCorner(QVector<int>(0)),
    _rightTopBackCorner(QVector<int>(0)) {

    // Create first and second corners
    _leftBottomFrontCorner << lBFC_x << lBFC_y << lBFC_z;
    _rightTopBackCorner << rTBC_x << rTBC_y << rTBC_z;

    // Calculate bounding box dimensions
    _length = rTBC_x - lBFC_x;
    _width  = rTBC_y - lBFC_y;
    _height = rTBC_z - lBFC_z;
}

void BoundingBox::setValues(int lBFC_x, int lBFC_y, int lBFC_z, int rTBC_x, int rTBC_y, int rTBC_z) {
    // Clear first corner and add new values
    _leftBottomFrontCorner.clear();
    _leftBottomFrontCorner << lBFC_x << lBFC_y << lBFC_z;

    // Clear second corner and add new values
    _rightTopBackCorner.clear();
    _rightTopBackCorner << rTBC_x << rTBC_y << rTBC_z;
}
