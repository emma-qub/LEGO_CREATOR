#ifndef CORNERDIALOG_H
#define CORNERDIALOG_H

#include "LegoDialog.h"
#include <QtGui>
#include "Corner.h"
#include "CornerNode.h"

class CornerDialog :public LegoDialog {
    Q_OBJECT

public:
    CornerDialog(QWidget* parent = 0);
    CornerDialog(const CornerDialog& cornerDialog);

    void reInitComboBox(void);

    virtual CornerDialog* cloning(void) const;

public slots:
    virtual void setLego(int);

private:
    QComboBox* _cornerTypeComboBox;
};

#endif // CORNERDIALOG_H
