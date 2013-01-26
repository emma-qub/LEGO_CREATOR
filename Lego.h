#ifndef LEGO_H
#define LEGO_H

#include <QColor>
#include <QObject>

class Lego : public QObject {
public:
    Lego(const QColor &color = QColor(Qt::red), QObject* parent = 0);
    virtual ~Lego();

    QColor getColor(void) const { return _color; }
    void setColor(const QColor& color) { _color = color; }

    virtual QString description(void) const = 0;

protected:
    QColor _color;
};

#endif // LEGO_H
