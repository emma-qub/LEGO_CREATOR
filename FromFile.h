#ifndef FROMFILE_H
#define FROMFILE_H

#include "Lego.h"

class FromFile : public Lego {

public:
    FromFile(const QString& fileName = "", const QColor &color = QColor(Qt::red));
    FromFile(const FromFile& fromFile);

    QString getFileName(void) const { return _fileName; }
    void setFileName(const QString& fileName) { _fileName = fileName; }

    virtual void calculateBoundingBox(void) {}

    virtual FromFile* cloning(void) const;

    virtual QString whoiam(void) const;

private:
    QString _fileName;

};

#endif // FROMFILE_H
