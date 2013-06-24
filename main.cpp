// Enable to switch between real application, and test code
#define DEBUG 0
#define CAST 0
#define RANDOM 0
#define TEST 0
#define THUMBS 0
#define DERECURSIVE 0
#define COLOR 0
#define LISTPARTS 0

#if DEBUG

#include <osg/Camera>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgViewer/CompositeViewer>
#include <osgViewer/Viewer>

#include <QDebug>
#include <QDir>
#include <QtAlgorithms>

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "TileNode.h"
#include "CornerNode.h"
#include "LDrawParser.h"
#include "PhotoCallback.h"

#if RANDOM
int rand_a_b(int a, int b){
    return rand()%(b-a) +a;
}
#endif

#else

#include <QApplication>
#include <QtGui>
//#include <X11/Xlib.h>
#include "MainWindow.h"

#endif

#if THUMBS
osgViewer::View* create2DView(int x, int y, int width, int height, const osg::Vec3& lookDir, const osg::Vec3& up, osg::GraphicsContext* gc, osg::Node* scene) {
    // Set view components
    osg::ref_ptr<osgViewer::View> view = new osgViewer::View;
    view->getCamera()->setGraphicsContext(gc);
    view->getCamera()->setViewport(x, y, width, height);
    view->setSceneData(scene);

    // Set camera values
    osg::Vec3 center = scene->getBound().center();
    double radius = scene->getBound().radius();
    view->getCamera()->setViewMatrixAsLookAt(center - lookDir*(radius*2.0), center, up);
    view->getCamera()->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(width)/static_cast<double>(height), 1.0f, 10000.0f);


    // Add lights
    osg::ref_ptr<osg::Light> sun = new osg::Light;
    sun->setLightNum(0);
    sun->setAmbient(osg::Vec4(.1f, .1f, .1f, 1.f));
    sun->setDiffuse(osg::Vec4(.8f, .8f, .1f, 1.f));
    sun->setSpecular(osg::Vec4(.8f, .8f, .8f, 1.f));
    sun->setPosition(osg::Vec4(1.f, 1.f, 1.f, 1.f));
    sun->setDirection(osg::Vec3(1.f, 0.f, 0.f));

    osg::ref_ptr<osg::Light> sun2 = new osg::Light;
    sun2->setLightNum(1);
    sun2->setAmbient(osg::Vec4(.1f, .1f, .1f, 1.f));
    sun2->setDiffuse(osg::Vec4(.8f, .8f, .1f, 1.f));
    sun2->setSpecular(osg::Vec4(.8f, .8f, .8f, 1.f));
    sun2->setPosition(osg::Vec4(-1.f, -1.f, -1.f, 1.f));
    sun2->setDirection(osg::Vec3(1.f, 0.f, 0.f));

    osg::ref_ptr<osg::LightSource> ls = new osg::LightSource;
    ls->setLight(sun);
    ls->setReferenceFrame(osg::LightSource::ABSOLUTE_RF);

    osg::ref_ptr<osg::LightSource> ls2 = new osg::LightSource;
    ls2->setLight(sun2);
    ls2->setReferenceFrame(osg::LightSource::ABSOLUTE_RF);

    scene->setDataVariance(osg::Object::STATIC);
    osg::StateSet* state = scene->getOrCreateStateSet();
    state->setMode(GL_LIGHTING, osg::StateAttribute::ON);
    state->setMode(GL_LIGHT0, osg::StateAttribute::ON);
    state->setMode(GL_LIGHT1, osg::StateAttribute::ON);

    scene->asGroup()->addChild(ls.get());
    scene->asGroup()->addChild(ls2.get());


    // Return view
    return view.release();
}

#endif

int main(int argc, char** argv) {

#if DEBUG
    srand(time(NULL)); // initialisation de rand

    QStringList allParts;

    QString path1 = "/home/shaolan/Documents/ldraw/parts/";
    QDir dir = QDir(path1);

    allParts = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);

    // Create the list that records all categories
    QStringList allCategories;

    #if LISTPARTS

    // Create part names list
    for (int k = 0; k < allParts.size(); k++) {
        QString fileName = "/home/shaolan/Documents/ldraw/parts/" + allParts.at(k);

        // Try to open text file in read only mode
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Error while opening in read only " + fileName + " file within main";
            continue;
        }

        // Stream text and encode in UTF-8
        QTextStream inFile(&file);
        inFile.setCodec("UTF-8");

        // Get whole text
        QStringList allFile = inFile.readAll().split("\n");

        // Remove blank lines
        allFile.removeAll("");

        // Create part type
        QString partType;

        // Create category name
        QString category;

        // Get line that indicates part type, i.e Part, Part Alias, Shortcut...
        QStringList partTypeLine = allFile.filter("!LDRAW_ORG");
        // Every file may contains LDRAW_ORG meta command once to
        if (partTypeLine.size() > 0) {
            partType = partTypeLine.at(0).split("!LDRAW_ORG").at(1);
            // Remove words after category name
            if (partType.contains("UPDATE"))
                partType = partType.split("UPDATE").at(0).simplified();
            if (partType.contains("ORIGINAL"))
                partType = partType.split("ORIGINAL").at(0).simplified();
        }

        // If part type is different from Part, then we are facing a part alias, or a shortcut
        // there is no need to get category, it is a special part. Otherwise, we get its category.
        if (partType == "Part") {
            // Get line, if any, that contains !CATEGORY
            QStringList categoryLine = allFile.filter("!CATEGORY");
            if (categoryLine.size() > 0) {
                category = categoryLine.at(0).split("!CATEGORY").at(1).simplified();
            // If there is no line containing !CATEGORY,
            // then the category is the first word of the first line.
            } else {
                category = allFile.at(0).split(QRegExp("\\s+")).at(1);
            }
        } else {
            category = partType;
        }

        // Read line per line
        if (category.startsWith('~') || category.startsWith('_'))
            category = category.mid(1);

        // Add category to list, only if it doesn't already exist in
        if (!allCategories.contains(category))
            allCategories << category;

        if (category.contains("Physical_Colour") || category.contains("Shortcut") || category.contains("Alias"))
            category = "Alias";

        QString name = allFile.filter("0 Name: ").at(0).split(QRegExp("\\s+")).at(2);
        if (!name.endsWith(".dat")) {
            qDebug() << "Problem with file" << fileName << "within main";
            continue;
        }
        QString description = allFile.at(0).split(QRegExp("0")).at(1).simplified();

        // Append current file information to right ldr file
        QFile categoryFile("/home/shaolan/Documents/Qt/LViewer/config/"+category+".ldr");
        if (!categoryFile.open(QIODevice::Append | QIODevice::Text)) {
            qDebug() << "Error while opening in write only "+category+".ldr file within main";
            continue;
        }

        QTextStream inCategoryFile(&categoryFile);
        inCategoryFile.setCodec("UTF-8");

        inCategoryFile << "0 !FILENAME " << name << " !DESCRIPTION " << description << "\n";

        categoryFile.close();
        file.close();
    }

    // Sort part names
    qSort(allCategories);

//    // Add special sections
//    allCategories << "Primitives" << "Subparts";

    // Create file to store part names
    QFile listPart("/home/shaolan/Documents/Qt/LViewer/config/Categories.ldr");
    if (!listPart.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Error while opening in write only Categories.ldr file within main";
    }

    // Get text stream
    QTextStream inFile(&listPart);
    inFile.setCodec("UTF-8");

    // Add section name to file
    for (int k = 0; k < allCategories.size(); k++) {
        inFile << "0 !CATEGORY " << allCategories.at(k) << "\n";
    }

//    // Dispatch pieces according to their category
//    for (int k = 0; k < allParts.size(); k++) {
//        QString fileName = "/home/shaolan/Documents/ldraw/parts/" + allParts.at(k);

//        // Try to open text file in read only mode
//        QFile file(fileName);
//        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//            qDebug() << "Error while opening in read only " + fileName + " file within main";
//            continue;
//        }

//        // Stream text and encode in UTF-8
//        QTextStream inFile(&file);
//        inFile.setCodec("UTF-8");

//        // Read line per line
//        QString currLine = inFile.readLine().split(QRegExp("\\s+")).at(1);
//        if (currLine.startsWith('~') || currLine.startsWith('_'))
//            currLine = currLine.mid(1);

//        if (currLine != "Moved" && !allCategories.contains(currLine))
//            allCategories << currLine;
//    }


    #endif


    #if CAST
    osg::ref_ptr<osg::Group> root = new osg::Group;

    {
        osg::ref_ptr<Tile> lego1 = new Tile;

        osg::ref_ptr<TileNode> legoNode1 = new TileNode(lego1);
        legoNode1->setName("tileNode1");
        legoNode1->addDescription("First geode");

        osg::ref_ptr<osg::MatrixTransform> mt1 = new osg::MatrixTransform;
        mt1->setName("MatrixTransform1");
        mt1->addChild(legoNode1);


        osg::ref_ptr<Corner> lego2 = new Corner;

        osg::ref_ptr<CornerNode> legoNode2 = new CornerNode(lego2);
        legoNode2->setName("cornerNode2");
        legoNode2->addDescription("Second geode");

        osg::ref_ptr<osg::MatrixTransform> mt2 = new osg::MatrixTransform;
        mt2->setName("MatrixTransform2");
        mt2->addChild(legoNode2);


        root->addChild(mt1.release());
        root->addChild(mt2.release());
    }

    {
        for (unsigned int k = 0; k < root->getNumChildren(); k++) {
            osg::MatrixTransform* mtCurr = static_cast<osg::MatrixTransform*>(root->getChild(k));
            std::cerr << mtCurr->getName() << std::endl;
            std::cerr << mtCurr->getChild(0)->getName() << std::endl;
            std::cerr << mtCurr->getChild(0)->getDescription(0) << std::endl;
            LegoNode* legoNode = dynamic_cast<LegoNode*>(mtCurr->getChild(0)->asGroup());
            if (legoNode) {
                std::cerr << legoNode->getLego()->whoiam().toStdString() << std::endl;
            } else {
                std::cerr << "Cannot cast..." << std::endl;
            }

            LegoNode* legoNode = static_cast<LegoNode*>(mtCurr->getChild(0));
            std::cerr << legoNode->getLego()->whoiam().toStdString() << std::endl;
        }
    }
    #endif

    #if TEST
    for (int k = 4905; k < 4931; k++) {
        QString blancString = "";
        for (int i = 0; i < 30 - allParts.at(k).size(); i++)
            blancString += " ";

        try {
            LDrawParser parser("/home/shaolan/Documents/ldraw/parts/" + allParts.at(k), "#257A3E", "#333333");
            osg::Group* lego = parser.createNode();

            osg::ref_ptr<osg::Group> scene = new osg::Group;
            scene->addChild(lego);



            osg::ref_ptr<osg::Light> sun = new osg::Light;
            sun->setLightNum(0);
            sun->setAmbient(osg::Vec4(.1f, .1f, .1f, 1.f));
            sun->setDiffuse(osg::Vec4(.8f, .8f, .8f, 1.f));
            sun->setSpecular(osg::Vec4(.8f, .8f, .8f, 1.f));
            sun->setPosition(osg::Vec4(1000.f, 1000.f, 1000.f, 1000.f));
            sun->setDirection(osg::Vec3(1.f, 0.f, 0.f));

            osg::ref_ptr<osg::LightSource> ls = new osg::LightSource;
            ls->setLight(sun);
            ls->setReferenceFrame(osg::LightSource::ABSOLUTE_RF);

            scene->setDataVariance(osg::Object::STATIC);
            osg::StateSet* state = scene->getOrCreateStateSet();
            state->setMode(GL_LIGHTING, osg::StateAttribute::ON);
            state->setMode(GL_LIGHT0, osg::StateAttribute::ON);

            scene->addChild(ls.get());




            osgViewer::Viewer viewer;
            viewer.setSceneData(scene);
            viewer.getCamera()->setClearColor(osg::Vec4(252.0/255.0, 254.0/255.0 ,234.0/255.0, 1.0));
            viewer.run();
            qDebug() << QString::number(k) + " " + allParts.at(k) << blancString + "\t[OK]";
        } catch (LDrawParser::OpenFailed&) {
            qDebug() << QString::number(k) + " " + allParts.at(k) << blancString + "\t[FAIL]";
        }
    }
    #endif

    #if RANDOM
    int getFileNb = rand_a_b(0, allParts.size());

    //LDrawParser parser("/home/shaolan/Documents/ldraw/parts/1.dat");
    //LDrawParser parser("/home/shaolan/Documents/ldraw/parts/370.dat");
    //LDrawParser parser("/home/shaolan/Documents/ldraw/parts/501b.dat");
    //LDrawParser parser("/home/shaolan/Documents/ldraw/parts/170.dat");
    //LDrawParser parser("/home/shaolan/Documents/ldraw/parts/294.dat");
    //LDrawParser parser("/home/shaolan/Documents/ldraw/parts/291.dat");
    //LDrawParser parser("/home/shaolan/Documents/ldraw/parts/3068p51.dat");
    //LDrawParser parser("/home/shaolan/Documents/ldraw/parts/3004p05.dat");
    //LDrawParser parser("/home/shaolan/Documents/ldraw/parts/425.dat");
    //LDrawParser parser("/home/shaolan/Documents/ldraw/parts/003238b.dat");
    //LDrawParser parser("/home/shaolan/Documents/ldraw/parts/101.dat");

    ////LDrawParser parser("/home/shaolan/Documents/ldraw/parts/2408.dat");
    ////LDrawParser parser("/home/shaolan/Documents/ldraw/parts/2466.dat");
    ////LDrawParser parser("/home/shaolan/Documents/ldraw/parts/2494.dat");
    ////LDrawParser parser("/home/shaolan/Documents/ldraw/parts/2571.dat");
    ////LDrawParser parser("/home/shaolan/Documents/ldraw/parts/2572.dat");
    ////LDrawParser parser("/home/shaolan/Documents/ldraw/parts/295.dat");
    ////LDrawParser parser("/home/shaolan/Documents/ldraw/parts/2826.dat");
    ////LDrawParser parser("/home/shaolan/Documents/ldraw/parts/32ac01.dat");
    ////LDrawParser parser("/home/shaolan/Documents/ldraw/parts/32bc01.dat");

    ////LDrawParser parser("/home/shaolan/Documents/ldraw/parts/32cc01.dat");
    ////LDrawParser parser("/home/shaolan/Documents/ldraw/parts/33ac01.dat");
    ////LDrawParser parser("/home/shaolan/Documents/ldraw/parts/33bc01.dat");
    ////LDrawParser parser("/home/shaolan/Documents/ldraw/parts/33cc01.dat");
    ////LDrawParser parser("/home/shaolan/Documents/ldraw/parts/3762.dat");
    ////LDrawParser parser("/home/shaolan/Documents/ldraw/parts/3855.dat");
    ////LDrawParser parser("/home/shaolan/Documents/ldraw/parts/4033c01.dat");

    ////LDrawParser parser("/home/shaolan/Documents/ldraw/parts/4035c01.dat");
    ///LDrawParser parser("/home/shaolan/Documents/ldraw/parts/4448.dat");
    ///LDrawParser parser("/home/shaolan/Documents/ldraw/parts/4448d01.dat");
    ///LDrawParser parser("/home/shaolan/Documents/ldraw/parts/4448p01.dat");
    ////LDrawParser parser("/home/shaolan/Documents/ldraw/parts/453ac01.dat");
    ////LDrawParser parser("/home/shaolan/Documents/ldraw/parts/453bc01.dat");
    ////LDrawParser parser("/home/shaolan/Documents/ldraw/parts/453cc01.dat");

    ////LDrawParser parser("/home/shaolan/Documents/ldraw/parts/4594.dat");
    ////LDrawParser parser("/home/shaolan/Documents/ldraw/parts/46103.dat");
    ///LDrawParser parser("/home/shaolan/Documents/ldraw/parts/4688.dat");
    ///LDrawParser parser("/home/shaolan/Documents/ldraw/parts/4866.dat");
    ////LDrawParser parser("/home/shaolan/Documents/ldraw/parts/60592c01.dat");
    ///LDrawParser parser("/home/shaolan/Documents/ldraw/parts/60593c01.dat");
    ////LDrawParser parser("/home/shaolan/Documents/ldraw/parts/60601.dat");
    ////LDrawParser parser("/home/shaolan/Documents/ldraw/parts/60602.dat");

    ////LDrawParser parser("/home/shaolan/Documents/ldraw/parts/60603.dat");
    /////////////////////LDrawParser parser("/home/shaolan/Documents/ldraw/parts/6159.dat");
    //LDrawParser parser("/home/shaolan/Documents/ldraw/parts/4219a.dat");
    //LDrawParser parser("/home/shaolan/Documents/ldraw/parts/3068bp05.dat");
    //LDrawParser parser("/home/shaolan/Documents/ldraw/parts/80394.dat");
    //LDrawParser parser("/home/shaolan/Documents/ldraw/parts/407924.dat");
    //LDrawParser parser("/home/shaolan/Documents/ldraw/parts/4100545.dat");
    //LDrawParser parser("/home/shaolan/Documents/ldraw/parts/4109600.dat");
    //LDrawParser parser("/home/shaolan/Documents/ldraw/parts/4109810.dat");
    LDrawParser parser("/home/shaolan/Documents/ldraw/parts/4107488.dat");
    //LDrawParser parser("/home/shaolan/Documents/ldraw/parts/4116688.dat");
    //LDrawParser parser("/home/shaolan/Documents/ldraw/parts/4141498.dat");
    ///LDrawParser parser("/home/shaolan/Documents/ldraw/parts/6160.dat");
    ////LDrawParser parser("/home/shaolan/Documents/ldraw/parts/62360.dat");
    ///LDrawParser parser("/home/shaolan/Documents/ldraw/parts/6238.dat");
    ///LDrawParser parser("/home/shaolan/Documents/ldraw/parts/6238a.dat");
    ///LDrawParser parser("/home/shaolan/Documents/ldraw/parts/6238b.dat");
    ////LDrawParser parser("/home/shaolan/Documents/ldraw/parts/62576.dat");

    ////LDrawParser parser("/home/shaolan/Documents/ldraw/parts/645ac01.dat");
    ////LDrawParser parser("/home/shaolan/Documents/ldraw/parts/645bc01.dat");
    ////LDrawParser parser("/home/shaolan/Documents/ldraw/parts/645cc01.dat");
    ///LDrawParser parser("/home/shaolan/Documents/ldraw/parts/810.dat");

    //LDrawParser parser("/home/shaolan/Documents/ldraw/parts/240.dat");
    //LDrawParser parser("/home/shaolan/Documents/ldraw/parts/180.dat");
    //LDrawParser parser("/home/shaolan/Documents/ldraw/parts/3001.dat");
    //LDrawParser parser("/home/shaolan/Documents/ldraw/parts/s/3001s01.dat");
    //LDrawParser parser("/home/shaolan/Documents/ldraw/p/box5.dat");
    //LDrawParser parser("/home/shaolan/Documents/ldraw/p/box3u2p.dat");
    //LDrawParser parser("/home/shaolan/Documents/testCCW.dat");
    //LDrawParser parser("/home/shaolan/Documents/testCCWs01.dat");
    //LDrawParser parser("/home/shaolan/Documents/test.dat");
    //LDrawParser parser("/home/shaolan/Documents/3001Test.dat");
    //LDrawParser parser("/home/shaolan/Documents/3001s01Test.dat");
    //LDrawParser parser("/home/shaolan/Documents/box5Test.dat");
    //LDrawParser parser("/home/shaolan/Documents/box3u2pTest.dat");
    qDebug() << allParts.at(getFileNb);
    //LDrawParser parser("/home/shaolan/Documents/ldraw/parts/" + allParts.at(getFileNb));
    osg::Group* lego = parser.createNode();

    osg::ref_ptr<osg::Group> scene = new osg::Group;
    scene->addChild(lego);



    osg::ref_ptr<osg::Light> sun = new osg::Light;
    sun->setLightNum(0);
    sun->setAmbient(osg::Vec4(.1f, .1f, .1f, 1.f));
    sun->setDiffuse(osg::Vec4(.8f, .8f, .8f, 1.f));
    sun->setSpecular(osg::Vec4(.8f, .8f, .8f, 1.f));
    sun->setPosition(osg::Vec4(100.f, 100.f, 100.f, 1.f));
    sun->setDirection(osg::Vec3(1.f, 0.f, 0.f));

//    osg::ref_ptr<osg::Light> sun2 = new osg::Light;
//    sun2->setLightNum(1);
//    sun2->setAmbient(osg::Vec4(.1f, .1f, .1f, 1.f));
//    sun2->setDiffuse(osg::Vec4(.8f, .8f, .1f, 1.f));
//    sun2->setSpecular(osg::Vec4(.8f, .8f, .8f, 1.f));
//    sun2->setPosition(osg::Vec4(-1.f, -1.f, -1.f, 1.f));
//    sun2->setDirection(osg::Vec3(1.f, 0.f, 0.f));

    osg::ref_ptr<osg::LightSource> ls = new osg::LightSource;
    ls->setLight(sun);
    ls->setReferenceFrame(osg::LightSource::ABSOLUTE_RF);

//    osg::ref_ptr<osg::LightSource> ls2 = new osg::LightSource;
//    ls2->setLight(sun2);
//    ls2->setReferenceFrame(osg::LightSource::ABSOLUTE_RF);

    scene->setDataVariance(osg::Object::STATIC);
    osg::StateSet* state = scene->getOrCreateStateSet();
    state->setMode(GL_LIGHTING, osg::StateAttribute::ON);
    state->setMode(GL_LIGHT0, osg::StateAttribute::ON);
//    state->setMode(GL_LIGHT1, osg::StateAttribute::ON);

    scene->addChild(ls.get());
//    scene->addChild(ls2.get());




    osgViewer::Viewer viewer;
    viewer.setSceneData(scene);
    viewer.getCamera()->setClearColor(osg::Vec4(252.0/255.0, 254.0/255.0 ,234.0/255.0, 1.0));
    viewer.run();
    #endif

    #if THUMBS

    for (int k = 0; k < 7114; k++) {
        LDrawParser parser("/home/shaolan/Documents/ldraw/parts/" + allParts.at(k));
        qDebug() << k << "\tOpening" << allParts.at(k);
        osg::ref_ptr<osg::Group> scene = parser.createNode();

        unsigned int width = 1440, height = 900;
        osg::GraphicsContext::WindowingSystemInterface* wsi = osg::GraphicsContext::getWindowingSystemInterface();
        if (wsi)
            wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), width, height);

        osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
        traits->x = 0;
        traits->y = 0;
        traits->width = width;
        traits->height = height;
        traits->windowDecoration = false;
        traits->doubleBuffer = true;
        traits->sharedContext = 0;

        osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
        if (!gc || !scene)
            return 1;

        osg::ref_ptr<osgViewer::View> mainView = create2DView(0, 0, width, height, osg::Z_AXIS+osg::Y_AXIS-osg::X_AXIS, -osg::Y_AXIS-osg::X_AXIS+osg::Z_AXIS, gc.get(), scene.get());
        osgViewer::CompositeViewer viewer;
        viewer.addView(mainView.get());

        osg::ref_ptr<osg::Image> thumbPic = new osg::Image;
        osg::ref_ptr<PhotoCallback> pcb = new PhotoCallback(thumbPic.get(), allParts.at(k).toStdString());

        mainView->getCamera()->setPostDrawCallback(pcb.get());
        mainView->getCamera()->setClearColor(osg::Vec4(252.0/255.0, 254.0/255.0 ,234.0/255.0, 1.0));

        thumbPic->setUserValue("Capture", true);
        viewer.frame();
    }

    return 0;

    #endif

    #if DERECURSIVE

    LDrawParser parser("/home/shaolan/Documents/3001Test.dat");
    parser.createNode();

    #endif

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

#if COLOR

    LDrawParser parser("/home/shaolan/Documents/ldraw/parts/3001.dat");

#endif
}
