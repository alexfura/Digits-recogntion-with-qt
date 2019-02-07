#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Digit recognizer app");

    w.show();


    return a.exec();
}
