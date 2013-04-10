#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <QVector>

#include <osg/Vec3>

class BoundingBox
{
public:
    BoundingBox(int lBFC_x = 0, int lBFC_y = 0, int lBFC_z = 0, int rTBC_x = 0, int rTBC_y = 0, int rTBC_z = 0);

    QVector<int> getLeftBottomFrontCorner(void) const { return _leftBottomFrontCorner; }
    QVector<int> getRightTopBackCorner(void) const { return _rightTopBackCorner; }
    void setValues(int lBFC_x, int lBFC_y, int lBFC_z, int rTBC_x, int rTBC_y, int rTBC_z);

    int getWidth(void) const { return _width; }
    int getLength(void) const { return _length; }
    int getHeight(void) const { return _height; }

    osg::Vec3 center(void) { return osg::Vec3((_rightTopBackCorner.at(0) + _leftBottomFrontCorner.at(0)) / 2,
                                              (_rightTopBackCorner.at(1) + _leftBottomFrontCorner.at(1)) / 2,
                                              (_rightTopBackCorner.at(2) + _leftBottomFrontCorner.at(2)) / 2); }

private:
    QVector<int> _leftBottomFrontCorner;
    QVector<int> _rightTopBackCorner;
    int _width;
    int _length;
    int _height;
};

#endif // BOUNDINGBOX_H
