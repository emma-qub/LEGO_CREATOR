#ifndef FROMFILEDIALOG_H
#define FROMFILEDIALOG_H

#include "LegoDialog.h"
#include <QtGui>
#include "FromFile.h"
#include "FromFileGeode.h"

class FromFileDialog : public LegoDialog {
    Q_OBJECT

public:
    FromFileDialog(QWidget* parent = NULL);
    FromFileDialog(const FromFileDialog& fromFileDialog);

    virtual FromFileDialog* cloning(void) const;

    virtual void setLego(int) {}
};

#endif // FROMFILEDIALOG_H
