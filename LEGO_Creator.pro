SOURCES += \
    main.cpp \
    MainWindow.cpp \
    ViewerWidget.cpp \
    Lego.cpp \
    LegoDialog.cpp \
    Brick.cpp \
    LegoGeode.cpp \
    BrickGeode.cpp \
    BrickDialog.cpp \
    LegoFactory.cpp \
    Corner.cpp \
    CornerGeode.cpp \
    CornerDialog.cpp \
    Road.cpp \
    RoadGeode.cpp \
    RoadDialog.cpp

HEADERS += \
    MainWindow.h \
    ViewerWidget.h \
    Lego.h \
    LegoDialog.h \
    Brick.h \
    LegoGeode.h \
    BrickGeode.h \
    BrickDialog.h \
    LegoFactory.h \
    Corner.h \
    CornerGeode.h \
    CornerDialog.h \
    Road.h \
    RoadGeode.h \
    RoadDialog.h

LIBS += \
    -L$OSG/lib \
    -losgQt \
    -losg \
    -losgGA \
    -losgDB \
    -losgViewer \
    #-losgTerrain \
    -L

QT += opengl

CONFIG += thread
