#ifndef QGAMEBOARD_H
#define QGAMEBOARD_H

#include "core/observer.h"
#include "gui/qgameoverwindow.h"
#include "core/snake_graph.h"
#include "core/game.h"

#include <QVector>
#include <QWidget>
#include <QPushButton>

class QResetButton;
//class QGame;
class QKeyEvent;
class QTile;
class QGridLayout;
class QVBoxLayout;
class QLabel;

class QGameBoard : public QWidget
{
    Q_OBJECT
public:
    explicit QGameBoard(QWidget *parent = 0);
    ~QGameBoard();

    Directions dir() {
        return tmp_direction;
    }

private:

    QGame *game;
    // gui representation of board
    QVector<QVector<QTile*> > gui_board;
    // main layout
    QVBoxLayout *mainLayout;
    // grid layout of board
    QGridLayout *boardLayout;
    // start button
    QPushButton *startButton;
    QPushButton *aiButton;
    // score widget
    QLabel *score;
    // game over widget
    QGameOverWindow gameOverWindow;
    // winner widget
    QLabel *youwin;
    // thread that controls game.
    QThread *thread;
    // direction cache
    Directions tmp_direction;

    void drawBoard();
    void initBoard();


protected:
    void keyPressEvent(QKeyEvent *event);

signals:
    void setAi(bool ai);
    void setDirection(Directions dir);
    void clearGame();
    void runGame();

public slots:
    void resetGame();
    void startGame();
    void aiGame();
    void notify();

};

#endif // QGAMEBOARD_H
