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

private:
    QComboBox* _windowTypeComboBox;
};

#endif // WINDOWDIALOG_H
