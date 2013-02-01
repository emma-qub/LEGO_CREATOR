#include "BoundingBox.h"

BoundingBox::BoundingBox(int lBFC_x, int lBFC_y, int lBFC_z, int rTBC_x, int rTBC_y, int rTBC_z) :
    _leftBottomFrontCorner(QVector<int>(3)),
    _rightTopBackCorner(QVector<int>(3)) {

    _leftBottomFrontCorner << lBFC_x << lBFC_y << lBFC_z;
    _rightTopBackCorner << rTBC_x << rTBC_y << rTBC_z;
}
