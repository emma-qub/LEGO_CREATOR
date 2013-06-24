#ifndef PHOTOCALLBACK_H
#define PHOTOCALLBACK_H

#include <osg/ValueObject>
#include <osg/Camera>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgViewer/Viewer>
#include <osgText/Text>
#include <iostream>
#include <sstream>

class PhotoCallback : public osg::Camera::DrawCallback {

public:
    PhotoCallback(osg::Image* img, const std::string& fileName, osgText::Text* text = NULL) : _image(img), _text(text), _fileName(fileName) {}
    virtual void operator() (osg::RenderInfo& renderInfo) const;

    void setFileName(const std::string fileName) { _fileName = fileName; }

protected:
    osg::ref_ptr<osg::Image> _image;
    osg::observer_ptr<osgText::Text> _text;
    //mutable int _fileIndex;
    std::string _fileName;
};


#endif // PHOTOCALLBACK_H
