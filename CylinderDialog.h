#ifndef CYLINDERDIALOG_H
#define CYLINDERDIALOG_H

#include "LegoDialog.h"
#include <QtGui>
#include "Cylinder.h"
#include "CylinderNode.h"

class CylinderDialog : public LegoDialog {

public:
    CylinderDialog(QWidget* parent = NULL);
    CylinderDialog(const CylinderDialog& cylinderDialog);

    int getCurrentCylinderTypeIndex(void) const { return _cylinderTypeComboBox->currentIndex(); }

    virtual void reInitComboBox(void);

    virtual CylinderDialog* cloning(void) const;

public slots:
    virtual void setLego(int);

private:
    QComboBox* _cylinderTypeComboBox;

};

#endif // CYLINDERDIALOG_H
