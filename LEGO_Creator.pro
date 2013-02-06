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
    RoadDialog.cpp \
    World.cpp \
    BoundingBox.cpp \
    GenerateRoadWindow.cpp \
    Slop.cpp \
    SlopDialog.cpp \
    SlopGeode.cpp \
    Character.cpp \
    CharacterDialog.cpp \
    CharacterGeode.cpp

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
    RoadDialog.h \
    World.h \
    BoundingBox.h \
    GenerateRoadWindow.h \
    Slop.h \
    SlopDialog.h \
    SlopGeode.h \
    Character.h \
    CharacterDialog.h \
    CharacterGeode.h

LIBS += \
    -L$OSG/lib \
    -losgQt \
    -losg \
    -losgGA \
    -losgDB \
    -losgViewer \
    -lOpenThreads \
    #-losgTerrain \
    -L

QT += opengl

CONFIG += thread
