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
    // Add the Character basis
    removeDrawables(0, getDrawableList().size());

    qDebug() << _lego->whoiam();

    // Get the Character
    //Character* character = static_cast<Character*>(_lego);

    // Get Character type
    //Character::CharacterType characterType = character->getCharacterType();

    std::string location = "../LEGO_CREATOR/data/LegoGuy/LegoGuy.obj";
    if (osg::ref_ptr<osg::Geode> guy = dynamic_cast<osg::Geode*>(osgDB::readNodeFile(location))) {
        this->addDrawable(guy->getDrawable(0));
    } else {
        qDebug() << "Cannot set LEGO character from file within CharacterGeode::createGeode";
    }
}

CharacterGeode* CharacterGeode::cloning(void) const {
    return new CharacterGeode(*this);
}
