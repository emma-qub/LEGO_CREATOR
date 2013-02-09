#include "CharacterGeode.h"
#include "Character.h"

#include <QDebug>

CharacterGeode::CharacterGeode() :
    LegoGeode() {
}

CharacterGeode::CharacterGeode(Character* character) :
    LegoGeode(character) {

    createGeode();
}

CharacterGeode::CharacterGeode(const CharacterGeode& characterGeode) :
    LegoGeode(characterGeode) {

    _lego = characterGeode._lego;
}

void CharacterGeode::createGeode(void) {
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    addChild(geode);

    // Add the Character basis
    geode->removeDrawables(0, geode->getDrawableList().size());

    qDebug() << _lego->whoiam();

    // Get the Character
    //Character* character = static_cast<Character*>(_lego);

    // Get Character type
    //Character::CharacterType characterType = character->getCharacterType();

    std::string location = "../LEGO_CREATOR/data/LegoGuy/LegoGuy.obj";
    if (osg::ref_ptr<osg::Geode> guy = dynamic_cast<osg::Geode*>(osgDB::readNodeFile(location))) {
        geode->addDrawable(guy->getDrawable(0));
    } else {
        qDebug() << "Cannot set LEGO character from file within CharacterGeode::createGeode";
    }
}

CharacterGeode* CharacterGeode::cloning(void) const {
    return new CharacterGeode(*this);
}
