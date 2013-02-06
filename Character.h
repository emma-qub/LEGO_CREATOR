#ifndef CHARACTER_H
#define CHARACTER_H

#include "Lego.h"
#include <QColor>

class Character : public Lego
{
public:
    enum CharacterType { classic, R2D2 };
    Character(CharacterType charactertype = classic, QObject* parent = 0);

    CharacterType getCharacterType(void) const { return _characterType; }

    void setCharacterType(CharacterType characterType) { _characterType = characterType; }
    void setCharacterType(int index);

    virtual void calculateBoundingBox(void);

    virtual Character* cloning(void) const;

    virtual QString whoiam(void) const;

private :

    CharacterType _characterType;
};

#endif // CHARACTER_H
