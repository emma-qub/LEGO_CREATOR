#include <QApplication>
#include <QtGui>

#include "MainWindow.h"

#include "World.h"

int main(int argc, char** argv) {
    srand(time(NULL));

    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}
