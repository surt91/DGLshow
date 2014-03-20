#include <QApplication>

#include "mainwindow.h"
#include "trajectory.h"

int main(int argc, char *argv[])
{
    if(argc == 2 && (strcmp(argv[1], "--benchmark") || strcmp(argv[1], "-b")))
    {
        Trajectory bench;
        bench.benchmark();
        return 0;
    }
    else
    {
        QApplication a(argc, argv);
        MainWindow w;
        w.show();

        return a.exec();
    }
}
