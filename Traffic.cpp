#include "Traffic.h"

#include <QSettings>
#include <QDir>

Traffic::Traffic() {
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

    // Create animation path
    _trafficPath = new osg::AnimationPath;

    // Create vehicules from vehicules directory
    std::vector<std::string> vehicules;
    foreach (QString v, vehiculesList) {
        vehicules.push_back(v.toStdString());
    }

    _vehicules = dynamic_cast<osg::Group*>(osgDB::readNodeFiles(vehicules));

    // Reading animation
    std::ifstream file((recordPath+recordFileName).toStdString().c_str(), std::ios::in);
    if (file) {
        _trafficPath->read(file);
        file.close();
    }
}

osg::ref_ptr<osg::AnimationPath> Traffic::getTrafficPath(void) {
    return _trafficPath.get();
}

osg::ref_ptr<osg::Node> Traffic::getVehicules() {
    return _vehicules.get();
}

void Traffic::runTrafic(osg::ref_ptr<osg::Group> root) {
    // Loop on vehicules
    for (unsigned int i = 0 ; i < _vehicules->getNumChildren(); i++) {
        // Apply animationPathCallback
        osg::ref_ptr<osg::MatrixTransform> t = new osg::MatrixTransform;
        osg::ref_ptr<osg::AnimationPathCallback> aniCallback = new osg::AnimationPathCallback(_trafficPath.get(),i+5,i+1); // depart, vitesse
        t->setUpdateCallback(aniCallback.get());

        // Transformation on vehicule
        osg::ref_ptr<osg::MatrixTransform> t2 = new osg::MatrixTransform;
        osg::Matrix m;
        m.makeTranslate(osg::Vec3(0,0,-3.8));
        t2->setMatrix(m);

        // Add vehicule
        t2->addChild(_vehicules->getChild(i));
        t->addChild(t2);
        root->addChild(t.get());
    }
}
