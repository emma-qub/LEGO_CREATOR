#ifndef WHEELDIALOG_H
#define WHEELDIALOG_H

#include "LegoDialog.h"
#include <QtGui>
#include "Wheel.h"
#include "WheelGeode.h"

class WheelDialog : public LegoDialog {
    Q_OBJECT

public:
    WheelDialog(QWidget* parent = NULL);
    WheelDialog(const WheelDialog& wheelDialog);

    virtual WheelDialog* cloning(void) const;

    virtual void setLego(int) {}
};

#endif // WHEELDIALOG_H
