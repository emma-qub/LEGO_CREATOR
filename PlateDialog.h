#ifndef PLATEDIALOG_H
#define PLATEDIALOG_H

#include "LegoDialog.h"
#include <QtGui>
#include "Plate.h"
#include "PlateGeode.h"

class PlateDialog : public LegoDialog {
    Q_OBJECT

public:
    PlateDialog(QWidget *parent = 0);
    PlateDialog(const PlateDialog& plateDialog);

    int getWidth(void) const { return _widthSpinBox->text().toInt(); }
    int getLength(void) const { return _lengthSpinBox->text().toInt(); }

    virtual PlateDialog* cloning(void) const;

public slots:
    virtual void setLego(int);

private:
    QSpinBox* _widthSpinBox;
    QSpinBox* _lengthSpinBox;

};

#endif // PLATEDIALOG_H
