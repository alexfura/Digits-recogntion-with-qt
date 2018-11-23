#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAbstractItemView>
#include <QDebug>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init_map();

    connect(ui->map, SIGNAL( cellEntered (int, int) ), this, SLOT( cellSelected( int, int ) ));

}

MainWindow::~MainWindow()
{
    delete ui;
}





void MainWindow::init_map()
{
    ui->map->setRowCount(8);
    ui->map->setColumnCount(8);

    ui->map->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->map->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->map->setSelectionMode(QAbstractItemView::NoSelection);

    for(int row = 0;row < ui->map->rowCount();row++)
    {
        for(int col = 0;col < ui->map->columnCount();col++)
        {
            ui->map->setItem(row, col, new QTableWidgetItem(""));
        }
    }

}


void MainWindow::cellSelected(int nRow, int nCol)
{
    qDebug() <<nRow<<nCol <<"Row and col";
    this->ui->map->item(nRow, nCol)->setBackgroundColor(Qt::black);
}
