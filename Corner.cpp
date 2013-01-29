#include "Corner.h"

Corner::Corner(CornerType cornerType, const QColor &color, QObject *parent) :
    Lego(color, parent),
    _cornerType(cornerType) {
}

Corner* Corner::cloning(void) const {
    return new Corner(*this);
}
