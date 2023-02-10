#include "widget.h"
#include "graphmanager.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    GraphManager gm;

    Widget w{gm};
    //w.setWindowState(Qt::WindowMaximized);
    w.show();
    return a.exec();
}
