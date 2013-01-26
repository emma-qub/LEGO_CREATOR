#ifndef LEGONODE_H
#define LEGONODE_H

#include <osg/Node>
#include <osg/ref_ptr>

#include "Lego.h"

class LegoNode : public osg::Node {

public:
    LegoNode(Lego* lego);
    virtual ~LegoNode();

    virtual void createNode(void) = 0;

private:
    Lego* _lego;
};

#endif // LEGONODE_H
