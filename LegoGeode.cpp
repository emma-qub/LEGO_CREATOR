#include "LegoGeode.h"

LegoGeode::LegoGeode(Lego* lego) :
    osg::Geode() {
    _lego = lego;
}

LegoGeode::LegoGeode(const LegoGeode& legoGeode) :
    osg::Geode(legoGeode) {
}
