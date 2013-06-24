#ifndef GENERATEROADWINDOW_H
#define GENERATEROADWINDOW_H

#include <QtGui>
#include <QDialog>

class GenerateRoadWindow : public QDialog {
    Q_OBJECT

public:
    GenerateRoadWindow(QWidget *parent = 0);

    int getWidth(void) const { return _widthSpinBox->text().toInt(); }
    int getLength(void) const { return _lengthSpinBox->text().toInt(); }
    
signals:
    
public slots:
    
private:
    QSpinBox* _widthSpinBox;
    QSpinBox* _lengthSpinBox;

    QPushButton* _okButton;
    QPushButton* _cancelButton;

};

#endif // GENERATEROADWINDOW_H
