#include <QApplication>
#include <QtGui>

#include "MainWindow.h"

#include "World.h"

int main(int argc, char** argv) {

    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}


















//#include <QtGui/QApplication>
//#include <iostream>

////#include "ViewerWidget.h"
//#include "ViewerQT.h"






//osg::ref_ptr<osg::Drawable> createCube(void) {
//    // On cree un cube
//    osg::ref_ptr<osg::Geometry> cube = new osg::Geometry;

//    // On cree le tableau des sommets
//    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;

//    // On cree les 8 sommets du cube
//    osg::Vec3 A(-1, 1, 1);
//    osg::Vec3 B(1, 1, 1);
//    osg::Vec3 C(1, -1, 1);
//    osg::Vec3 D(-1, -1, 1);
//    osg::Vec3 E(-1, 1, -1);
//    osg::Vec3 F(1, 1, -1);
//    osg::Vec3 G(1, -1, -1);
//    osg::Vec3 H(-1, -1, -1);

//    // On cree les 6 faces a partir des 8 sommets
//    // Face du bas
//    vertices->push_back(C);
//    vertices->push_back(G);
//    vertices->push_back(H);
//    vertices->push_back(D);

//    // Face du haut
//    vertices->push_back(B);
//    vertices->push_back(A);
//    vertices->push_back(E);
//    vertices->push_back(F);

//    // Face devant
//    vertices->push_back(A);
//    vertices->push_back(B);
//    vertices->push_back(C);
//    vertices->push_back(D);

//    // Face derriere
//    vertices->push_back(E);
//    vertices->push_back(F);
//    vertices->push_back(G);
//    vertices->push_back(H);

//    // Face gauche
//    vertices->push_back(A);
//    vertices->push_back(D);
//    vertices->push_back(H);
//    vertices->push_back(E);

//    // Face droite
//    vertices->push_back(B);
//    vertices->push_back(F);
//    vertices->push_back(G);
//    vertices->push_back(C);

//    // On fait correspondre les sommets au cube
//    cube->setVertexArray(vertices);

//    // On cree le tableau des couleurs
//    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;

//    // On ajoute les couleurs
//    colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f) ); // rouge
//    colors->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f) ); // vert
//    colors->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f) ); // bleu
//    colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) ); // blanc
//    colors->push_back(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f) ); // jeune
//    colors->push_back(osg::Vec4(0.95f, 0.48f, 0.11f, 1.0f) ); // orange

//    // On fait correspondre les couleurs aux faces du cube
//    cube->setColorArray(colors);
//    cube->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);

//    // On cree les normales
//    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;

//    // On ajoute les normales DANS LE MEME ORDRE QUE LES FACES
//    normals->push_back(osg::Vec3(0, -1, 0));
//    normals->push_back(osg::Vec3(0, 1, 0));
//    normals->push_back(osg::Vec3(0, 0, 1));
//    normals->push_back(osg::Vec3(0, 0, -1));
//    normals->push_back(osg::Vec3(-1, 0, 0));
//    normals->push_back(osg::Vec3(1, 0, 0));

//    // On fait correspondre les normales aux faces du cube
//    cube->setNormalArray(normals);
//    cube->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE);

//    // On définit les quadrilatères GL_QUADS avec les 24 sommets
//    cube->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 24));

//    // On retourne le pointeur sur Drawable representant le cube
//    return cube.get();
//}







//int main( int argc, char** argv ) {
////    osg::ArgumentParser arguments(&argc, argv);

////    osgViewer::ViewerBase::ThreadingModel threadingModel = osgViewer::ViewerBase::CullDrawThreadPerContext;
////    while (arguments.read("--SingleThreaded")) threadingModel = osgViewer::ViewerBase::SingleThreaded;
////    while (arguments.read("--CullDrawThreadPerContext")) threadingModel = osgViewer::ViewerBase::CullDrawThreadPerContext;
////    while (arguments.read("--DrawThreadPerContext")) threadingModel = osgViewer::ViewerBase::DrawThreadPerContext;
////    while (arguments.read("--CullThreadPerCameraDrawThreadPerContext")) threadingModel = osgViewer::ViewerBase::CullThreadPerCameraDrawThreadPerContext;

//    QApplication app(argc, argv);
//    //ViewerWidget* viewWidget = new ViewerWidget(threadingModel);
//    ViewerQT* viewWidget = new ViewerQT;


//    osg::ref_ptr<osg::Group> scene = new osg::Group;

//    osg::ref_ptr<osg::Geode> cube = new osg::Geode;
//    cube->addDrawable(createCube());

//    scene->addChild(cube);

//    osg::ref_ptr<osg::Camera> camera = new osg::Camera;
//    camera->setProjectionMatrix(osg::Matrix::perspective(30, 16/9, 10, 100));
//    camera->setViewMatrix(osg::Matrix::identity());

//    viewWidget->setSceneData(scene);
//    viewWidget->setCamera(camera);

//    viewWidget->setGeometry( 100, 100, 800, 600 );
//    while (!viewWidget->done()) {
//        viewWidget->frame();
//        viewWidget->update();
//    }


//    return app.exec();
//}


















