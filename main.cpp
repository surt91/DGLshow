#include <QApplication>

#include "trajectory.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Trajectory pic(Lorenz);
    pic.show();

    return a.exec();
}
