#include "FromFile.h"

FromFile::FromFile(const QString& fileName, const QColor &color) :
    Lego(color),
    _fileName(fileName) {
}

FromFile::FromFile(const FromFile& fromFile) :
    Lego(fromFile) {

    _fileName = fromFile._fileName;
}

FromFile* FromFile::cloning(void) const {
    return new FromFile(*this);
}

QString FromFile::whoiam(void) const {
    return "file: "+_fileName;
}
