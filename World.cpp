#include "World.h"

#include <QDebug>

World::World() :
    _brickPositions(QVector<QVector<QVector<bool> > >(MAX_HEIGHT, QVector<QVector<bool> >(MAX_LENGTH, QVector<bool>(MAX_WIDTH, false)))),
    _roadPositions(QVector<QVector<bool> >(MAX_LENGTH, QVector<bool>(MAX_WIDTH, false))),
    _building(false),
    _assemblies(QVector<osg::ref_ptr<osg::Node> >()) {

    _scene = new osg::Group;
    _currMatrixTransform = new osg::MatrixTransform;
    _currLegoGeode = NULL;
}

World::~World(void) {
}

void World::addBrick(LegoGeode* legoGeode) {
    _currLegoGeode = legoGeode;

    _currMatrixTransform->removeChildren(0, _currMatrixTransform->getNumChildren());
    _currMatrixTransform->addChild(_currLegoGeode);
    _currMatrixTransform->setMatrix(osg::Matrix());

    _scene->addChild(new osg::Node(*_currMatrixTransform));
}

void World::rotation(bool counterClockWise) {
    osg::Matrix mat;
    double direction = 1.0;
    if (counterClockWise)
        direction = -1.0;
    mat.setRotate(osg::Quat(direction*M_PI/2, osg::Vec3(0, 0, 1)));
    _currMatrixTransform->setMatrix(_currMatrixTransform->getMatrix()*mat);
}

void World::translation(int x, int y, int z) {
    osg::Matrix mat;
    mat.setTrans(x, y, z);
    _currMatrixTransform->setMatrix(_currMatrixTransform->getMatrix()*mat);
}
