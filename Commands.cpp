#include "Commands.h"

#include "World.h"


// /////////////////////////////////////////////////////////////////
// AddLegoCommand
// /////////////////////////////////////////////////////////////////

AddLegoCommand::AddLegoCommand(World* world, osg::ref_ptr<LegoNode> legoNode, QUndoCommand* parent) :
    QUndoCommand(parent),
    _matrixName() {

    _world = world;
    _currLegoNode = legoNode->cloning();
    _currLego = legoNode->getLego()->cloning();
    _currLegoNode->setLego(_currLego);

    setText("Add "+_currLegoNode->getLego()->whoiam());
}

void AddLegoCommand::undo(void) {
    _world->deleteLego(_matrixName);
}

void AddLegoCommand::redo(void) {
    _matrixName = _world->addBrick(_currLegoNode.get(), _currLego.get());
}


// /////////////////////////////////////////////////////////////////
// DeleteLegoCommand
// /////////////////////////////////////////////////////////////////

DeleteLegoCommand::DeleteLegoCommand(World* world, osg::ref_ptr<LegoNode> legoNode,
                                     const std::string& matrixName, QUndoCommand* parent) :
    QUndoCommand(parent),
    _matrixName(matrixName) {

    _world = world;
    _currLegoNode = legoNode->cloning();
    _currLego = legoNode->getLego()->cloning();
    _currLegoNode->setLego(_currLego);

    setText("Del "+_currLegoNode->getLego()->whoiam());
}

void DeleteLegoCommand::undo(void) {
    _matrixName = _world->addBrick(_currLegoNode.get(), _currLego.get());
}

void DeleteLegoCommand::redo(void) {
    _world->deleteLego(_matrixName);
}


// /////////////////////////////////////////////////////////////////
// MoveLegoCommand
// /////////////////////////////////////////////////////////////////

MoveLegoCommand::MoveLegoCommand(World* world, osg::ref_ptr<LegoNode> legoNode, int x, int y, int z, QUndoCommand* parent) :
    QUndoCommand(parent),

    _x(x),
    _y(y),
    _z(z) {

    _world = world;
    _currLegoNode = legoNode->cloning();
    _currLego = legoNode->getLego()->cloning();
    _currLegoNode->setLego(_currLego);

    setText("Del "+_currLegoNode->getLego()->whoiam());
}

void MoveLegoCommand::undo(void) {
    //_matTransIndex = _world->addBrick(_currLegoNode.get(), _currLego.get());
}

void MoveLegoCommand::redo(void) {
    //_world->translation(_matTransIndex);
}
