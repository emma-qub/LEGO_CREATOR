#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>

#include "World.h"
#include "LegoNode.h"


class AddLegoCommand : public QUndoCommand {
public:
    AddLegoCommand(World* world, osg::ref_ptr<LegoNode> legoNode, QUndoCommand* parent = NULL);

    virtual void undo(void);
    virtual void redo(void);

private:
    World* _world;
    //osg::ref_ptr<osg::MatrixTransform> _matTrans;
    osg::ref_ptr<LegoNode> _currLegoNode;
    osg::ref_ptr<Lego> _currLego;
    std::string _matrixName;
};

class DeleteLegoCommand : public QUndoCommand {
public:
    DeleteLegoCommand(World* world, osg::ref_ptr<LegoNode> legoNode, const std::string& matrixName, QUndoCommand* parent = NULL);

    virtual void undo(void);
    virtual void redo(void);

private:
    World* _world;
    //osg::ref_ptr<osg::MatrixTransform> _matTrans;
    osg::ref_ptr<LegoNode> _currLegoNode;
    osg::ref_ptr<Lego> _currLego;
    std::string _matrixName;
};

class MoveLegoCommand : public QUndoCommand {
public:
    MoveLegoCommand(World* world, osg::ref_ptr<LegoNode> legoNode, int x, int y, int z, QUndoCommand* parent = NULL);

    virtual void undo(void);
    virtual void redo(void);

private:
    World* _world;
    osg::ref_ptr<osg::MatrixTransform> _matTrans;
    osg::ref_ptr<LegoNode> _currLegoNode;
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
