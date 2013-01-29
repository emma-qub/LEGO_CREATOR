#ifndef CORNER_H
#define CORNER_H

#include "Lego.h"

class Corner : public Lego {

public:
    enum LegoType { brick, plate };

    Corner(LegoType legoType = brick, const QColor& color = QColor(Qt::red), QObject* parent = 0);

    LegoType getLegoType(void) const { return _legoType; }
    int calculateHeight(void) const { if (_legoType == plate) return 1; else return 3; }

    virtual Corner* cloning(void) const;

private:
    LegoType _legoType;
};

#endif // CORNER_H
