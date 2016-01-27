#include "gui/mainwindow.h"
#include "gui/qgameboard.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //srand(time(NULL));
    QGameBoard board;
    board.show();

    return a.exec();
}
