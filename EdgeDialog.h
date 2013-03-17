#ifndef EDGEDIALOG_H
#define EDGEDIALOG_H

#include "LegoDialog.h"
#include <QtGui>
#include "Edge.h"
#include "EdgeNode.h"

class EdgeDialog : public LegoDialog {
    Q_OBJECT

public:
    EdgeDialog(QWidget* parent = NULL);
    EdgeDialog(const EdgeDialog& edgeDialog);

    int getLength(void) const { return _lengthSpinBox->text().toInt(); }
    int getCurrentEdgeTypeIndex(void) const { return _edgeTypeComboBox->currentIndex(); }

    virtual void reInitComboBox(void);

    virtual EdgeDialog* cloning(void) const;

public slots:
    virtual void setLego(int);
    virtual void updateMaxLength(int edgeType);

private:
    QSpinBox* _lengthSpinBox;
    QComboBox* _edgeTypeComboBox;
    QGroupBox* _lengthGroupBox;

};

#endif // EDGEDIALOG_H
