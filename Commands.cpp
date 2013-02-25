#include "Commands.h"

#include "World.h"

#include <QDebug>


// /////////////////////////////////////////////////////////////////
// AddLegoCommand
// /////////////////////////////////////////////////////////////////

AddLegoCommand::AddLegoCommand(World* world, osg::ref_ptr<LegoGeode> legoGeode, QUndoCommand* parent) :
    QUndoCommand(parent) {

    _world = world;
    _currLegoGeode = legoGeode->cloning();
    _currLego = legoGeode->getLego()->cloning();
    _currLegoGeode->setLego(_currLego);

    setText("Add "+_currLegoGeode->getLego()->whoiam());
}

void AddLegoCommand::undo(void) {
    _world->deleteLego(_matTransIndex);
}

void AddLegoCommand::redo(void) {
    _matTransIndex = _world->addBrick(_currLegoGeode.get(), _currLego.get());
}


// /////////////////////////////////////////////////////////////////
// DeleteLegoCommand
// /////////////////////////////////////////////////////////////////

DeleteLegoCommand::DeleteLegoCommand(World* world, osg::ref_ptr<LegoGeode> legoGeode, QUndoCommand* parent) :
    QUndoCommand(parent),
    _matTransIndex(-1) {

    _world = world;
    _currLegoGeode = legoGeode->cloning();
    _currLego = legoGeode->getLego()->cloning();
    _currLegoGeode->setLego(_currLego);

    setText("Del "+_currLegoGeode->getLego()->whoiam());
}

void DeleteLegoCommand::undo(void) {
    _matTransIndex = _world->addBrick(_currLegoGeode.get(), _currLego.get());
}

void DeleteLegoCommand::redo(void) {
    _world->deleteLego();
}


// /////////////////////////////////////////////////////////////////
// MoveLegoCommand
// /////////////////////////////////////////////////////////////////

MoveLegoCommand::MoveLegoCommand(World* world, osg::ref_ptr<LegoGeode> legoGeode, int x, int y, int z, QUndoCommand* parent) :
    QUndoCommand(parent),

    _x(x),
    _y(y),
    _z(z) {

    _world = world;
    _currLegoGeode = legoGeode->cloning();
    _currLego = legoGeode->getLego()->cloning();
    _currLegoGeode->setLego(_currLego);

    setText("Del "+_currLegoGeode->getLego()->whoiam());
}

void MoveLegoCommand::undo(void) {
    _matTransIndex = _world->addBrick(_currLegoGeode.get(), _currLego.get());
}

void MoveLegoCommand::redo(void) {
    //_world->translation(_matTransIndex);
}
