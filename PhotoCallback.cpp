#include "PhotoCallback.h"

void PhotoCallback::operator() (osg::RenderInfo& renderInfo) const {
    bool capturing = false;

    if (_image.valid() && _image->getUserValue("Capture", capturing)) {
        osg::GraphicsContext* gc = renderInfo.getState()->getGraphicsContext();
        if (capturing && gc->getTraits()) {
            int width = gc->getTraits()->width;
            int height = gc->getTraits()->height;

            GLenum pixelFormat = (gc->getTraits()->alpha ? GL_RGBA : GL_RGB);
            _image->readPixels( 0, 0, width, height, pixelFormat, GL_UNSIGNED_BYTE );
            std::stringstream ss;
            ss << "/home/shaolan/Images/LEGO/Parts/" << _fileName << ".bmp";
            if (osgDB::writeImageFile(*_image, ss.str()) && _text.valid())
                _text->setText(std::string("Saved to ") + ss.str());
        }
        _image->setUserValue("Capture", false);
    }
}
