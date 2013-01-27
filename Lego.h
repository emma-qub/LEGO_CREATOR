#ifndef LEGO_H
#define LEGO_H

#include <QColor>
#include <QObject>

class Lego : public QObject {

public:
    Lego(const QColor& color = QColor(Qt::red), QObject* parent = 0);
    Lego(const Lego& lego);

    virtual QColor getColor(void) const { return _color; }
    virtual void setColor(const QColor& color) { _color = color; }

    virtual Lego* cloning(void) const = 0;

    virtual QString whoiam(void) const;

protected:
    QColor _color;
};

#endif // LEGO_H
