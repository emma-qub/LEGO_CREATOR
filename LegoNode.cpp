#include "LegoNode.h"

LegoNode::LegoNode(Lego* lego) :
    osg::Node() {
    _lego = lego;
}

LegoNode::~LegoNode() {
    delete _lego;
}
