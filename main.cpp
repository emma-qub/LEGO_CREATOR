#define TEST 0

#if TEST

#include <osg/Group>
#include <osgViewer/Viewer>
#include <osg/MatrixTransform>
#include <TileGeode.h>
#include <CornerGeode.h>
#include <iostream>

#else

#include <QApplication>
#include <QtGui>
//#include <X11/Xlib.h>
#include "MainWindow.h"

#endif

int main(int argc, char** argv) {

#if TEST

    osg::ref_ptr<osg::Group> root = new osg::Group;

    {
        osg::ref_ptr<Tile> lego1 = new Tile;

        osg::ref_ptr<TileGeode> legoGeode1 = new TileGeode(lego1);
        legoGeode1->setName("tileGeode1");
        legoGeode1->addDescription("First geode");

        osg::ref_ptr<osg::MatrixTransform> mt1 = new osg::MatrixTransform;
        mt1->setName("MatrixTransform1");
        mt1->addChild(legoGeode1);


        osg::ref_ptr<Corner> lego2 = new Corner;

        osg::ref_ptr<CornerGeode> legoGeode2 = new CornerGeode(lego2);
        legoGeode2->setName("cornerGeode2");
        legoGeode2->addDescription("Second geode");

        osg::ref_ptr<osg::MatrixTransform> mt2 = new osg::MatrixTransform;
        mt2->setName("MatrixTransform2");
        mt2->addChild(legoGeode2);


        root->addChild(mt1.release());
        root->addChild(mt2.release());
    }

    {
        for (unsigned int k = 0; k < root->getNumChildren(); k++) {
            osg::MatrixTransform* mtCurr = static_cast<osg::MatrixTransform*>(root->getChild(k));
            std::cerr << mtCurr->getName() << std::endl;
            std::cerr << mtCurr->getChild(0)->getName() << std::endl;
            std::cerr << mtCurr->getChild(0)->getDescription(0) << std::endl;
//            LegoGeode* legoGeode = dynamic_cast<LegoGeode*>(mtCurr->getChild(0)->asGroup());
//            if (legoGeode) {
//                std::cerr << legoGeode->getLego()->whoiam().toStdString() << std::endl;
//            } else {
//                std::cerr << "Cannot cast..." << std::endl;
//            }

//            LegoGeode* legoGeode = static_cast<LegoGeode*>(mtCurr->getChild(0));
//            std::cerr << legoGeode->getLego()->whoiam().toStdString() << std::endl;
        }
    }

#else

    // Init srand to have pseudo-random numbers
    srand(time(NULL));

//    // Init X11 threads, because I really wanted to use another thread to render OSG components but it does not work either
//    QCoreApplication::setAttribute(Qt::AA_X11InitThreads);

    // Call QApplication
    QApplication app(argc, argv);

    // Create main window and display it
    MainWindow window;
    window.show();

    // Return application exec
    return app.exec();

#endif
}
