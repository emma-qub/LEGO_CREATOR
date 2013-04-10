#include "PickHandler.h"

#include "LegoNode.h"
#include "BrickNode.h"

#include <QDebug>

// Enable to pseudo hide the selection box when nothing is selected
#define RATIO 10.0f

osg::Node* PickHandler::getOrCreateSelectionBox(void) {
    // If selection box doesn't exist, we create it
    if (!_selectionBox) {
        // Instantiate _selectionBox class variable
        _selectionBox = new osg::MatrixTransform;
        _selectionBox->setNodeMask(0x1);

        // Create box geode and asign it to _selectionBox
        initSelectionBox();

        // Give a nice rendering
        osg::StateSet* ss = _selectionBox->getOrCreateStateSet();
        ss->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
        ss->setAttributeAndModes(new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE));
    }

    // Return selection box
    return _selectionBox.get();
}

bool PickHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) {
    // To select something, users have to press CTRL and release left mouse button
    if (ea.getEventType() != osgGA::GUIEventAdapter::RELEASE
    ||  ea.getButton()    != osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON
    || !(ea.getModKeyMask()& osgGA::GUIEventAdapter::MODKEY_CTRL))
        return false;

    // Dynamic cast aa into viewer*
    osgViewer::View* viewer = dynamic_cast<osgViewer::View*>(&aa);

    qDebug() << "Clicking in viewer:" << viewer << "and using scene:" << viewer->getSceneData();

    // If dynamic cast worked
    if (viewer) {
        // Create intersector line according to mouse coordinates and window matrix
        osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector = new osgUtil::LineSegmentIntersector(osgUtil::Intersector::WINDOW, ea.getX(), ea.getY());

        // Create intersection visitor according to previous intersector line
        osgUtil::IntersectionVisitor iv(intersector.get());

        // Set traversal mask
        iv.setTraversalMask(~0x1);

        // Assign intersector visitor to camera
        viewer->getCamera()->accept(iv);

        // If there is at least one inttersection
        if (intersector->containsIntersections()) {
            // Get nearest result
            osgUtil::LineSegmentIntersector::Intersection result = *(intersector->getIntersections().begin());

            // Get result bounding box
            osg::BoundingBox bb = result.drawable->getParent(0)->asGeode()->getBoundingBox();

            // Get bounding box coordinates in world
            osg::Vec3 worldCenter = bb.center() * osg::computeLocalToWorld(result.nodePath);

            // Set _selectionBox according to previous bounding box
            _selectionBox->setMatrix(osg::Matrix::scale(RATIO*(bb.xMax() - bb.xMin()), RATIO*(bb.yMax() - bb.yMin()), RATIO*(bb.zMax() - bb.zMin())) * osg::Matrix::translate(worldCenter));

            LegoNode* legoNode = dynamic_cast<LegoNode*>(result.drawable->getParent(0)->getParent(0));
            if (legoNode) {
//                if (dynamic_cast<BrickNode*>(legoNode))
//                    legoNode->getLego()->setColor(QColor(Qt::white));
//                else
//                    legoNode->getLego()->setColor(QColor(Qt::blue));

                qDebug() << "Selected piece adress:" << legoNode;
                legoNode->createGeode();

//                osg::MatrixTransform* mt = dynamic_cast<osg::MatrixTransform*>(legoNode->getParent(0));
//                if (mt) {
//                    for (int i = 0; i < 4; i++) {
//                        for (int j = 0; j < 3; j++) {
//                            osg::Matrix mat = mt->getMatrix();
//                            qDebug() << mat(i, j);
//                        }
//                    }
//                    _selectionBox->postMult(mt->getMatrix());
//                }

//                qDebug() << QString::fromStdString(mt->getParent(0)->getName());

            } else {
                qDebug() << "Not a lego node";
                initSelectionBox();
            }
        }
    } else {
        qDebug() << "Cannot cast to osgViewer::View* within PickHandler::handle.";
    }

    // Return false anyway, to avoid callback issues
    return false;
}
 void PickHandler::initSelectionBox(void) {
     // Remove box geode child
     _selectionBox->removeChildren(0, _selectionBox->getNumChildren());

     // Create init box geode
     osg::ref_ptr<osg::Geode> geode = new osg::Geode;
     geode->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(), 1.0f/RATIO)));

     // Add box geode
     _selectionBox->addChild(geode);
 }
