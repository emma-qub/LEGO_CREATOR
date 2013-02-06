#ifndef CHARACTERGEODE_H
#define CHARACTERGEODE_H

#include <osgDB/ReadFile>
#include "LegoGeode.h"
#include "Character.h"

class CharacterGeode : public LegoGeode
{
public:
    CharacterGeode();
    CharacterGeode(Character* character);
    CharacterGeode(const CharacterGeode& characterGeode);

    virtual void createGeode(void);

    virtual CharacterGeode* cloning(void) const;
};


#endif // CHARACTERGEODE_H
