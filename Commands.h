#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>

#include "World.h"
#include "LegoGeode.h"


class AddLegoCommand : public QUndoCommand {
public:
    AddLegoCommand(World* world, osg::ref_ptr<osg::MatrixTransform> matTrans, QUndoCommand* parent = NULL);

    virtual void undo(void);
    virtual void redo(void);

private:
    World* _world;
    osg::ref_ptr<osg::MatrixTransform> _matTrans;
    unsigned int _matTransIndex;
};

class DeleteLegoCommand : public QUndoCommand {
public:
    DeleteLegoCommand();
};

class MoveLegoCommand : public QUndoCommand {
public:
    MoveLegoCommand();
};

class RotateLegoCommand : public QUndoCommand {
public:
    RotateLegoCommand();
};


#endif // COMMANDS_H
