#ifndef FRONTSHIPDIALOG_H
#define FRONTSHIPDIALOG_H

#include "LegoDialog.h"
#include <QtGui>
#include "FrontShip.h"
#include "FrontShipGeode.h"

class FrontShipDialog : public LegoDialog {
    Q_OBJECT

public:
    FrontShipDialog(QWidget* parent = NULL);
    FrontShipDialog(const FrontShipDialog& frontShipDialog);

    virtual FrontShipDialog* cloning(void) const;

    virtual void setLego(int) {}
};

#endif // FRONTSHIPDIALOG_H
