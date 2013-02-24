#include "Commands.h"

#include "World.h"

#include <QDebug>


// /////////////////////////////////////////////////////////////////
// AddLegoCommand
// /////////////////////////////////////////////////////////////////

AddLegoCommand::AddLegoCommand(World* world, osg::ref_ptr<osg::MatrixTransform> matTrans, QUndoCommand* parent) :
    QUndoCommand(parent) {

    _world = world;
    _matTrans = matTrans;

    LegoGeode* legoGeode = static_cast<LegoGeode*>(matTrans->getChild(0));
    setText("Add "+legoGeode->getLego()->whoiam());
}

void AddLegoCommand::undo(void) {
    unsigned int matTransIndex = _world->getScene()->getChildIndex(_matTrans);
    _world->deleteLego(matTransIndex);
}

void AddLegoCommand::redo(void) {
    _matTransIndex = _world->addBrick(_matTrans.get());
}
