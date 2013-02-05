#ifndef SLOP_H
#define SLOP_H

#include "Lego.h"

class Slop : public Lego {
    public:

        enum SlopType { simple, renforce };

        Slop(int length = 1, int width = 1, const QColor& color = QColor(Qt::red), SlopType slopType = simple, QObject* parent = 0);
        Slop(const Slop& Slop);

        SlopType getSlopType(void) const { return _slopType; }

        int getLength(void) const { return _length; }
        void setLength(int length) { _length = length; }
        int getWidth(void) const { return _width; }
        void setWidth(int width) { _width = width; }
        void setSlopType(SlopType slopType) { _slopType = slopType; }
        void setSlopType(int index) { index == 0 ? _slopType = simple : _slopType = renforce; }

        virtual void calculateBoundingBox(void);

        //int calculateHeight(void) const { return _slopType == brick ? 3 : 1; }

        virtual Slop* cloning(void) const;

        virtual QString whoiam(void) const;

    private:
        SlopType _slopType;
        int _length;
        int _width;
};

#endif // SLOP_H
