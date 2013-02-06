#include "CharacterGeode.h"
#include "Character.h"

CharacterGeode::CharacterGeode() :
    LegoGeode() {
}

CharacterGeode::CharacterGeode(Character* Character) :
    LegoGeode(Character) {

    createGeode();
}

CharacterGeode::CharacterGeode(const CharacterGeode& CharacterGeode) :
    LegoGeode(CharacterGeode) {

    _lego = CharacterGeode._lego;
}

void CharacterGeode::createGeode(void) {
    // Add the Character basis
    removeDrawables(0, getDrawableList().size());

    // Get the Character
    Character* character = static_cast<Character*>(_lego);

    // Get Character type
    Character::CharacterType characterType = character->getCharacterType();

    std::string location = "data/LegoGuy/LegoGuy.obj";
    osg::ref_ptr<osg::Geode> guy = dynamic_cast<osg::Geode*>(osgDB::readNodeFile(location));
    this->addDrawable(guy->getDrawable(0));

}

CharacterGeode* CharacterGeode::cloning(void) const {
    return new CharacterGeode(*this);
}
