#ifndef LEGODIALOG_H
#define LEGODIALOG_H

#include <QWidget>

class LegoDialog : public QWidget {

public:
    LegoDialog(QObject* parent = 0);

    virtual createDialog(void) = 0;

};

#endif // LEGODIALOG_H
