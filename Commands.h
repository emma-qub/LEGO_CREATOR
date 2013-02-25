#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>

#include "World.h"
#include "LegoGeode.h"


class AddLegoCommand : public QUndoCommand {
public:
    AddLegoCommand(World* world, osg::ref_ptr<LegoGeode> legoGeode, QUndoCommand* parent = NULL);

    virtual void undo(void);
    virtual void redo(void);

private:
    World* _world;
    osg::ref_ptr<osg::MatrixTransform> _matTrans;
    osg::ref_ptr<LegoGeode> _currLegoGeode;
    osg::ref_ptr<Lego> _currLego;
    unsigned int _matTransIndex;
};

class DeleteLegoCommand : public QUndoCommand {
public:
    DeleteLegoCommand(World* world, osg::ref_ptr<LegoGeode> legoGeode, QUndoCommand* parent = NULL);

    virtual void undo(void);
    virtual void redo(void);

private:
    World* _world;
    osg::ref_ptr<osg::MatrixTransform> _matTrans;
    osg::ref_ptr<LegoGeode> _currLegoGeode;
    osg::ref_ptr<Lego> _currLego;
    unsigned int _matTransIndex;
};

class MoveLegoCommand : public QUndoCommand {
public:
    MoveLegoCommand(World* world, osg::ref_ptr<LegoGeode> legoGeode, int x, int y, int z, QUndoCommand* parent = NULL);

    virtual void undo(void);
    virtual void redo(void);

private:
    World* _world;
    osg::ref_ptr<osg::MatrixTransform> _matTrans;
    osg::ref_ptr<LegoGeode> _currLegoGeode;
    osg::ref_ptr<Lego> _currLego;
    unsigned int _matTransIndex;
    int _x;
    int _y;
    int _z;
};

class RotateLegoCommand : public QUndoCommand {
public:
    RotateLegoCommand();
};


#endif // COMMANDS_H
