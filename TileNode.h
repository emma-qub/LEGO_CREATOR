#ifndef TileNode_H
#define TileNode_H

#include "LegoNode.h"
#include "Tile.h"

class TileNode : public LegoNode {

public:
    TileNode();
    TileNode(osg::ref_ptr<Tile> tile);
    TileNode(const TileNode& tileNode);

    virtual void createGeode(void);
    osg::Drawable* createTinyClassic(void) const;
    osg::Drawable* createClassic(void) const;
    osg::Drawable* createCornerInt(void) const;
    osg::Drawable* createCornerExt(void) const;
    osg::Drawable* createRoof(void) const;

    virtual TileNode* cloning(void) const;
};

#endif // TileNode_H
