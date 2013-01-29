#include "Corner.h"

Corner::Corner(LegoType legoType, const QColor &color, QObject *parent) :
    Lego(color, parent),
    _legoType(legoType) {
}

Corner* Corner::cloning(void) const {
    return new Corner(*this);
}
