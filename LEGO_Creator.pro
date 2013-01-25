SOURCES += \
    main.cpp \
    MainWindow.cpp \
    ViewerWidget.cpp

HEADERS += \
    MainWindow.h \
    ViewerWidget.h

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
