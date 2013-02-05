#ifndef SLOPDIALOG_H
#define SLOPDIALOG_H

#include "LegoDialog.h"
#include <QtGui>
#include "Slop.h"
#include "SlopGeode.h"


class SlopDialog : public LegoDialog {
    Q_OBJECT

    public:
        SlopDialog(QWidget *parent = 0);
        SlopDialog(const SlopDialog& slopDialog);

        int getLength(void) const { return _lengthSpinBox->text().toInt(); }
        int getWidth(void) const { return _widthSpinBox->text().toInt(); }

        void reInitComboBox(void);

        virtual SlopDialog* cloning(void) const;

    public slots:
        virtual void setLego(int);

    private:
        QComboBox* _cornerTypeComboBox;
        QSpinBox* _lengthSpinBox;
        QSpinBox* _widthSpinBox;
        QComboBox* _slopTypeComboBox;
};

#endif // SLOPDIALOG_H
