#include "Traffic.h"

#include <QDebug>
#include <QSettings>
#include <QDir>

Traffic::Traffic() {
    // Create switch node
    _root = new osg::Switch;

    // By default, when adding a child, it has to be hidden
    _root->setNewChildDefaultValue(false);

    // Create animation path
    _trafficPath = new osg::AnimationPath;

    // Create vehicules switch node
    _vehicules = new osg::Switch;

    // Add all vehicules
    createTraffic();
}

void Traffic::addVehicules(void) {
    // Get settings
    QSettings settings(QSettings::UserScope, "Perso", "Lego Creator");

    // Get record path
    QString recordPath = settings.value("RecordPath").toString();
    if (!recordPath.endsWith('/'))
        recordPath += '/';

    // Get record file name
    QString recordFileName = settings.value("RecordFileName").toString();

    // Get Vehicules directory path
    QString vehiculesPath = settings.value("VehiculesPath").toString();
    if (!vehiculesPath.endsWith('/'))
        vehiculesPath += '/';

    // Get every vehicules in vehicules directory
    QDir vehiculesDir = QDir(vehiculesPath);
    QStringList vehiculesList = vehiculesDir.entryList(QDir::Files | QDir::NoDotAndDotDot);

    // Create vehicules from vehicules directory
    std::vector<std::string> vehicules;
    foreach (QString v, vehiculesList) {
        vehicules.push_back((vehiculesPath+v).toStdString());
    }

    // Add vehicules from files
    _vehicules->addChild(osgDB::readNodeFiles(vehicules));

    // Reading animation
    std::ifstream file((recordPath+recordFileName).toStdString().c_str(), std::ios::in);
    if (file) {
        _trafficPath->read(file);
        file.close();
    }
}

void Traffic::createTraffic(void) {
    // In case _vehicules is not empty
    _vehicules->removeChildren(0, _vehicules->getNumChildren());

    // Add vehicules from files
    addVehicules();

    // Loop on vehicules
    for (unsigned int i = 0 ; i < _vehicules->getNumChildren(); i++) {
        // Apply animationPathCallback
        osg::ref_ptr<osg::MatrixTransform> t = new osg::MatrixTransform;
        t->setName("transformation");
        osg::ref_ptr<osg::AnimationPathCallback> aniCallback = new osg::AnimationPathCallback(_trafficPath.get(),i+5,i+1); // depart, vitesse
        t->setUpdateCallback(aniCallback.get());

        // Transformation on vehicule
        osg::ref_ptr<osg::MatrixTransform> t2 = new osg::MatrixTransform;
        t2->setName("transformation");
        osg::Matrix m;
        //m.preMultScale(osg::Vec3(20.0, 20.0, 20.0));
        m.preMultRotate(osg::Quat(-M_PI/2, osg::Vec3d(1.0, 0.0, 0.0)));
        m.preMultRotate(osg::Quat(M_PI, osg::Vec3d(0.0, 0.0, 1.0)));
        //m.makeTranslate(osg::Vec3(0,0,-3.8));
        t2->setMatrix(m);

        // Add vehicule and transformations
        Node* currVehicule = _vehicules->getChild(i);
        currVehicule->setName("vehicule");
        t2->addChild(currVehicule);
        t->addChild(t2);
        _root->addChild(t.get());
    }
}

void Traffic::switchTraffic(bool b) {
    qDebug() << _root->getNumChildren() << b;
    for (unsigned int k = 0; k < _root->getNumChildren(); k++)
        _root->setValue(k, b);
}
