#include "LegoGeode.h"

LegoGeode::LegoGeode(Lego* lego) :
    osg::Geode() {
    _lego = lego;
}

LegoGeode::~LegoGeode() {
    delete _lego;
}
