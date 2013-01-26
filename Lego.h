#ifndef LEGO_H
#define LEGO_H

#include <QColor>
#include <QObject>

class Lego : public QObject {
public:
    Lego(const QColor &color = QColor(Qt::red), QObject* parent = 0);
    virtual ~Lego();

    virtual QString description(void) = 0;

private:
    QColor _color;
};

#endif // LEGO_H
