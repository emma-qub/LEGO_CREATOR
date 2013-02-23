#include "FromFileGeode.h"

#include <osgDB/ReadFile>

FromFileGeode::FromFileGeode() :
    LegoGeode() {
}

FromFileGeode::FromFileGeode(osg::ref_ptr<FromFile> fromFile) :
    LegoGeode(fromFile) {

    createGeode();
}

FromFileGeode::FromFileGeode(const FromFileGeode& fromFileGeode) :
    LegoGeode(fromFileGeode) {
}

void FromFileGeode::createGeode(void) {
    removeChildren(0, getNumChildren());

    // Get the fromFile
    FromFile* fromFile = static_cast<FromFile*>(_lego);

    // Get file name
    QString fileName = fromFile->getFileName();

    // Read osg file
    addChild(osgDB::readNodeFile(fileName.toStdString()));
}

FromFileGeode* FromFileGeode::cloning(void) const {
    return new FromFileGeode(*this);
}
