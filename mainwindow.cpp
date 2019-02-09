#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAbstractItemView>
#include <QDebug>
#include <QMessageBox>
#include <QTableWidget>

#include <QString>

using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init_map();

    connect(ui->map, SIGNAL( cellEntered (int, int) ), this, SLOT( cellSelected( int, int ) ));
    connect(ui->clearButt, &QPushButton::clicked, this, [this]{this->clear();});
    digit = zeros(1, 784);

    net = new network(784, 100, 10);

    net->load("mnist_train.csv");
    net->MBGD(12, 12, 0.01, 0, 0.9);
    net->count_score();

    net->load("mnist_test.csv");

    net->count_score();
    net->save_results();
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::init_map()
{
    ui->map->setRowCount(28);
    ui->map->setColumnCount(28);

    ui->map->setSelectionMode(QAbstractItemView::NoSelection);
    ui->map->verticalHeader()->hide();
    ui->map->horizontalHeader()->hide();

    ui->map->setShowGrid(false);

    ui->map->verticalHeader()->setMaximumSectionSize(this->ui->map->height() / 28);
    ui->map->horizontalHeader()->setMaximumSectionSize(this->ui->map->width() /28);

    ui->map->verticalHeader()->setMinimumSectionSize(this->ui->map->height() / 28);
    ui->map->horizontalHeader()->setMinimumSectionSize(this->ui->map->width() /28);


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
    this->ui->map->item(nRow, nCol)->setBackgroundColor(Qt::black);

    if(nRow != 27)
    {
        this->ui->map->item(nRow+1, nCol)->setBackgroundColor(Qt::black);
        this->digit.at(0, nCol + (nRow +1) * 28) = 1;
    }
    this->digit.at(0, nCol + nRow * 28) = 1;
    this->predict();
}

void MainWindow::predict()
{
    int predicted = net->predict(this->digit);

    this->ui->prediction->setNum(predicted);
}


void MainWindow::clear()
{
    for(int row = 0;row < ui->map->rowCount();row++)
    {
        for(int col = 0;col < ui->map->columnCount();col++)
        {
            this->ui->map->item(row, col)->setBackgroundColor(Qt::white);
        }
    }

    digit.zeros(1, 784);

    this->ui->prediction->clear();
}
