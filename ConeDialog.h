#ifndef CONEDIALOG_H
#define CONEDIALOG_H

#include "LegoDialog.h"
#include <QtGui>
#include "Cone.h"
#include "ConeNode.h"

class ConeDialog : public LegoDialog {
    Q_OBJECT

public:
    ConeDialog(QWidget* parent = NULL);
    ConeDialog(const ConeDialog& coneDialog);

    virtual ConeDialog* cloning(void) const;

    virtual void setLego(int) {}
};

#endif // CONEDIALOG_H
