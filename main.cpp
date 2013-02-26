#include <QApplication>
#include <QtGui>

//#include <X11/Xlib.h>

#include "MainWindow.h"

int main(int argc, char** argv) {
    // Init srand to have pseudo-random numbers
    srand(time(NULL));

//    // Init X11 threads, because I really want to use another thread to render OSG components
//    QCoreApplication::setAttribute(Qt::AA_X11InitThreads);

    // Call QApplication
    QApplication app(argc, argv);

    // Create main window and display it
    MainWindow window;
    window.show();

    // Return application exec
    return app.exec();
}
