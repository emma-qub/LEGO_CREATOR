#ifndef BRICKDIALOG_H
#define BRICKDIALOG_H

#include "LegoDialog.h"
#include <QtGui>
#include "Brick.h"
#include "BrickGeode.h"

class BrickDialog : public LegoDialog {
    //Q_OBJECT

public:
    BrickDialog(QWidget *parent = 0);
    BrickDialog(const BrickDialog& brickDialog);

//    int getWidthSpinBox(void) const;
//    int getLengthSpinBox(void) const;

//    virtual void setLego(Lego* lego, LegoGeode* legoGeode);

    virtual BrickDialog* cloning(void) const;

private:
    QSpinBox* _widthSpinBox;
    QSpinBox* _lengthSpinBox;

};

#endif // BRICKDIALOG_H
