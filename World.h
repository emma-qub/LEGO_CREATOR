#ifndef WORLD_H
#define WORLD_H

#include <QVector>

#include <osg/Node>
#include <osg/ref_ptr>
#include <osg/MatrixTransform>
#include <osg/LightSource>

#include "LegoGeode.h"

class World {

public:
    World();
    virtual ~World(void);

    osg::ref_ptr<osg::Group> getScene(void) const { return _scene.get(); }

    bool isBuilding(void) const { return _building; }

    void initBrick(void);
    void fitBrick(void);
    void deleteLego(void);
    bool canBeFit(void) const;
    void addBrick(osg::ref_ptr<LegoGeode> legoGeode);
    void rotation(bool counterClockWise = false);
    void translation(double x, double y, double z);
    void translationXYZ(int x, int y, int z);

    static int minHeight;
    static int maxHeight;
    static int minWidth;
    static int maxWidth;
    static int minLength;
    static int maxLength;

private:
//    QVector<QVector<QVector<bool> > > _brickPositions;
//    QVector<QVector<bool> > _roadPositions;
    bool _building;
    osg::ref_ptr<osg::Group> _scene;
    osg::ref_ptr<osg::MatrixTransform> _currMatrixTransform;
    osg::ref_ptr<LegoGeode> _currLegoGeode;
    QVector<osg::ref_ptr<osg::Node> > _assemblies;
};

#endif // WORLD_H
