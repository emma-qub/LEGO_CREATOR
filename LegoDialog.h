#ifndef LEGODIALOG_H
#define LEGODIALOG_H

#include <QWidget>
#include "Lego.h"
#include "LegoGeode.h"

class LegoDialog : public QWidget {
    //Q_OBJECT

public:
    LegoDialog(QWidget* parent = 0);
    LegoDialog(const LegoDialog&);

    //virtual void setLego(Lego* lego, LegoGeode* legoGeode) = 0;

    virtual LegoDialog* cloning(void) const = 0;

//signals:
//    void legoChanged(LegoGeode*);
};

#endif // LEGODIALOG_H
