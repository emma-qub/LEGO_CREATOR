#include "SlopGeode.h"

#include <osg/Geometry>
#include <osg/Material>
#include <osg/BlendFunc>
#include <osg/MatrixTransform>

#include <QDebug>

SlopGeode::SlopGeode() :
    LegoGeode() {
}

SlopGeode::SlopGeode(Slop* slop) :
    LegoGeode(slop) {

    createGeode();
}

SlopGeode::SlopGeode(const SlopGeode& slopGeode) :
    LegoGeode(slopGeode) {
}

void SlopGeode::createGeode(void) {
    removeChildren(0, getNumChildren());

    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    addChild(geode);

    // Get the slop
    Slop* slop = static_cast<Slop*>(_lego);

    // Get slop color
    QColor color = slop->getColor();

    // Get slop type
    Slop::SlopType slopType = slop->getSlopType();

    // Get integer sizes
    int height = 3;
    if(slopType == Slop::plate)
        height = 1;
    int length = slop->getLength();
    int width = slop->getWidth();
    bool renf = false;
    bool rooff = false;
    if(slopType != Slop::classic) {
        rooff = true;
        if(width == 3)
            width = 2;
    }
    // Become type "renforce"
    if (width > 1 && !rooff) {
        renf = true;
        width = width - 1;
    }

    int width_2 = 0;
    if(renf)
        width_2 = 1;
    else if(rooff)
        width_2 = width;

    // Get real position, according to slop size
    // d : down, u : up, l : left, r : right, f : front, b : back
    double d = -height*Lego::height_unit/2;
    double u = height*Lego::height_unit/2;
    double l = -length*Lego::length_unit/2;
    double r = length*Lego::length_unit/2;
    double f = -width*Lego::length_unit/2;
    double b = width*Lego::length_unit/2;

    // Create 8 vertices
//    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    osg::Vec3 lfd(l, f, d);
    osg::Vec3 rfd(r, f, d);
//    osg::Vec3 rfu(r, f, u);
//    osg::Vec3 lfu(l, f, u);
    osg::Vec3 lbd(l, b, d);
    osg::Vec3 rbd(r, b, d);
    osg::Vec3 rbu(r, b, u);
    osg::Vec3 lbu(l, b, u);

    osg::Vec3 lfd1 = lbd;
    osg::Vec3 rfd1 = rbd;
    osg::Vec3 rfu1 = rbu;
    osg::Vec3 lfu1 = lbu;
    osg::Vec3 lbd1(l, b+width_2*Lego::length_unit, d);
    osg::Vec3 rbd1(r, b+width_2*Lego::length_unit, d);
    osg::Vec3 rbu1(r, b+width_2*Lego::length_unit, u);
    osg::Vec3 lbu1(l, b+width_2*Lego::length_unit, u);

    // Add the slop basis
    geode->addDrawable(createCarre(lbd, rbd, rbu, lbu, osg::Vec3(0, 1, 0), color));  // back
    geode->addDrawable(createTriangle(rfd, rbd, rbu, osg::Vec3(1, 0, 0), color));    // right
    geode->addDrawable(createTriangle(lfd, lbd, lbu, osg::Vec3(-1, 0, 0), color));    // left
    geode->addDrawable(createCarre(lfd, rfd, rbu, lbu, osg::Vec3(0, -1, 1), color));  // slop

    // Add brick renforce
    //QDebug("laaaa111");
    if (renf) {
        geode->addDrawable(createCarre(lbd1, rbd1, rbu1, lbu1, osg::Vec3(0, 1, 0), color));  // back
        geode->addDrawable(createCarre(rfd1, rbd1, rbu1, rfu1, osg::Vec3(1, 0, 0), color));  // right
        geode->addDrawable(createCarre(lfd1, lbd1, lbu1, lfu1, osg::Vec3(-1, 0, 0), color));  // left
        geode->addDrawable(createCarre(lfu1, rfu1, rbu1, lbu1, osg::Vec3(0, 0, 1), color));  // top
        geode->addDrawable(createCarre(lfd, rfd, rbd1, lbd1, osg::Vec3(0, -1, 0), color, true));  // down
    }
    if(rooff) {
        //QDebug(laaaa);
        //addDrawable(createCarre(lbd, rbd, rbu, lbu, osg::Vec3(0, 1, 0), color));  // back
        geode->addDrawable(createTriangle(rfu1, rfd1, rbd1, osg::Vec3(1, 0, 0), color));    // right
        geode->addDrawable(createTriangle(lfu1, lfd1, lbd1, osg::Vec3(-1, 0, 0), color));    // left
        geode->addDrawable(createCarre(lfu1, rfu1, rbd1, lbd1, osg::Vec3(0, 1, 1), color));  // slop
        geode->addDrawable(createCarre(lfd, rfd, rbd1, lbd1, osg::Vec3(0, -1, 0), color, true));  // down
    }
    else
        geode->addDrawable(createCarre(lfd, rfd, rbd, lbd, osg::Vec3(0, -1, 0), color, true));  // down


    // Distance between two plot center
    double distPlot = Lego::length_unit;

    // Calculate x max and y max for plots
    double xmin = -(length-1)*Lego::length_unit/2;

    // Add plots iteratively if the slop type is not flat
    if (renf && !rooff) {
        width ++;
        for (int i = 0; i < length; i++) {
            double radiusX = xmin + i*distPlot;
            geode->addDrawable(createPlot(radiusX, (width)*Lego::length_unit/2, height));
        }
        width --;
    }

     /*
    // Calculate x max and y max for bottom cylinder
    bool thinw = (width == 1);
    bool thinl = (slop->getLength() == 1);
    bool thin = (thinw || thinl);
    double xminb = xmin;
    if (!thinl)
        xminb = xmin+Lego::length_unit/2;
    double yminb = ymin;
    if (!thinw)
        yminb = ymin+Lego::length_unit/2;

    if (thinw && !thinl)
        width = 2;
    if (thinl && !thinw)
        length = 2;

    // Add bottom cylinder iteratively
    for (int i = 0; i < length-1; i++) {
        for (int j = 0; j < width-1; j++) {
            double radiusX = xminb + i*distPlot;
            double radiusY = yminb + j*distPlot;
            ref_ptr<Drawable> cylinder = createCylinder(radiusX, radiusY, 1, thin);
            ref_ptr<MatrixTransform> t = new MatrixTransform;
            Matrix m;
            m.makeTranslate(Vec3(0,0,-distPlot));
            t->setMatrix(m);
            t->addChild(cylinder);
            addDrawable(t);
        }
    }*/
}

void SlopGeode::setColornormals(osg::Vec3 normal, osg::ref_ptr<osg::Geometry> geoTriangle, QColor color)
{
    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
    normals->push_back(normal);
    geoTriangle->setNormalArray(normals);
    geoTriangle->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    osg::Vec4 colorVec(static_cast<float>(color.red())/255.0, static_cast<float>(color.green())/255.0, static_cast<float>(color.blue())/255.0, 1.0);
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    colors->push_back(colorVec);
    geoTriangle->setColorArray(colors);
    geoTriangle->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);

}

osg::ref_ptr<osg::Drawable> SlopGeode::createTriangle(osg::Vec3 rfd, osg::Vec3 rbd, osg::Vec3 rbu, osg::Vec3 normal, QColor color)
{
    osg::ref_ptr<osg::Geometry> geoTriangle = new osg::Geometry;
     osg::ref_ptr<osg::Vec3Array> tabSommet = new osg::Vec3Array;
     tabSommet->push_back(rfd);
     tabSommet->push_back(rbd);
     tabSommet->push_back(rbu);
     geoTriangle ->setVertexArray(tabSommet);

     // Nous créons une primitive Triangle et nous ajoutons les sommets selon leur index dans le tableau tabSommet
     osg::ref_ptr<osg::DrawElementsUInt> pPrimitiveSet = new osg::DrawElementsUInt( osg::PrimitiveSet::TRIANGLES, 0 );
     pPrimitiveSet->push_back(0);
     pPrimitiveSet->push_back(1);
     pPrimitiveSet->push_back(2);
     geoTriangle->addPrimitiveSet(pPrimitiveSet);

     setColornormals( normal, geoTriangle.get(), color);

     return geoTriangle.get();
}

void SlopGeode::addTransparency(osg::ref_ptr<osg::Geometry> slopGeometry)
{
    double alpha = 0.1;
    osg::StateSet* state = slopGeometry->getOrCreateStateSet();
    state->setMode(GL_BLEND,osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);
    osg::Material* mat = new osg::Material;
    mat->setAlpha(osg::Material::FRONT_AND_BACK, alpha);
    state->setAttributeAndModes(mat,osg::StateAttribute::ON |
    osg::StateAttribute::OVERRIDE);
    osg::BlendFunc* bf = new osg::BlendFunc(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA);
    state->setAttributeAndModes(bf);
    state->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    state->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    slopGeometry->setStateSet(state);

}

osg::ref_ptr<osg::Drawable> SlopGeode::createCarre(osg::Vec3 A, osg::Vec3 B, osg::Vec3 C, osg::Vec3 D, osg::Vec3 normal, QColor color, bool transparency)
{
     osg::ref_ptr<osg::Geometry> geoTriangle = new osg::Geometry;
     osg::ref_ptr<osg::Vec3Array> tabSommet = new osg::Vec3Array;
     tabSommet->push_back(A);
     tabSommet->push_back(B);
     tabSommet->push_back(C);
     tabSommet->push_back(D);
     geoTriangle ->setVertexArray(tabSommet);

     // Nous créons une primitive Triangle et nous ajoutons les sommets selon leur index dans le tableau tabSommet
     osg::ref_ptr<osg::DrawElementsUInt> pPrimitiveSet = new osg::DrawElementsUInt( osg::PrimitiveSet::QUADS, 0 );
     pPrimitiveSet->push_back(0);
     pPrimitiveSet->push_back(1);
     pPrimitiveSet->push_back(2);
     pPrimitiveSet->push_back(3);
     geoTriangle->addPrimitiveSet(pPrimitiveSet);

     if(transparency) {
         double alpha = 0.1;
         osg::StateSet* state = geoTriangle->getOrCreateStateSet();
         state->setMode(GL_BLEND,osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);
         osg::Material* mat = new osg::Material;
         mat->setAlpha(osg::Material::FRONT_AND_BACK, alpha);
         state->setAttributeAndModes(mat,osg::StateAttribute::ON |
         osg::StateAttribute::OVERRIDE);
         osg::BlendFunc* bf = new osg::BlendFunc(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA);
         state->setAttributeAndModes(bf);
         state->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
         state->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
         geoTriangle->setStateSet(state);

         // Match vertices
         //slopGeometry->setVertexArray(vertices);

         // Add color (each rectangle has the same color except for the down one which is transparent)
         osg::Vec4 colorVec(static_cast<float>(color.red())/255.0, static_cast<float>(color.green())/255.0, static_cast<float>(color.blue())/255.0, 1.0);
         osg::Vec4 transparent(.0f, .0f, .0f, .0f);
         osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
         // Add transparent color
         colors->push_back(transparent);
         // Add color to 5 other faces
         for (int k = 1; k < 6; k++)
             colors->push_back(colorVec);

         // Macth color
         geoTriangle->setColorArray(colors);
         geoTriangle->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);
//         addTransparency( geoTriangle.get());
//         set_Color_normals( normal, *geoTriangle.get(), QColor(0, 0, 0, 0));
     }
     else
         setColornormals( normal, geoTriangle.get(), color);

     return geoTriangle.get();
}

osg::ref_ptr<osg::Drawable> SlopGeode::createSlop(void) const {

    // Get the slop
    Slop* slop = static_cast<Slop*>(_lego);

    // Get slop color
    QColor color = slop->getColor();

    // Get slop type
    Slop::SlopType slopType = slop->getSlopType();

    // Get integer sizes
    int height = 3;
    int length = slop->getLength();
    int width = slop->getWidth();
    if (slopType == Slop::roof)
        width = 2;

    // Get real position, according to slop size
    // d : down, u : up, l : left, r : right, f : front, b : back
    double d = -height*Lego::height_unit/2;
    double u = height*Lego::height_unit/2;
    double l = -length*Lego::length_unit/2;
    double r = length*Lego::length_unit/2;
    double f = -width*Lego::length_unit/2;
    double b = width*Lego::length_unit/2;

    // Create 8 vertices
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    osg::Vec3 lfd(l, f, d);
    osg::Vec3 rfd(r, f, d);
    osg::Vec3 rfu(r, f, u);
    osg::Vec3 lfu(l, f, u);
    osg::Vec3 lbd(l, b, d);
    osg::Vec3 rbd(r, b, d);
    osg::Vec3 rbu(r, b, u);
    osg::Vec3 lbu(l, b, u);

    // Create faces
/*    // Down face
    vertices->push_back(rfd);
    vertices->push_back(rbd);
    vertices->push_back(lbd);
    vertices->push_back(lfd);

    // Up face
    vertices->push_back(rfu);
    vertices->push_back(rbu);
    vertices->push_back(lbu);
    vertices->push_back(lfu);

    // Front face
    vertices->push_back(rfd);
    vertices->push_back(rfu);
    vertices->push_back(lfu);
    vertices->push_back(lfd);

    // Back face
    vertices->push_back(rbd);
    vertices->push_back(rbu);
    vertices->push_back(lbu);
    vertices->push_back(lbd);

    // Left face
    vertices->push_back(lfd);
    vertices->push_back(lfu);
    vertices->push_back(lbu);
    vertices->push_back(lbd);

    // Right face
    vertices->push_back(rfd);
    vertices->push_back(rfu);
    vertices->push_back(rbu);
    vertices->push_back(rbd);
*/
    // Create slop geometry
    osg::ref_ptr<osg::Geometry> slopGeometry = new osg::Geometry;

    // Handle transparency
    double alpha = 0.1;
    osg::StateSet* state = slopGeometry->getOrCreateStateSet();
    state->setMode(GL_BLEND,osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);
    osg::Material* mat = new osg::Material;
    mat->setAlpha(osg::Material::FRONT_AND_BACK, alpha);
    state->setAttributeAndModes(mat,osg::StateAttribute::ON |
    osg::StateAttribute::OVERRIDE);
    osg::BlendFunc* bf = new osg::BlendFunc(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA);
    state->setAttributeAndModes(bf);
    state->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    state->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    slopGeometry->setStateSet(state);

    // Match vertices
    slopGeometry->setVertexArray(vertices);

    // Add color (each rectangle has the same color except for the down one which is transparent)
    osg::Vec4 colorVec(static_cast<float>(color.red())/255.0, static_cast<float>(color.green())/255.0, static_cast<float>(color.blue())/255.0, 1.0);
    osg::Vec4 transparent(.0f, .0f, .0f, .0f);
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    // Add transparent color
    colors->push_back(transparent);
    // Add color to 5 other faces
    for (int k = 1; k < 6; k++)
        colors->push_back(colorVec);

    // Macth color
    slopGeometry->setColorArray(colors);
    slopGeometry->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    // Create normals
/*    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;

    // Add normals (mind the insert order!)
    normals->push_back(osg::Vec3(0, 0, -1));
    normals->push_back(osg::Vec3(0, 0, 1));
    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(0, 1, 0));
    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(1, 0, 0));

    // Match normals
    slopGeometry->setNormalArray(normals);
    slopGeometry->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE);
*/
    // Define slop GL_QUADS with 24 vertices
  //  slopGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 24));

    // Return the slop whithout plot
    //addDrawable(createCarre(lbd, rbd, rbu, lbu));
   // addDrawable(createTriangle(rfd, rbd, rbu));
   // addDrawable(createTriangle(lfd, lbd, lbu));
   // addDrawable(createCarre(lfd, rfd, rbu, lbu));


    return slopGeometry.get();
}

SlopGeode* SlopGeode::cloning(void) const {
    return new SlopGeode(*this);
}
