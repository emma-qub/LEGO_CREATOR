#include "World.h"

#include <QDebug>

#include "LegoFactory.h"

int World::minHeight = 0;
int World::maxHeight = 100;
int World::minWidth = -500;
int World::maxWidth = 500;
int World::minLength = -500;
int World::maxLength = 500;

World::World() :
//    _brickPositions(QVector<QVector<QVector<bool> > >(maxHeight-minHeight, QVector<QVector<bool> >(maxLength-minLength, QVector<bool>(maxWidth-minWidth, false)))),
//    _roadPositions(QVector<QVector<bool> >(maxLength-minLength, QVector<bool>(maxWidth-minWidth, false))),
    _building(false),
    _assemblies(QVector<osg::ref_ptr<osg::Node> >()) {

    _scene = new osg::Group;
    _currMatrixTransform = new osg::MatrixTransform;
    _currLegoGeode = NULL;
}

World::~World(void) {
}

void World::initBrick(void) {
    // LEGO parts in preview are centered in origin.
    // The goal here is to translate them to match there leftBottomFront corner with the origin.

    // Get the Bounding Box
    BoundingBox box = _currLegoGeode->getLego()->getBoundingBox();

    // Get corners
    QVector<int> lBFC = box.getLeftBottomFrontCorner();
    QVector<int> rTBC = box.getRightTopBackCorner();

    // Calculate x, y and z translations
    double x = (rTBC.at(0) - lBFC.at(0))/2.0;
    double y = (rTBC.at(1) - lBFC.at(1))/2.0;
    double z = (rTBC.at(2) - lBFC.at(2))/2.0;

    // Translation
    translation(x, y, z);
}

void World::addBrick(LegoGeode* legoGeode) {
//    Lego* newLego = NULL;
//    if ((newLego = dynamic_cast<Lego*>(LegoFactory<Lego, QString>::instance()->create("Lego")))) {
//        _currLegoGeode = LegoFactory<LegoGeode, QString>::instance()->create("LegoGeode");
//        _currLegoGeode = legoGeode;
//        _currLegoGeode->setLego(newLego);

//        osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
//        _currMatrixTransform = mt;

//        mt->addChild(_currLegoGeode);
//        _scene->addChild(mt.get());

//        initBrick();
//    } else {
//        qDebug() << "Cannot cast in Lego* within World::addBrick";
//    }


    Lego* newLego = legoGeode->getLego()->cloning();
    _currLegoGeode = legoGeode->cloning();
    _currLegoGeode->setLego(newLego);

    osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
    _currMatrixTransform = mt;

    mt->addChild(_currLegoGeode);
    _scene->addChild(mt.get());

    initBrick();
}

void World::rotation(bool counterClockWise) {
    // Calculate rotation direction
    double direction = 1.0;
    if (counterClockWise)
        direction = -1.0;

    // Rotate according to direction with z axis
    osg::Matrix rotate;
    rotate.makeRotate(osg::Quat(direction*M_PI/2, osg::Vec3(0, 0, 1)));

    // Apply rotation.
    // NB: rotate * mat -> local rotation
    //     mat * rotate -> global rotation
    _currMatrixTransform->setMatrix(rotate*_currMatrixTransform->getMatrix());

    // Update bounding box
    _currLegoGeode->getLego()->getBoundingBox().rotate();
}

void World::translationXYZ(int x, int y, int z) {
    _currLegoGeode->getLego()->translateBoundingBox(x, y, z);

    x *= Lego::length_unit;
    y *= Lego::length_unit;
    z *= Lego::height_unit;

    initBrick();

    osg::Matrix mat = _currMatrixTransform->getMatrix();
    mat.makeTranslate(x, y, z);

    _currMatrixTransform->setMatrix(mat*_currMatrixTransform->getMatrix());
}

void World::translation(double x, double y, double z) {
    x *= Lego::length_unit;
    y *= Lego::length_unit;
    z *= Lego::height_unit;

    osg::Matrix mat = _currMatrixTransform->getMatrix();
    mat.makeTranslate(x, y, z);
    _currMatrixTransform->setMatrix(mat);
}
