#include "CharacterNode.h"
#include "Character.h"

#include <osg/MatrixTransform>
#include <osg/Texture2D>

CharacterNode::CharacterNode() :
    LegoNode() {
}

CharacterNode::CharacterNode(Character *character) :
    LegoNode(character) {

    createGeode();
}

CharacterNode::CharacterNode(const CharacterNode& characterNode) :
    LegoNode(characterNode) {
}

void CharacterNode::createGeode(void) {
    // Remove previous children
    removeChildren(0, getNumChildren());

    // Create matrix transform, because Lego character has to be scale...
    osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
    osg::Matrix m = mt->getMatrix();
    int s = 80;
    m.makeScale(s, s, s);
    mt->setMatrix(m);

    // ...and translated
    int height = _lego->getBoundingBox().getHeight();
    m.makeTranslate(0, 0, -height*Lego::height_unit/2);
    mt->postMult(m);

    // Create node from file
    mt->addChild(osgDB::readNodeFile("../LEGO_CREATOR/OSG/LegoGuy/LegoGuy.osg"));

    // Add matrix transform
    addChild(mt);
}

CharacterNode* CharacterNode::cloning(void) const {
    return new CharacterNode(*this);
}
