#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <armadillo>
#include "network.h"


using namespace arma;

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void train();
    void count();


public Q_SLOTS:
    void cellSelected(int nRow, int nCol);
    void predict();
    void clear();

private:
    void init_map();
    Ui::MainWindow *ui;
    Mat<double> digit;
    network * net;

};

#endif // MAINWINDOW_H
