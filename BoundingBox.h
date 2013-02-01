#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <QVector>

class BoundingBox
{
public:
    BoundingBox(int lBFC_x = 0, int lBFC_y = 0, int lBFC_z = 0, int rTBC_x = 0, int rTBC_y = 0, int rTBC_z = 0);

    QVector<int> getLeftBottomFrontCorner(void) const { return _leftBottomFrontCorner; }
    QVector<int> getRightTopBackCorner(void) const { return _rightTopBackCorner; }

private:
    QVector<int> _leftBottomFrontCorner;
    QVector<int> _rightTopBackCorner;
};

#endif // BOUNDINGBOX_H
