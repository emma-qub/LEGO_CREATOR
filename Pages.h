#ifndef PAGES_H
#define PAGES_H

#include <QtGui>

class ViewerPage : public QWidget {
    Q_OBJECT

public:
    ViewerPage(QWidget* parent = NULL);

    void setWidth(int width) { _widthSpinBox->setValue(width); }
    void setLength(int length) { _lengthSpinBox->setValue(length); }
    void setColor(const QColor& color);

    int getWidth(void) const { return _widthSpinBox->value(); }
    int getPreviousWidth(void) const { return _previousWidth; }
    int getLength(void) const { return _lengthSpinBox->value(); }
    int getPreviousLength(void) const { return _previousLength; }
    QColor getColor(void) const { return _color; }
    QColor getPreviousColor(void) const { return _previousColor; }

public:
    QSpinBox* _widthSpinBox;
    int _previousWidth;
    QSpinBox* _lengthSpinBox;
    int _previousLength;
    QLabel* _colorLabel;
    QPushButton* _colorButton;
    QColor _color;
    QColor _previousColor;

public slots:
    void browsePalette(void);
};

#endif // PAGES_H
