#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>

#include "ViewerWidget.h"
#include "LegoDialog.h"
#include "World.h"


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0);
    virtual ~MainWindow();

    void initFactories(void);
    void initPreview(void);
    void initDialogs(void);

    void setStyle(void);

    void createFileMenu(void);
    void createGenerateMenu(void);
    void createTrafficMenu(void);
    void createHelpMenu(void);
    void createParamsDock(void);
    void createMoveDock(void);
    void createScene(void);

    void chooseRoad(int i, int j, int width, int length, bool roadTop, bool roadRight);

    void writeFile(const QString& fileName);

public slots:
    void browseColor(void);
    void chooseDialog(int dialogIndex);
    void legoUpdated(LegoGeode* legoGeode);

    void createLego(void);
    void fitLego(void);

    void translate(int);
    void rotateLeft(void);
    void rotateRight(void);

    void generateRoad(void);
    void generateHouse(void);

    void eraseScene(void);
    void newFile(void);
    void openFile(void);
    void saveFile(void);
    void saveAsFile(void);
    void quitSoft(void);

    void checkExistence(QString fileName);

    void recordPath(void);
    void viewTraffic(void);

private:
    //QTabWidget* _tabs;

    ViewerWidget* _brickViewer;
    ViewerWidget* _sceneViewer;
    QFrame* _sceneFrame;

    QWidget* _paramsWidget;
    QDockWidget* _paramsDock;

    QComboBox* _shapeComboBox;
    QPushButton* _colorButton;
    QPushButton* _createButton;

    QWidget* _moveWidget;
    QDockWidget* _moveDock;

    QSpinBox* _xTransSpinBox;
    QSpinBox* _yTransSpinBox;
    QSpinBox* _zTransSpinBox;
    QPushButton* _leftRotateButton;
    QPushButton* _rightRotateButton;
    QPushButton* _fitButton;
    QPushButton* _deleteButton;

    QAction* _newAction;
    QAction* _openAction;
    QAction* _saveAction;
    QAction* _saveAsAction;
    QAction* _quitAction;

    QAction* _generateRoadAction;
    QAction* _generateBuildingAction;
    QAction* _generateHouseAction;

    QAction* _recordPathAction;
    QAction* _viewTrafficAction;

    QAction* _helpAction;
    QAction* _aboutAction;

    QColor _legoColor;

    osg::ref_ptr<osg::Group> _scene;
    osg::ref_ptr<LegoGeode> _currLegoGeode;
    Lego* _currLego;

    QVector<LegoDialog*> _legoDialog;

    World _world;

    QVector<QVector<QVector<bool> > > _roadPath;

    QSettings _settings;

    bool _alreadySaved;
    bool _saved;

signals:
    void fileAlreadyExists(bool);
};

#endif // MAINWINDOW_H
