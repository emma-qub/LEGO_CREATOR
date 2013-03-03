SOURCES += \
    main.cpp \
    MainWindow.cpp \
    ViewerWidget.cpp \
    Lego.cpp \
    LegoDialog.cpp \
    LegoNode.cpp \
    Brick.cpp \
    BrickNode.cpp \
    BrickDialog.cpp \
    LegoFactory.cpp \
    Corner.cpp \
    CornerNode.cpp \
    CornerDialog.cpp \
    Road.cpp \
    RoadNode.cpp \
    RoadDialog.cpp \
    World.cpp \
    BoundingBox.cpp \
    GenerateRoadWindow.cpp \
    Character.cpp \
    CharacterDialog.cpp \
    CharacterNode.cpp \
    Window.cpp \
    WindowNode.cpp \
    WindowDialog.cpp \
    Door.cpp \
    DoorNode.cpp \
    DoorDialog.cpp \
    FromFile.cpp \
    FromFileNode.cpp \
    FromFileDialog.cpp \
    Traffic.cpp \
    Wheel.cpp \
    WheelNode.cpp \
    WheelDialog.cpp \
    Commands.cpp \
    Tile.cpp \
    TileNode.cpp \
    TileDialog.cpp \
    FrontShip.cpp \
    FrontShipNode.cpp \
    FrontShipDialog.cpp \
    SettingsDialog.cpp \
    Pages.cpp

HEADERS += \
    MainWindow.h \
    ViewerWidget.h \
    Lego.h \
    LegoDialog.h \
    LegoNode.h \
    Brick.h \
    BrickNode.h \
    BrickDialog.h \
    LegoFactory.h \
    Corner.h \
    CornerNode.h \
    CornerDialog.h \
    Road.h \
    RoadNode.h \
    RoadDialog.h \
    World.h \
    BoundingBox.h \
    GenerateRoadWindow.h \
    Character.h \
    CharacterDialog.h \
    CharacterNode.h \
    Window.h \
    WindowNode.h \
    WindowDialog.h \
    Door.h \
    DoorNode.h \
    DoorDialog.h \
    FromFile.h \
    FromFileNode.h \
    FromFileDialog.h \
    Traffic.h \
    Wheel.h \
    WheelNode.h \
    WheelDialog.h \
    Commands.h \
    Tile.h \
    TileNode.h \
    TileDialog.h \
    FrontShip.h \
    FrontShipNode.h \
    FrontShipDialog.h \
    SettingsDialog.h \
    Pages.h

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
