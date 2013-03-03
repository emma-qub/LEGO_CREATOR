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
    Character.cpp \
    CharacterDialog.cpp \
    CharacterGeode.cpp \
    Window.cpp \
    WindowGeode.cpp \
    WindowDialog.cpp \
    Door.cpp \
    DoorGeode.cpp \
    DoorDialog.cpp \
    FromFile.cpp \
    FromFileGeode.cpp \
    FromFileDialog.cpp \
    Traffic.cpp \
    Wheel.cpp \
    WheelGeode.cpp \
    WheelDialog.cpp \
    Commands.cpp \
    Tile.cpp \
    TileGeode.cpp \
    TileDialog.cpp \
    FrontShip.cpp \
    FrontShipGeode.cpp \
    FrontShipDialog.cpp

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
    Character.h \
    CharacterDialog.h \
    CharacterGeode.h \
    Window.h \
    WindowGeode.h \
    WindowDialog.h \
    Door.h \
    DoorGeode.h \
    DoorDialog.h \
    FromFile.h \
    FromFileGeode.h \
    FromFileDialog.h \
    Traffic.h \
    Wheel.h \
    WheelGeode.h \
    WheelDialog.h \
    Commands.h \
    Tile.h \
    TileGeode.h \
    TileDialog.h \
    FrontShip.h \
    FrontShipGeode.h \
    FrontShipDialog.h

LIBS += \
    -L$OSG/lib \
    -losgQt \
    -losg \
    -losgGA \
    -losgDB \
    -losgViewer \
    -lOpenThreads \
    -losgUtil \
    #-losgTerrain \
    #-lX11 \
    -L

QT += opengl

CONFIG += thread
