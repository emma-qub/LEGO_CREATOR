#ifndef TILEGEODE_H
#define TILEGEODE_H

#include "LegoGeode.h"
#include "Tile.h"

class TileGeode : public LegoGeode {

public:
    TileGeode();
    TileGeode(osg::ref_ptr<Tile> tile);
    TileGeode(const TileGeode& tileGeode);

    virtual void createGeode(void);
    osg::ref_ptr<osg::Drawable> createTinyClassic(void) const;
    osg::ref_ptr<osg::Drawable> createClassic(void) const;
    osg::ref_ptr<osg::Drawable> createRoof(void) const;

    virtual TileGeode* cloning(void) const;
};

#endif // TILEGEODE_H
