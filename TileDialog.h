#ifndef TILEDIALOG_H
#define TILEDIALOG_H

#include "LegoDialog.h"
#include <QtGui>
#include "Tile.h"
#include "TileNode.h"

class TileDialog : public LegoDialog {
    Q_OBJECT

public:
    TileDialog(QWidget* parent = NULL);
    TileDialog(const TileDialog& tileDialog);

    int getWidth(void) const { return _widthSpinBox->text().toInt(); }
    int getLength(void) const { return _lengthSpinBox->text().toInt(); }

    virtual void reInitComboBox(void);

    virtual TileDialog* cloning(void) const;

public slots:
    virtual void setLego(int);
    virtual void updateMaxWidth(int tileType);

private:
    QSpinBox* _widthSpinBox;
    QSpinBox* _lengthSpinBox;
    QComboBox* _tileTypeComboBox;

};

#endif // TILEDIALOG_H
