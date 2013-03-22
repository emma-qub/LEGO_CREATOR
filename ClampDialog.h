#ifndef CLAMPDIALOG_H
#define CLAMPDIALOG_H

#include "LegoDialog.h"
#include <QtGui>
#include "Clamp.h"
#include "ClampNode.h"

class ClampDialog : public LegoDialog {
    Q_OBJECT
    
public:
    ClampDialog(QWidget* parent = NULL);
    ClampDialog(const ClampDialog& clampDialog);
    
    virtual ClampDialog* cloning(void) const;
    
public slots:
    virtual void setLego(int);
    
private:
    
};

#endif // CLAMPDIALOG_H

