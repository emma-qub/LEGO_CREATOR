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

    int getWidth(void) const { return _widthSpinBox->text().toInt(); }
    int getLength(void) const { return _lengthSpinBox->text().toInt(); }

    virtual BrickDialog* cloning(void) const;

public slots:
    virtual void setLego(int);
    virtual void updateMaxWidth(int brickType);

private:
    QSpinBox* _widthSpinBox;
    QSpinBox* _lengthSpinBox;
    QComboBox* _brickTypeComboBox;

};

#endif // BRICKDIALOG_H
