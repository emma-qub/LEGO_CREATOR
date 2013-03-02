#ifndef WORLD_H
#define WORLD_H

#include <QVector>

#include <osg/Node>
#include <osg/ref_ptr>
#include <osg/MatrixTransform>
#include <osg/LightSource>

#include <string>

#include "LegoGeode.h"

class World {

public:
    World();
    virtual ~World(void);

    osg::ref_ptr<osg::Group> getScene(void) const { return _scene.get(); }

    void initBrick(void);
    void fitBrick(void);
    void deleteLego(void);
    void deleteLego(const std::string& matrixName);
    std::string addBrick(LegoGeode* legoGeode, Lego* lego);
    bool canBeFit(void) const;
    void rotation(bool counterClockWise = false);
    void translation(double x, double y, double z);
    void translationXYZ(double x, double y, double z);

    static int minHeight;
    static int maxHeight;
    static int minWidth;
    static int maxWidth;
    static int minLength;
    static int maxLength;

    static int count;

private:
    osg::ref_ptr<osg::Group> _scene;
    osg::ref_ptr<osg::MatrixTransform> _currMatrixTransform;
    QVector<unsigned int> _matTransIndexes;
    double _x;
    double _y;
    double _z;
    bool _isTurned;
};

#endif // WORLD_H
