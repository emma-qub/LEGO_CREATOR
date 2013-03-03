#ifndef ROADDIALOG_H
#define ROADDIALOG_H

#include "LegoDialog.h"
#include <QtGui>
#include "Road.h"
#include "RoadNode.h"

class RoadDialog : public LegoDialog {
    Q_OBJECT

public:
    RoadDialog(QWidget* parent = 0);
    RoadDialog(const RoadDialog& roadDialog);

    void reInitComboBox(void);

    virtual RoadDialog* cloning(void) const;

public slots:
    virtual void setLego(int);

private:
    QComboBox* _roadTypeComboBox;
};

#endif // ROADDIALOG_H
