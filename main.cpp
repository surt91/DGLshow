#include <QApplication>

#include "trajectory.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Trajectory pic;
    pic.show();

    return a.exec();
}
