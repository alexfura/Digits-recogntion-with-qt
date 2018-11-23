#include "map.h"
#include <QDebug>
#include <QObject>
#include <QHeaderView>
#include <QTableWidget>

Map::Map(QWidget *parent): QTableWidget (parent)
{
    this->setColumnCount(8);
    this->setRowCount(8);
    this->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->setSelectionMode(QAbstractItemView::NoSelection);

    qDebug() <<"Object created";
}


void Map::drawItem(int row, int col)
{
    this->item(row, col)->setBackgroundColor(Qt::black);
}


