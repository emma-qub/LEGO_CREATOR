#ifndef CORNERDIALOG_H
#define CORNERDIALOG_H

#include "LegoDialog.h"
#include <QtGui>
#include "Corner.h"
#include "CornerGeode.h"

class CornerDialog :public LegoDialog {
    Q_OBJECT

public:
    CornerDialog(QWidget* parent = 0);
    CornerDialog(const CornerDialog& brickDialog);

    virtual CornerDialog* cloning(void) const;

public slots:
    virtual void setLego(int);

};

#endif // CORNERDIALOG_H
