#ifndef DOORDIALOG_H
#define DOORDIALOG_H

#include "LegoDialog.h"
#include <QtGui>
#include "Door.h"
#include "DoorNode.h"

class DoorDialog : public LegoDialog {
    Q_OBJECT

public:
    DoorDialog(QWidget* parent = NULL);
    DoorDialog(const DoorDialog& doorDialog);

    virtual DoorDialog* cloning(void) const;

    virtual void setLego(int);

private slots:
    void browseDoorColor(void);
    void browseDoorHandleColor(void);

private:
    QPushButton* _doorColorButton;
    QColor _doorColor;
    QPushButton* _doorHandleColorButton;
    QColor _doorHandleColor;
};

#endif // DOORDIALOG_H
