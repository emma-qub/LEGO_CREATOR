#include "Grid.h"

Grid::Grid(const QColor& color) :
    Lego(color) {

    calculateBoundingBox();
}

Grid::Grid(const Grid& grid) :
    Lego(grid) {

    calculateBoundingBox();
}

void Grid::calculateBoundingBox(void) {
    _boundingBox = BoundingBox(0, 0, 0, 2, 1, 1);
}

Grid* Grid::cloning(void) const {
    return new Grid(*this);
}

QString Grid::whoiam(void) const {
    return "Grid";
}
