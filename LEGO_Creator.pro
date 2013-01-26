SOURCES += \
    main.cpp \
    MainWindow.cpp \
    ViewerWidget.cpp \
    Lego.cpp \
    LegoDialog.cpp \
    Brick.cpp \
    LegoGeode.cpp \
    BrickGeode.cpp

HEADERS += \
    MainWindow.h \
    ViewerWidget.h \
    Lego.h \
    LegoDialog.h \
    Brick.h \
    LegoGeode.h \
    BrickGeode.h

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
