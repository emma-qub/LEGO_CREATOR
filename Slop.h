#ifndef SLOP_H
#define SLOP_H

#include "Lego.h"

class Slop : public Lego {
    public:

        enum SlopType { simple, renforce, roof };

        Slop(int width = 1, int length = 1, SlopType slopType = renforce, const QColor& color = QColor(Qt::red), QObject* parent = 0);
        Slop(const Slop& Slop);

        int getWidth(void) const { return _width; }
        int getLength(void) const { return _length; }
        SlopType getSlopType(void) const { return _slopType; }

        void setWidth(int width) { _width = width; }
        void setLength(int length) { _length = length; }
        void setSlopType(SlopType slopType) { _slopType = slopType; }
        void setSlopType(int index) {
                if(index == 0)
                    _slopType = simple;
                else if(index == 1)
                    _slopType = renforce;
                else
                    _slopType = roof;
            }

        virtual void calculateBoundingBox(void);

        //int calculateHeight(void) const { return _slopType == brick ? 3 : 1; }

        virtual Slop* cloning(void) const;

        virtual QString whoiam(void) const;

    private:
        int _width;
        int _length;
        SlopType _slopType;
};

#endif // SLOP_H
