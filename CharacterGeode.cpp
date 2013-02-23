#include "CharacterGeode.h"
#include "Character.h"

#include <osg/MatrixTransform>
#include <osg/Texture2D>

#include <QDebug>

CharacterGeode::CharacterGeode() :
    LegoGeode() {
}

CharacterGeode::CharacterGeode(osg::ref_ptr<Character> character) :
    LegoGeode(character) {

    createGeode();
}

CharacterGeode::CharacterGeode(const CharacterGeode& characterGeode) :
    LegoGeode(characterGeode) {

    _lego = characterGeode._lego;
}

void CharacterGeode::createGeode(void) {
    removeChildren(0, getNumChildren());

    osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
    osg::Matrix m = mt->getMatrix();
    int s = 80;
    m.makeScale(s, s, s);
    mt->setMatrix(m);

    mt->addChild(osgDB::readNodeFile("../LEGO_CREATOR/OSG/LegoGuy/LegoGuy.osg"));
    addChild(mt);
}

CharacterGeode* CharacterGeode::cloning(void) const {
    return new CharacterGeode(*this);
}
