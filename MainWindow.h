#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>

#include "ViewerWidget.h"
#include "BrickGeode.h"
#include "BrickDialog.h"
#include "LegoFactory.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0);

    void initFactories(void);
    void initPreview(void);
    void initDialogs(void);

    void createFileMenu(void);
    void createGenerateMenu(void);
    void createHelpMenu(void);
    void createParamsDock(void);

public slots:
    void browseColor(void);
    //void createBrick(int);
    void chooseDialog(int dialogIndex);
    void legoUpdated(LegoGeode* legoGeode);

private:
    QTabWidget* _tabs;

    ViewerWidget* _previewBrick;

    QWidget* _paramsWidget;
    QDockWidget* _paramsDock;

    QComboBox* _legoComboBox;
    QPushButton* _colorButton;

    QAction* _newAction;
    QAction* _openAction;
    QAction* _saveAction;
    QAction* _saveAsAction;
    QAction* _quitAction;

    QAction* _generateBuildingAction;
    QAction* _generateHouseAction;

    QAction* _helpAction;
    QAction* _aboutAction;

    QColor _legoColor;

    osg::ref_ptr<osg::Group> _scene;
    osg::ref_ptr<LegoGeode> _currLegoGeode;
    Lego* _currLego;

    QVector<LegoDialog*> _legoDialog;

//    LegoFactory<Lego, QString> _legoFactory;
//    LegoFactory<LegoGeode> _legoGeodeFactory;
//    LegoFactory<BrickDialog, QString> _legoDialogFactory;

signals:
    void colorChanged(int);
};

#endif // MAINWINDOW_H
