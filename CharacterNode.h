#ifndef CharacterNode_H
#define CharacterNode_H

#include <osgDB/ReadFile>
#include "LegoNode.h"
#include "Character.h"

class CharacterNode : public LegoNode
{
public:
    CharacterNode();
    CharacterNode(Character* character);
    CharacterNode(const CharacterNode& characterNode);

    virtual void createGeode(void);

    virtual CharacterNode* cloning(void) const;
};


#endif // CharacterNode_H
