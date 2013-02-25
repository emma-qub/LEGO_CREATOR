#include "World.h"

#include <QDebug>

#include "LegoFactory.h"

#include <osgUtil/Optimizer>

int World::minHeight = 0;
int World::maxHeight = 100;
int World::minWidth = -500;
int World::maxWidth = 500;
int World::minLength = -500;
int World::maxLength = 500;

World::World() {
    // Initialize matrix transform indexes
    _matTransIndexes = QVector<unsigned int>(0);

    // Create scene
    _scene = new osg::Group;
    _currMatrixTransform = new osg::MatrixTransform;

    // Optimizer
    osgUtil::Optimizer optimizer;
    optimizer.optimize(_scene);
}

World::~World(void) {
}

void World::initBrick(void) {
    // LEGO parts in preview are centered in origin.
    // The goal here is to translate them to match there leftBottomFront corner with the origin.

    // Get current LegoGeode
    LegoGeode* currLegoGeode = static_cast<LegoGeode*>(_currMatrixTransform->getChild(0));

    // Get the Bounding Box
    BoundingBox box = currLegoGeode->getLego()->getBoundingBox();

    // Calculate x, y and z translations
    _x = Lego::length_unit*box.getLength()/2.0;
    _y = Lego::length_unit*box.getWidth()/2.0;
    _z = Lego::height_unit*box.getHeight()/2.0;

    // Translation
    osg::Matrix mat = _currMatrixTransform->getMatrix();
    mat.makeTranslate(_x, _y, _z);
    _currMatrixTransform->setMatrix(mat);

    _isTurned = false;
}

void World::deleteLego(void) {
    _scene->removeChild(_matTransIndexes.last());
    _matTransIndexes.pop_back();
}

void World::deleteLego(unsigned int matTransIndex) {
    _scene->removeChild(matTransIndex);
}

unsigned int World::addBrick(LegoGeode* legoGeode, Lego* lego) {
    osg::ref_ptr<LegoGeode> newLegoGeode = legoGeode->cloning();
    osg::ref_ptr<Lego> newLego = lego->cloning();
    newLegoGeode->setLego(newLego.get());

    _currMatrixTransform = new osg::MatrixTransform;
    _currMatrixTransform->addChild(newLegoGeode.get());
    _scene->addChild(_currMatrixTransform.get());

    initBrick();

    int matTransIndex = _scene->getChildIndex(_currMatrixTransform);
    _matTransIndexes << matTransIndex;
    return matTransIndex;
}

void World::rotation(bool counterClockWise) {
    // Calculate rotation direction
    double direction = 1.0;
    if (counterClockWise)
        direction = -1.0;

    // Rotate according to direction with z axis
    osg::Matrix rotate;
    rotate.makeRotate(osg::Quat(direction*M_PI/2, osg::Vec3(0, 0, 1)));

    // If pairwise is different between width and length, we have to move the Lego of 1/2 Lego unit on x and y axis
    int x = static_cast<int>(2*_x/Lego::length_unit);
    int y = static_cast<int>(2*_y/Lego::length_unit);
    if ((x%2==0 && y%2==1) || (x%2==1 && y%2==0)) {
        osg::Matrix mat = _currMatrixTransform->getMatrix();
        mat.makeTranslate(-Lego::length_unit/2, Lego::length_unit*1/2, 0);
        _currMatrixTransform->preMult(mat);
        _isTurned = !_isTurned;
    }

    // Apply rotation.
    // NB: rotate * mat -> local rotation
    //     mat * rotate -> global rotation
    _currMatrixTransform->preMult(rotate);
}

void World::translationXYZ(double x, double y, double z) {
    x *= Lego::length_unit;
    y *= Lego::length_unit;
    z *= Lego::height_unit;

    osg::Matrix mat = _currMatrixTransform->getMatrix();
    osg::Vec3d trans = mat.getTrans();
    mat.makeTranslate(-trans[0]+x+_x, -trans[1]+y+_y, -trans[2]+z+_z);

    _currMatrixTransform->postMult(mat);

    if (_isTurned) {
        osg::Matrix mat = _currMatrixTransform->getMatrix();
        mat.makeTranslate(Lego::length_unit/2, -Lego::length_unit*1/2, 0);
        _currMatrixTransform->preMult(mat);
    }
}

void World::translation(double x, double y, double z) {
    x *= Lego::length_unit;
    y *= Lego::length_unit;
    z *= Lego::height_unit;

    osg::Matrix mat = _currMatrixTransform->getMatrix();
    mat.makeTranslate(x, y, z);
    _currMatrixTransform->setMatrix(mat);
}
