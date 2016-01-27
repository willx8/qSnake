#include "gui/qgameboard.h"
#include "core/game.h"
#include "gui/qtile.h"
#include "gui/qresetbutton.h"

#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QString>
#include <QPushButton>
#include <QThread>
#include <QDebug>

QGameBoard::~QGameBoard()
{
    delete game;
}

QGameBoard::QGameBoard(QWidget *parent) :
    QWidget(parent)
{
    // set default size
    resize(650,450);

    // create the main layout
    mainLayout = new QVBoxLayout();
    setLayout(mainLayout);

    // will be created in initBoard()
    boardLayout = NULL;

    // create the game and register as observer
    game = new QGame(15, 10 /* columns&rows */,
                   500 /* speed */);
    //game->registerObserver(this);

    // create the gui board and draw it
    gui_board.resize(game->columns());
    for (int i = 0; i < game->columns(); ++i)
        gui_board[i].resize(game->rows());
    for (int i = 0; i < game->columns(); ++i)
        for (int j = 0; j < game->rows(); ++j)
            gui_board[i][j] = NULL;
    initBoard();

    // create the score widget and add it to the board
    score = new QLabel(QString("SCORE: %1").arg(0));
    score->setStyleSheet("QLabel { color: rgb(235,224,214); font: 16pt; }");
    score->setFixedHeight(50);
    mainLayout->insertWidget(1, score, 0, Qt::AlignRight);

    // style sheet of the board
    setStyleSheet("QGameBoard { background-color: rgb(187,173,160) }");

    // add start button
    startButton = new QPushButton("START", this);
    aiButton = new QPushButton("AI mode", this);
    mainLayout->insertWidget(2, startButton, 1, Qt::AlignRight);
    mainLayout->insertWidget(2, aiButton, 1, Qt::AlignRight);
    connect(startButton, SIGNAL (released()), this, SLOT (startGame()));
    connect(aiButton, SIGNAL (released()), this, SLOT (aiGame()));
    connect(gameOverWindow.getResetBtn(), SIGNAL(clicked()), this, SLOT(resetGame()));
    connect(game, SIGNAL(NextState()), this, SLOT(notify()));

    thread = new QThread( );
    game->moveToThread(thread);
    thread->start();
    //connect( thread, SIGNAL(started()), game, SLOT(Run()) );
    //connect( game, SIGNAL(GameOver()), thread, SLOT(quit()) );
    //automatically delete thread and task object when work is done:
    connect( thread, SIGNAL(finished()), game, SLOT(deleteLater()) );
    connect( thread, SIGNAL(finished()), thread, SLOT(deleteLater()) );
    connect(this, SIGNAL(setAi(bool)), game, SLOT(set_ai(bool)));
    connect(this, SIGNAL(clearGame()), game, SLOT(clear()));
    connect(this, SIGNAL(runGame()), game, SLOT(Run()));
    qRegisterMetaType<Directions>( "Directions" );
    connect(this, SIGNAL(setDirection(Directions)), game->snake_, SLOT(SetDirection(Directions)));
    this->setFocus();
    game->snake_->registerDirGetter(this);
    tmp_direction = kUp;
    //connect(game->snake_, SIGNAL(requestDirection()), this, SLOT(responseDirection()));
}

void QGameBoard::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "press: " << event->key();
    switch (event->key()) {
    case Qt::Key_Up:
        tmp_direction = kUp;
        break;
    case Qt::Key_Left:
        tmp_direction = kLeft;
        break;
    case Qt::Key_Right:
        tmp_direction = kRight;
        break;
    case Qt::Key_Down:
        tmp_direction = kDown;
        break;
    }
}

void QGameBoard::notify()
{
    if (game->is_game_over())
        gameOverWindow.show();

    if (game->won()) {
        score->setText(QString("You won, congratulations!"));
        gameOverWindow.show();
    }
    else {
        score->setText(QString("SCORE: %1").arg(game->score()));
    }

    drawBoard();
}

void QGameBoard::drawBoard()
{
    // draw head
    gui_board[game->snake_head.x][game->snake_head.y]->draw_head();
    // delete previous tail
    gui_board[game->snake_prev_tail.x][game->snake_prev_tail.y]->draw_background();
    // draw food if food position changed.
    gui_board[game->food.x][game->food.y]->draw_food();
}

void QGameBoard::initBoard()
{
    delete boardLayout;
    boardLayout = new QGridLayout();
    boardLayout->setSpacing(0);
    for (int x = 0; x < game->columns(); ++x) {
        for (int y = 0; y < game->rows(); ++y) {
            delete gui_board[x][y];
            gui_board[x][y] = new QTile();
            boardLayout->addWidget(gui_board[x][y], y, x);
            gui_board[x][y]->draw_background();
        }
    }
    mainLayout->insertLayout(0, boardLayout);
}

void QGameBoard::resetGame()
{
    initBoard();
    clearGame();
    score->setText(QString("SCORE: %1").arg(0));
    gameOverWindow.hide();
}

void QGameBoard::startGame()
{
    setAi(false);
    runGame();
    qDebug() << "starting";
}

void QGameBoard::aiGame()
{
    setAi(true);
    runGame();
    //bool s = QMetaObject::invokeMethod(game, "set_ai", Qt::QueuedConnection, Q_ARG(bool, true));
    //printf("success: %d\n", s);
}
