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
