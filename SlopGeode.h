#ifndef SLOPGEODE_H
#define SLOPGEODE_H

#include "LegoGeode.h"
#include "Slop.h"

class SlopGeode : public LegoGeode {

public:
    SlopGeode();
    SlopGeode(Slop* slop);
    SlopGeode(const SlopGeode& slopGeode);

    virtual void createGeode(void);

    osg::ref_ptr<osg::Drawable> createSlop(void) const;

    osg::ref_ptr<osg::Drawable> createTriangle(osg::Vec3 rfd, osg::Vec3 rbd, osg::Vec3 rbu, osg::Vec3 r, QColor color) ;
    osg::ref_ptr<osg::Drawable> createCarre(osg::Vec3 A, osg::Vec3 B, osg::Vec3 C, osg::Vec3 D, osg::Vec3 r, QColor color, bool trans = NULL) ;
    void set_Color_normals(osg::Vec3 normal, osg::Geometry& geoTriangle, QColor color);
    void add_transparency(osg::Geometry& slopGeometry);

    virtual SlopGeode* cloning(void) const;

};

#endif // SLOPGEODE_H