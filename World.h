#ifndef WORLD_H
#define WORLD_H

#include <QVector>

#include <osg/Node>
#include <osg/ref_ptr>
#include <osg/MatrixTransform>

#include "LegoGeode.h"

#define MAX_HEIGHT  3//100
#define MAX_WIDTH  4//1000
#define MAX_LENGTH 4//1000

class World {

public:
    World();
    virtual ~World(void);

    bool isBuilding(void) const { return _building; }

    void fitBrick(void);
    bool canBeFit(void) const;
    void addBrick(LegoGeode* legoGeode);
    void rotation(bool counterClockWise = false);
    void translation(int x, int y, int z);

private:
    QVector<QVector<QVector<bool> > > _brickPositions;
    QVector<QVector<bool> > _roadPositions;
    bool _building;
    osg::ref_ptr<osg::Group> _scene;
    osg::ref_ptr<osg::MatrixTransform> _currMatrixTransform;
    LegoGeode* _currLegoGeode;
    QVector<osg::ref_ptr<osg::Node> > _assemblies;
};

#endif // WORLD_H
