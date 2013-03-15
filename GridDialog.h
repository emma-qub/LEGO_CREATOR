#ifndef GRIDDIALOG_H
#define GRIDDIALOG_H

#include "LegoDialog.h"
#include <QtGui>
#include "Grid.h"
#include "GridNode.h"

class GridDialog : public LegoDialog {
    Q_OBJECT

public:
    GridDialog(QWidget* parent = NULL);
    GridDialog(const GridDialog& gridDialog);

    virtual GridDialog* cloning(void) const;

    virtual void setLego(int) {}
};

#endif // GRIDDIALOG_H
