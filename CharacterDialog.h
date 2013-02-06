#ifndef CHARACTERDIALOG_H
#define CHARACTERDIALOG_H

#include "LegoDialog.h"
#include "Character.h"
#include <QtGui>
#include "CharacterGeode.h"

class CharacterDialog : public LegoDialog
{
    Q_OBJECT

public:
    CharacterDialog(QWidget* parent = 0);
    CharacterDialog(const CharacterDialog& CharacterDialog);

    void reInitComboBox(void);

    virtual CharacterDialog* cloning(void) const;

public slots:
    virtual void setLego(int);

private:
    QComboBox* _characterTypeComboBox;
};

#endif // CHARACTERDIALOG_H
