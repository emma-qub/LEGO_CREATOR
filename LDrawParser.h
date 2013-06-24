#ifndef LDRAWPARSER_H
#define LDRAWPARSER_H

#include <QString>
#include <QColor>
#include <QMap>

#include <osg/Node>

class LDrawParser {

public:
    enum Winding { ccw, cw };
    enum Certified { yes, no, unknown };

public:
    static int tab;

public:
    class OpenFailed : std::exception {
    public:
        const virtual char *what() const throw () {
            return "Cannot open file";
        }
    };

public:
    struct ColorParams {
        QString colorName;
        QString surfValue;
        QString edgeValue;
        double alphaValue;
    };

public:
    LDrawParser(const QString& fileName);
    LDrawParser(const LDrawParser& lDrawParser);
    virtual ~LDrawParser(void);

    static osg::Vec3 calculateNormal(const osg::Vec3& a, const osg::Vec3& b, const osg::Vec3& c);
    static osg::Vec3 multMatVec(const osg::Vec3& vec, const osg::Matrix& mat);

    osg::Group* createNode(void);
    void fillArrays(QString fileName, bool accumCull, bool accumInvert, osg::Matrix accumTransformMatrix,
                    osg::Vec3Array* lineVerticesArray, osg::Vec3Array* triangleVerticesArray, osg::Vec3Array* quadVerticesArray,
                    osg::Vec4Array* lineColorsArray, osg::Vec4Array* triangleColorsArray, osg::Vec4Array* quadColorsArray, int currColor = 16);

private:
    void fillColorsArray(void);
    QString getSurfQString(int colorId);
    QString getEdgeQString(int colorId);
    osg::Vec4 getSurfOrEdgeColor(int colorId, bool isSurf = true);
    double getAlphaValue(int colorId);

private:
    QString _fileName;
    QMap<int, ColorParams> _colorsArray;
};

#endif // LDRAWPARSER_H
