#include "RoadGeode.h"

#include <osg/Geometry>
#include <osgDB/ReadFile>
#include <osg/TextureRectangle>
#include <osg/TexMat>

#include <cmath>


RoadGeode::RoadGeode() :
    LegoGeode() {
}

RoadGeode::RoadGeode(osg::ref_ptr<Road> road) :
    LegoGeode(road) {

    createGeode();
}

RoadGeode::RoadGeode(const RoadGeode& roadGeode) :
    LegoGeode(roadGeode) {
}

void RoadGeode::createGeode(void) {
    // Remove previous children
    removeChildren(0, getNumChildren());

    // Create geode
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    // add geode
    addChild(geode);

    // Get the brick
    Road* road = static_cast<Road*>(_lego);

    // Get brick type
    Road::RoadType roadType = road->getRoadType();

    // Get integer sizes
    int length = 32;
    int width = 32;

    // Get real position, according to brick size
    // l : left, r : right, f : front, b : back
    double l = -length*Lego::length_unit/2;
    double r = length*Lego::length_unit/2;
    double f = -width*Lego::length_unit/2;
    double b = width*Lego::length_unit/2;

    // Create square up face...
    osg::ref_ptr<osg::Geometry> roadGeometry = new osg::Geometry;
    // ... and square down face
    osg::ref_ptr<osg::Geometry> downGeometry = new osg::Geometry;

    // Create 4 vertices for up face...
    osg::ref_ptr<osg::Vec3Array> verticesUp = new osg::Vec3Array;
    verticesUp->push_back(osg::Vec3(l, f, 0));
    verticesUp->push_back(osg::Vec3(r, f, 0));
    verticesUp->push_back(osg::Vec3(r, b, 0));
    verticesUp->push_back(osg::Vec3(l, b, 0));
    roadGeometry->setVertexArray(verticesUp);
    // ... and 4 vertices for down face
    osg::ref_ptr<osg::Vec3Array> verticesDown = new osg::Vec3Array;
    verticesDown->push_back(osg::Vec3(l, f, -EPS));
    verticesDown->push_back(osg::Vec3(r, f, -EPS));
    verticesDown->push_back(osg::Vec3(r, b, -EPS));
    verticesDown->push_back(osg::Vec3(l, b, -EPS));
    downGeometry->setVertexArray(verticesDown);

    // Create texture coords for up face
    osg::ref_ptr<osg::Vec2Array> texCoords = new osg::Vec2Array;
    texCoords->push_back(osg::Vec2(0.0f, 0.0f));
    texCoords->push_back(osg::Vec2(1.0f, 0.0f));
    texCoords->push_back(osg::Vec2(1.0f, 1.0f));
    texCoords->push_back(osg::Vec2(0.0f, 1.0f));
    roadGeometry->setTexCoordArray(0, texCoords);

    // Create normal for up face...
    osg::ref_ptr<osg::Vec3Array> normalsUp = new osg::Vec3Array;
    normalsUp->push_back(osg::Vec3(0.0f, 0.0f, 1.0f));
    roadGeometry->setNormalArray(normalsUp);
    roadGeometry->setNormalBinding(osg::Geometry::BIND_OVERALL);
    // ... and for down face
    osg::ref_ptr<osg::Vec3Array> normalsDown = new osg::Vec3Array;
    normalsDown->push_back(osg::Vec3(0.0f, 0.0f, -1.0f));
    downGeometry->setNormalArray(normalsDown);
    downGeometry->setNormalBinding(osg::Geometry::BIND_OVERALL);

    // Create color for up face...
    osg::ref_ptr<osg::Vec4Array> colorsUp = new osg::Vec4Array;
    colorsUp->push_back(osg::Vec4(1.0, 1.0, 1.0, 1.0));
    osg::ref_ptr<osg::Vec4Array> colorsDown = new osg::Vec4Array;
    colorsDown->push_back(osg::Vec4(0.0, 112.0/255.0, 44.0/255.0, 1.0));
    if (roadType != Road::none)
        roadGeometry->setColorArray(colorsUp);
    else
        roadGeometry->setColorArray(colorsDown);
    roadGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);
    // ... and for down face
    downGeometry->setColorArray(colorsDown);
    downGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);

    //  Define road GL_QUADS with 4 vertices...
    roadGeometry->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, 4));
    // and down face
    downGeometry->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, 4));

    // Disable display list to show up the texture
    roadGeometry->setUseDisplayList(false);

    // Create image
    osg::ref_ptr<osg::Image> img = NULL;
    switch (roadType) {
    case Road::straight:
        img = osgDB::readImageFile("../LEGO_CREATOR/IMG/Straightb.png");
        break;
    case Road::curve:
        img = osgDB::readImageFile("../LEGO_CREATOR/IMG/Curveb.png");
        break;
    case Road::intersection:
        img = osgDB::readImageFile("../LEGO_CREATOR/IMG/Intersectionb.png");
        break;
    case Road::cross:
        img = osgDB::readImageFile("../LEGO_CREATOR/IMG/Crossb.png");
        break;
    case Road::none:
        img = NULL;
        break;
    }

    // Get state
    osg::ref_ptr<osg::StateSet> state = roadGeometry->getOrCreateStateSet();

    // If the road exists
    if (roadType != Road::none) {
        // Set texture
        osg::ref_ptr<osg::TextureRectangle> tex = new osg::TextureRectangle(img);

        osg::ref_ptr<osg::TexMat> texMat = new osg::TexMat;
        texMat->setScaleByTextureRectangleSize(true);

        // Set state
        state->setTextureAttributeAndModes(0, tex, osg::StateAttribute::ON);
        state->setTextureAttributeAndModes(0, texMat, osg::StateAttribute::ON);
    }

    // Turn off light for up face...
    state->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    // ... and for down face
    downGeometry->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

    // Set drawables to this (RoadGeode)
    geode->removeDrawables(0, geode->getDrawableList().size());
    geode->addDrawable(roadGeometry.get());
    geode->addDrawable(downGeometry.get());

    // Calculate plots according to road shape
    calculatePlots();
}

void RoadGeode::calculatePlots(void) {
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    addChild(geode);

    // Get the brick
    Road* road = static_cast<Road*>(_lego);

    // Get brick type
    Road::RoadType roadType = road->getRoadType();

    // Distance between two plots equal to Lego length unit
    double distPlot = Lego::length_unit;

    // The road has no height, but plots ave to be risen by an EPS to avoid graphical default
    switch (roadType) {
    case Road::straight:
        // Straight road
        for (int i = 0; i < 7; i++) {
            // There are two regions to add plots: the four corners
            for (int j = 0; j < 32; j++) {
                // left side
                geode->addDrawable(createPlot(-31*distPlot/2+i*distPlot, -31*distPlot/2+j*distPlot, EPS));
                // Right side
                geode->addDrawable(createPlot(19*distPlot/2+i*distPlot, -31*distPlot/2+j*distPlot, EPS));
            }
        }
        break;
    case Road::curve:
        for (int i = 0; i < 32; i++) {
            for (int j = 0; j < 32; j++) {
                // Above the curve OR Under the curve
                if (i*i+j*j >= 25*25 || i*i+j*j <= 6*6)
                    geode->addDrawable(createPlot(-31*distPlot/2+i*distPlot, -31*distPlot/2+j*distPlot, EPS));
            }
        }
        break;
    case Road::intersection:
        // Intersection road
        for (int i = 0; i < 7; i++) {
            // There are three regions to add plots: the four corners
            for (int j = 0; j < 32; j++) {
                // left side
                geode->addDrawable(createPlot(-31*distPlot/2+i*distPlot, -31*distPlot/2+j*distPlot, EPS));
            }
            for (int j = 0; j < 7; j++) {
                // Top right corner
                geode->addDrawable(createPlot(19*distPlot/2+i*distPlot, 19*distPlot/2+j*distPlot, EPS));
                // Bottom right corner
                geode->addDrawable(createPlot(19*distPlot/2+i*distPlot, -31*distPlot/2+j*distPlot, EPS));
            }
        }
        break;
    case Road::cross:
        // Cross road
        for (int i = 0; i < 7; i++) {
            // There are four regions to add plots: the four corners
            for (int j = 0; j < 7; j++) {
                // Top left corner
                geode->addDrawable(createPlot(-31*distPlot/2+i*distPlot, 19*distPlot/2+j*distPlot, EPS));
                // Top right corner
                geode->addDrawable(createPlot(19*distPlot/2+i*distPlot, 19*distPlot/2+j*distPlot, EPS));
                // Bottom left corner
                geode->addDrawable(createPlot(-31*distPlot/2+i*distPlot, -31*distPlot/2+j*distPlot, EPS));
                // Bottom right corner
                geode->addDrawable(createPlot(19*distPlot/2+i*distPlot, -31*distPlot/2+j*distPlot, EPS));
            }
        }
        break;
    case Road::none:
        // No road
        for (int i = 0; i < 32; i++) {
            for (int j = 0; j < 32; j++) {
                geode->addDrawable(createPlot(-31*distPlot/2+i*distPlot, -31*distPlot/2+j*distPlot, EPS));
            }
        }
    }
}

RoadGeode* RoadGeode::cloning(void) const {
    return new RoadGeode(*this);
}
