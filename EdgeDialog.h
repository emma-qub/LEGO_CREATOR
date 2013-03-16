#ifndef EDGEDIALOG_H
#define EDGEDIALOG_H

#include "LegoDialog.h"
#include <QtGui>
#include "Edge.h"
#include "EdgeNode.h"

class EdgeDialog : public LegoDialog {

public:
    EdgeDialog(QWidget* parent = NULL);
    EdgeDialog(const EdgeDialog& edgeDialog);

    int getCurrentEdgeTypeIndex(void) const { return _edgeTypeComboBox->currentIndex(); }

    virtual void reInitComboBox(void);

    virtual EdgeDialog* cloning(void) const;

public slots:
    virtual void setLego(int);

private:
    QComboBox* _edgeTypeComboBox;

};

#endif // EDGEDIALOG_H
