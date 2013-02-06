#include "CharacterDialog.h"

CharacterDialog::CharacterDialog(QWidget* parent) :
    LegoDialog(parent) {
}

CharacterDialog::CharacterDialog(const CharacterDialog& CharacterDialog) :
    LegoDialog(CharacterDialog) {

    // Character type
    _characterTypeComboBox = new QComboBox(this);
    QStringList characterTypeList;
    characterTypeList << "Classic" << "R2D2";
    _characterTypeComboBox->addItems(characterTypeList);
    QFormLayout* characterTypeLayout = new QFormLayout;
    characterTypeLayout->addRow("Character type:", _characterTypeComboBox);

    // Main Layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(characterTypeLayout);

    // Connections
    connect(_characterTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setLego(int)));

    setLayout(mainLayout);
}

void CharacterDialog::reInitComboBox(void) {
    _characterTypeComboBox->setCurrentIndex(0);
}

void CharacterDialog::setLego(int) {
    if (Character* character = dynamic_cast<Character*>(_lego)) {
        if (CharacterGeode* characterGeode = dynamic_cast<CharacterGeode*>(_legoGeode)) {
            character->setCharacterType(_characterTypeComboBox->currentIndex());

            characterGeode->createGeode();

            emit changedLego(characterGeode);
        } else {
            qDebug() << "Cannot cast in CharacterGeode* CharacterDialog::setLego";
        }
    } else {
        qDebug() << "Cannot cast in Character* CharacterDialog::setLego";
    }
}

CharacterDialog* CharacterDialog::cloning(void) const {
    return new CharacterDialog(*this);
}
