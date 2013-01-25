SOURCES += \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    MainWindow.h

LIBS += \
    -L$OSG/lib \
    -losgQt \
    -losg \
    -losgGA \
    -losgDB \
    -losgViewer \
    -losgTerrain \
    -L

QT += opengl

CONFIG += thread
