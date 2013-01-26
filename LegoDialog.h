#ifndef LEGODIALOG_H
#define LEGODIALOG_H

#include <QWidget>

class LegoDialog : public QWidget {

public:
    LegoDialog(QWidget *parent = 0);

    virtual void createDialog(void) = 0;

};

#endif // LEGODIALOG_H
