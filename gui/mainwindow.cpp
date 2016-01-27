#include "gui/mainwindow.h"
//#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent) /*,
    ui(new Ui::MainWindow) */
{
    //ui->setupUi(this);
    gameBoard = new QGameBoard ();
    setCentralWidget(gameBoard);
}

MainWindow::~MainWindow()
{
    //delete ui;
}
