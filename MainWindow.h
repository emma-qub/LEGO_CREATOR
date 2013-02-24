#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>

#include "ViewerWidget.h"
#include "LegoDialog.h"
#include "World.h"

#include "Traffic.h"


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
    void createEditMenu(void);
    void createGenerateMenu(void);
    void createTrafficMenu(void);
    void createHelpMenu(void);
    void createUndoView(void);
    void createParamsDock(void);
    void createMoveDock(void);
    void createScene(void);
    void initTraffic(void);

    void chooseRoad(int i, int j, int width, int length, bool roadTop, bool roadRight);

    void openFromFile(const QString& fileName);
    void writeFile(const QString& fileName);

public slots:
    void browseColor(void);
    void chooseDialog(int dialogIndex);
    void legoUpdated(LegoGeode* legoGeode);

    void createLego(void);
    void fitLego(void);
    void deleteLego(void);

    void translate(int);
    void rotateLeft(void);
    void rotateRight(void);

    void generateRoad(void);
    void generateHouse(void);
    void generateCity(void);

    void eraseScene(void);
    void newFile(void);
    void openFile(void);
    void saveFile(void);
    void saveAsFile(void);
    void quitSoft(void);

    void checkExistence(QString fileName);

    void viewTraffic(bool trafficOn = false);

    void freezeFit(void);
    void freezeCreate(void);

private:
    //QTabWidget* _tabs;
    Traffic* _traffic;

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

    QAction* _undoAction;
    QAction* _redoAction;

    QAction* _generateRoadAction;
    QAction* _generateHouseAction;
    QAction* _generateCityAction;

    QAction* _viewTrafficAction;

    QAction* _helpAction;
    QAction* _aboutAction;

    QColor _legoColor;

    osg::ref_ptr<osg::Group> _scene;
    osg::ref_ptr<osg::MatrixTransform> _currMatTrans;
    osg::ref_ptr<LegoGeode> _currLegoGeode;
    osg::ref_ptr<Lego> _currLego;

    QVector<LegoDialog*> _legoDialog;

    World _world;

    QVector<QVector<QVector<bool> > > _roadPath;

    QSettings _settings;

    bool _alreadySaved;
    bool _saved;

    QUndoStack* _undoStack;
    QUndoView* _undoView;

signals:
    void fileAlreadyExists(bool);
};

#endif // MAINWINDOW_H
