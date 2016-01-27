#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include <random>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <QObject>
#include "core/snake.h"
#include "core/snake_graph.h"
#include "core/subject.h"

class QGame : public QObject {

    Q_OBJECT
 public:
  QGame(int columns, int rows, int speed_mode) : QObject(0), kRefreshSpeed(speed_mode)  {
      srand(time(NULL));

      snake_ = new Snake(this);
      kColumns = columns;
      kRows = rows;
      snake_->SetBorder(kColumns, kRows);
  }

  ~QGame() {;}

  void Welcome();

  bool UpdatePosition();

  bool is_game_over() { return game_over_; }

  bool won() { return false; }

  void UpdateScore();

  void CreateFood();

  int columns() { return kColumns; }
  int rows() { return kRows; }
  int score() { return score_; }
  Position snake_head;
  Position snake_prev_tail;
  Position food;

  Snake *snake_;


 private:
  Position center() {
    return { kColumns / 2, kRows / 2 };
  }

  const int kRefreshSpeed;
  int kColumns;
  int kRows;
  bool game_over_;
  bool user_quit_;
  bool ai_;
  int score_;
  std::mutex print_get_mutex_; // make printw and getch mutex.

 signals:
  void NextState();
  void GameOver();

 public slots:
  void Run();
  void set_ai(bool ai);
  void clear();
//  void SetDirection(Directions direction) {
//      snake_->SetDirection(direction);
//  }

};

#endif
