#ifndef MAP_H
#define MAP_H

#include <QTableWidget>


class Map : public QTableWidget
{
public:
    explicit Map(QWidget *parent = nullptr);

private:
    void drawItem(int row, int col);
};

#endif // MAP_H
