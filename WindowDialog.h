#ifndef WINDOWDIALOG_H
#define WINDOWDIALOG_H

#include "LegoDialog.h"
#include <QtGui>
#include "Window.h"
#include "WindowNode.h"

class WindowDialog : public LegoDialog {
    Q_OBJECT

public:
    WindowDialog(QWidget* parent = NULL);
    WindowDialog(const WindowDialog& windowDialog);

    virtual void reInitComboBox(void);

    virtual WindowDialog* cloning(void) const;

public slots:
    virtual void setLego(int);
    virtual void pannelVisible(int);
    virtual void isPannelUsed(void);

private:
    QComboBox* _windowTypeComboBox;
    QCheckBox* _useLeftPannel;
    QCheckBox* _useRightPannel;
};

#endif // WINDOWDIALOG_H
