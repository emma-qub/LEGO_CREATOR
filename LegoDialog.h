#ifndef LEGODIALOG_H
#define LEGODIALOG_H

#include <QWidget>
#include "Lego.h"
#include "LegoGeode.h"

class LegoDialog : public QWidget {
    Q_OBJECT

public:
    LegoDialog(QWidget* parent = 0);
    LegoDialog(const LegoDialog&);

    virtual void initLego(Lego* lego) { _lego = lego; }
    virtual void initLegoGeode(LegoGeode* legoGeode) { _legoGeode = legoGeode; }

    virtual LegoDialog* cloning(void) const = 0;

public slots:
    virtual void setLego(int) = 0;

protected:
    Lego* _lego;
    LegoGeode* _legoGeode;

signals:
    void changedLego(LegoGeode*);
};

#endif // LEGODIALOG_H
