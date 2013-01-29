#ifndef CORNER_H
#define CORNER_H

#include "Lego.h"

class Corner : public Lego {

public:
    enum CornerType { brick, plate };

    Corner(CornerType cornerType = brick, const QColor& color = QColor(Qt::red), QObject* parent = 0);

    CornerType getCornerType(void) const { return _cornerType; }

    void setCornerType(CornerType cornerType) { _cornerType = cornerType; }
    void setCornerType(int index) { index == 0 ? _cornerType = brick : _cornerType = plate; }

    int calculateHeight(void) const { return _cornerType == brick ? 3 : 1; }

    virtual Corner* cloning(void) const;

private:
    CornerType _cornerType;
};

#endif // CORNER_H
