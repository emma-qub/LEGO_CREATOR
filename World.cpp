#include "World.h"

#include <QDebug>
#include <QSettings>

#include "LegoFactory.h"

#include <osgUtil/Optimizer>

int World::minHeight = 0;
int World::maxHeight = 100;
int World::minWidth = -500;
int World::maxWidth = 500;
int World::minLength = -500;
int World::maxLength = 500;
int World::count = 0;

World::World() {
    // Initialize matrix transform indexes
    _matTransIndexes = QVector<unsigned int>(0);

    // Create scene
    _scene = new osg::Group;
    _currMatrixTransform = new osg::MatrixTransform;

    // Add guide lines
    createGuideLines();

    // Optimizer
    osgUtil::Optimizer optimizer;
    optimizer.optimize(_scene);
}

World::~World(void) {
}

void World::createGuideLines() {
    // Get width and length defined within settings
    QSettings settings(QSettings::UserScope, "Perso", "Lego Creator");

    // Get whether grid is visible
    bool isGridVisible;
    if (settings.childKeys().contains("ViewerGridVisible")) {
        isGridVisible = settings.value("ViewerGridVisible").toBool();
    } else {
        isGridVisible = settings.value("DefaultViewerGridVisible").toBool();
    }

    // If grid is visible, OK, otherwise, no need to do this part
    if (isGridVisible) {

        // Remove previous lines
        removeGuideLines();

        // Create line geode
        osg::ref_ptr<osg::Geode> line = new osg::Geode;

        int width = 0;
        if (settings.childKeys().contains("ViewerWidth")) {
            width = settings.value("ViewerWidth").toInt();
        } else {
            width = settings.value("DefaultViewerWidth").toInt();
        }

        int length = 0;
        if (settings.childKeys().contains("ViewerLength")) {
            length = settings.value("ViewerLength").toInt();
        } else {
            length = settings.value("DefaultViewerLength").toInt();
        }

        // Get viewer background color
        QColor bgColor;
        if (settings.childKeys().contains("ViewerColor")) {
            bgColor = settings.value("ViewerColor").value<QColor>();
        } else {
            bgColor = settings.value("DefaultViewerColor").value<QColor>();
        }

        // Calculate whether background color is dark
        bool isViewerBgDark = (bgColor.black() > 127);

        for (int i = -width; i <= width; i+=2) {
            for (int j = -length; j <= length; j+=2) {
                // Create four points
                osg::Vec3 v0(i*Lego::length_unit, -length*Lego::length_unit, -0.1);
                osg::Vec3 v1(i*Lego::length_unit, length*Lego::length_unit, -0.1);
                osg::Vec3 v2(-width*Lego::length_unit, j*Lego::length_unit, -0.1);
                osg::Vec3 v3(width*Lego::length_unit, j*Lego::length_unit, -0.1);

                // Add points
                osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
                vertices->push_back(v0);
                vertices->push_back(v1);
                vertices->push_back(v2);
                vertices->push_back(v3);

                // Create geometry
                osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;
                geometry->setVertexArray(vertices);

                // Create color according to viewer color :
                // if background color is dark, grid is white
                // otherwise, grid is black
                osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
                osg::Vec4 colorVec(0.0, 0.0, 0.0, 1.0);
                if (isViewerBgDark)
                    colorVec.set(1.0, 1.0, 1.0, 1.0);
                colors->push_back(colorVec);

                // Match color
                geometry->setColorArray(colors);
                geometry->setColorBinding(osg::Geometry::BIND_OVERALL);

                // Define line
                geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 4));

                // Add drawables geode
                line->addDrawable(geometry);
            }
        }

        // Give a name
        line->setName("GuideLines");

        // Add line to scene
        _scene->addChild(line);
    }
}

void World::removeGuideLines(void) {
    for (unsigned int k = 0; k < _scene->getNumChildren(); k++) {
        // If child is the previous guide lines, we remove it
        if (_scene->getChild(k)->getName() == "GuideLines")
            _scene->removeChild(k);
    }
}

void World::initBrick(void) {
    // LEGO parts in preview are centered in origin.
    // The goal here is to translate them to match there leftBottomFront corner with the origin.

    // Get current LegoNode
    LegoNode* currLegoNode = static_cast<LegoNode*>(_currMatrixTransform->getChild(0));

    // Get the Bounding Box
    BoundingBox box = currLegoNode->getLego()->getBoundingBox();

    // Calculate x, y and z translations
    _x = Lego::length_unit*box.getLength()/2.0;
    _y = Lego::length_unit*box.getWidth()/2.0;
    _z = Lego::height_unit*box.getHeight()/2.0;

    // Translation
    osg::Matrix mat = _currMatrixTransform->getMatrix();
    mat.makeTranslate(_x, _y, _z);
    _currMatrixTransform->setMatrix(mat);

    _isTurned = false;
}

void World::deleteLego(void) {
    // Remove last Lego inserted
    _scene->removeChild(_matTransIndexes.last());
    // Pop the stack
    _matTransIndexes.pop_back();
}

void World::deleteLego(const std::string& matrixName) {
    // The node to delete
    osg::Node* concernedMatTrans = NULL;
    // We search for the _scene child that has the right name
    for (unsigned int k = 0; k < _scene->getNumChildren(); k++) {
        if (_scene->getChild(k)->getName() == matrixName) {
            concernedMatTrans = _scene->getChild(k);
            break;
        }
    }

    // If we found the right child, we delete it
    if (concernedMatTrans)
        _scene->removeChild(concernedMatTrans);
    // Else, we print a message...
    else
        qDebug() << "Cannot find the right child within World::deleteLego";
}

std::string World::addBrick(LegoNode* legoNode, Lego* lego) {
    // ClonelegoNode and Lego to create a new one in the scene
    osg::ref_ptr<LegoNode> newLegoNode = legoNode->cloning();
    osg::ref_ptr<Lego> newLego = lego->cloning();
    newLegoNode->setLego(newLego.get());

    // Create a matrix transform parent
    _currMatrixTransform = new osg::MatrixTransform;
    _currMatrixTransform->addChild(newLegoNode.get());
    // Because LEGO bricks don't move
    _currMatrixTransform->setDataVariance(osg::Object::STATIC);
    _scene->addChild(_currMatrixTransform.get());

    // Assign a brand new name to the previous matrix, in order to find it later
    // First, we increment the counter
    count++;
    // Create the brand new name and assign it
    std::string matrixName = QString("MatrixTransform%1").arg(count).toStdString();
    _currMatrixTransform->setName(matrixName);

    // Init brick, to place it at the right place
    initBrick();

    // Add curr matrix transform index in array
    _matTransIndexes << _scene->getChildIndex(_currMatrixTransform);

    // Return the matrix transform name, because we need to record it within addCommand class
    // Therefore, we will be able to find it later, when undo/redo several times
    return matrixName;
}

void World::rotation(bool counterClockWise) {
    // Calculate rotation direction
    double direction = 1.0;
    if (counterClockWise)
        direction = -1.0;

    // Rotate according to direction with z axis
    osg::Matrix rotate;
    rotate.makeRotate(osg::Quat(direction*M_PI/2, osg::Vec3(0, 0, 1)));

    // If pairwise is different between width and length, we have to move the Lego of 1/2 Lego unit on x and y axis
    int x = static_cast<int>(2*_x/Lego::length_unit);
    int y = static_cast<int>(2*_y/Lego::length_unit);
    if ((x%2==0 && y%2==1) || (x%2==1 && y%2==0)) {
        osg::Matrix mat = _currMatrixTransform->getMatrix();
        mat.makeTranslate(-Lego::length_unit/2, Lego::length_unit*1/2, 0);
        _currMatrixTransform->preMult(mat);
        _isTurned = !_isTurned;
    }

    // Apply rotation.
    // NB: rotate * mat -> local rotation
    //     mat * rotate -> global rotation
    _currMatrixTransform->preMult(rotate);
}

void World::translationXYZ(double x, double y, double z) {
    // Multiply by Lego units
    x *= Lego::length_unit;
    y *= Lego::length_unit;
    z *= Lego::height_unit;

    // Create translation matrix
    osg::Matrix mat = _currMatrixTransform->getMatrix();
    osg::Vec3d trans = mat.getTrans();
    mat.makeTranslate(-trans[0]+x+_x, -trans[1]+y+_y, -trans[2]+z+_z);

    // Post mult
    _currMatrixTransform->postMult(mat);

    // Watch out the oddwise: pieces with different oddwise between length and width values
    // have to be shifted of 1/2 on both length and width to match Lego plots
    if (_isTurned) {
        osg::Matrix mat = _currMatrixTransform->getMatrix();
        mat.makeTranslate(Lego::length_unit/2, -Lego::length_unit*1/2, 0);
        _currMatrixTransform->preMult(mat);
    }
}

void World::translation(double x, double y, double z) {
    // Multiply by Lego units
    x *= Lego::length_unit;
    y *= Lego::length_unit;
    z *= Lego::height_unit;

    // Create translation matrix
    osg::Matrix mat = _currMatrixTransform->getMatrix();
    mat.makeTranslate(x, y, z);
    _currMatrixTransform->setMatrix(mat);
}
