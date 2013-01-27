#ifndef BRICKDIALOG_H
#define BRICKDIALOG_H

#include "LegoDialog.h"
#include <QtGui>
#include "Brick.h"
#include "BrickGeode.h"

class BrickDialog : public LegoDialog {
    Q_OBJECT

public:
    BrickDialog(QWidget *parent = 0);
    BrickDialog(const BrickDialog& brickDialog);

    virtual BrickDialog* cloning(void) const;

public slots:
    virtual void setLego(int);

private:
    QSpinBox* _widthSpinBox;
    QSpinBox* _lengthSpinBox;

};

#endif // BRICKDIALOG_H
