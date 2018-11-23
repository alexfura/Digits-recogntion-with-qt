#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

private slots:
    void cellSelected(int nRow, int nCol);

private:
    void init_map();
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
