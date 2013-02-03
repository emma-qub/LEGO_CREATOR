#include "Brick.h"

#include "QDebug"

Brick::Brick(int width, int length, const QColor &color, BrickType brickType, QObject* parent) :
    Lego(color, parent),
    _width(width),
    _length(length),
    _brickType(brickType) {

    calculateBoundingBox();
}

Brick::Brick(const Brick& brick) :
    Lego(brick) {

    _width = brick._width;
    _length = brick._length;
    _brickType = brick._brickType;

    calculateBoundingBox();
}

void Brick::setBrickType(int index) {
    switch (index) {
    case 0 :
        _brickType = classic;
        break;
    case 1 :
        _brickType = plate;
        break;
    case 2 :
        _brickType = flat;
        break;
    }
}

void Brick::calculateBoundingBox(void) {
    switch (_brickType) {
    case 0:
        _boundingBox = BoundingBox(0, 0, 0, _length, _width, 3);
        break;
    case 1:
        _boundingBox = BoundingBox(0, 0, 0, _length, _width, 1);
        break;
    case 2:
        _boundingBox = BoundingBox(0, 0, 0, _length, _width, 1);
        break;
    }

    qDebug() << "Dans la calculateBoundingBox de Brick";
    for (int k = 0; k < _boundingBox.getLeftBottomFrontCorner().size(); k++)
        qDebug() << QString("%1 %2").arg(k).arg(_boundingBox.getLeftBottomFrontCorner().at(k));
    qDebug() << "Dans la calculateBoundingBox de Brick";
}

Brick* Brick::cloning(void) const {
    return new Brick(*this);
}

QString Brick::whoiam(void) const {
    return "I am a Brick";
}
