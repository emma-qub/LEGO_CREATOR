#include "FromFileNode.h"

#include <osgDB/ReadFile>

FromFileNode::FromFileNode() :
    LegoNode() {
}

FromFileNode::FromFileNode(FromFile *fromFile) :
    LegoNode(fromFile) {

    createGeode();
}

FromFileNode::FromFileNode(const FromFileNode& fromFileNode) :
    LegoNode(fromFileNode) {
}

void FromFileNode::createGeode(void) {
    removeChildren(0, getNumChildren());

    // Get the fromFile
    FromFile* fromFile = static_cast<FromFile*>(_lego);

    // Get file name
    QString fileName = fromFile->getFileName();

    // Read osg file
    addChild(osgDB::readNodeFile(fileName.toStdString()));
}

FromFileNode* FromFileNode::cloning(void) const {
    return new FromFileNode(*this);
}
