#ifndef REVERSETILEDIALOG_H
#define REVERSETILEDIALOG_H

#include "LegoDialog.h"
#include <QtGui>
#include "ReverseTile.h"
#include "ReverseTileNode.h"

class ReverseTileDialog : public LegoDialog {
    Q_OBJECT

public:
    ReverseTileDialog(QWidget* parent = NULL);
    ReverseTileDialog(const ReverseTileDialog& reverseTileDialog);

    int getWidth(void) const { return _widthSpinBox->text().toInt(); }
    int getLength(void) const { return _lengthSpinBox->text().toInt(); }

    virtual ReverseTileDialog* cloning(void) const;

public slots:
    virtual void setLego(int);

private:
    QSpinBox* _widthSpinBox;
    QSpinBox* _lengthSpinBox;
};

#endif // REVERSETILEDIALOG_H
