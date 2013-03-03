#ifndef LEGODIALOG_H
#define LEGODIALOG_H

#include <QWidget>
#include "Lego.h"
#include "LegoNode.h"

class LegoDialog : public QWidget {
    Q_OBJECT

public:
    LegoDialog(QWidget* parent = NULL);
    LegoDialog(const LegoDialog&);

    virtual void initLego(Lego* lego) { _lego = lego; }
    virtual void initLegoNode(LegoNode* legoNode) { _legoNode = legoNode; }
    virtual void reInitComboBox() {}

    virtual LegoDialog* cloning(void) const = 0;

public slots:
    virtual void setLego(int) = 0;

protected:
    Lego* _lego;
    LegoNode* _legoNode;

signals:
    void changedLego(LegoNode*);
};

#endif // LEGODIALOG_H
