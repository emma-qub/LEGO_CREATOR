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

    virtual WindowDialog* cloning(void) const;

    virtual void setLego(int) {}
};

#endif // WINDOWDIALOG_H
