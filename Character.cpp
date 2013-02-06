#include "Character.h"
#include <QObject>

Character::Character(CharacterType charactertype, QObject *parent) :  Lego(QColor(), parent), _characterType(charactertype) {
     calculateBoundingBox();
}

void Character::calculateBoundingBox(void) {

}

void Character::setCharacterType(int index) {
    switch (index) {
    case 0 :
        _characterType = classic;
        break;
    case 1 :
        _characterType = R2D2;
        break;
    }
}

Character* Character::cloning(void) const {
    return new Character(*this);
}

QString Character::whoiam(void) const {
    return "I am a Character";
}
