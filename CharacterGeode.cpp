#include "CharacterGeode.h"
#include "Character.h"

#include <osg/MatrixTransform>
#include <osg/Texture2D>

CharacterGeode::CharacterGeode() :
    LegoGeode() {
}

CharacterGeode::CharacterGeode(osg::ref_ptr<Character> character) :
    LegoGeode(character) {

    createGeode();
}

CharacterGeode::CharacterGeode(const CharacterGeode& characterGeode) :
    LegoGeode(characterGeode) {
}

void CharacterGeode::createGeode(void) {
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

CharacterGeode* CharacterGeode::cloning(void) const {
    return new CharacterGeode(*this);
}
