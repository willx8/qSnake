#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include <random>
#include <string>
#include <QThread>
#include <QDebug>
#include <stdlib.h>
#include <time.h>
#include "core/snake.h"
#include "core/game.h"

using std::chrono::milliseconds;

//QGame::QGame(int columns, int rows, int speed_mode) : kRefreshSpeed(speed_mode) {
//    srand(time(NULL));

//    snake_ = new Snake();
//    kColumns = columns;
//    kRows = rows;
//    snake_->SetBorder(kColumns, kRows);
//}
 
void QGame::set_ai(bool ai) {
    ai_ = ai;
    snake_->Ai(ai);
}

void QGame::Welcome() {

  }

bool QGame::UpdatePosition() {
    /*
    // This mutex avoid conflict when deals with window.
    // Comment mutexs you may see the strange bug.
    {
      std::unique_lock<std::mutex> lck(print_get_mutex_);
      mvwaddch(window_, snake.head().y, 
          snake.head().x, '*' | COLOR_PAIR(4));
      mvwaddch(window_, snake.prev_tail().y, snake.prev_tail().x, ' ');
      wmove(window_, kDownBound, 0);
      wrefresh(window_);
    }
    */
    snake_head = snake_->head();
    snake_prev_tail = snake_->prev_tail();
    bool game_over = snake_->head().x == 0 ||
      snake_->head().x == kColumns || snake_->head().y == 0 ||
      snake_->head().y == kRows;
    return game_over;
}

void QGame::clear() {
    snake_->clear();
    score_ = -1;
    game_over_ = user_quit_ = false;
}

void QGame::Run() {
    snake_->SetHead(center());
    UpdateScore();
    CreateFood();

    /*
    auto kb_listen = [this] () {
      int ch;
      while (!game_over_ && !user_quit_) {
        std::this_thread::sleep_for(milliseconds(5));
        // This mutex avoid conflict when deals with window.
        // Comment mutexs you may see the strange bug.
        {
          std::unique_lock<std::mutex> lck(print_get_mutex_);
          ch = getch();
        }
        if (ch != ERR) {
          Directions direction = kNoChange;
          switch (ch) {
            case KEY_UP: direction = kUp; break;
            case KEY_DOWN: direction = kDown; break;
            case KEY_LEFT: direction = kLeft; break;
            case KEY_RIGHT: direction = kRight; break;
            case 'q': user_quit_ = true; break;
            default: ;
          }
          snake_->SetDirection(direction);
        }
      }
    };
    std::thread keyboard_listen;
    if (!ai_) keyboard_listen = std::thread(kb_listen);
    */

    while (!game_over_) {

      QThread::msleep(kRefreshSpeed);
      if (user_quit_) {
          qDebug() << "log: user quit.\n";
          break;
      }
      bool eaten = false, suicided = false;
      snake_->MoveAndEat(food, &eaten, &suicided);
      if (suicided) {
          game_over_ = true;
          qDebug() << "log: suicided.\n";
      } else {
          game_over_ = UpdatePosition(); // killed by wall.
      }

      if (eaten) {
          qDebug() << "log: food eaten. update score and create new food.\n";
          UpdateScore();
          CreateFood();
      }
      NextState();
      //notifyObservers(); // tell gui to draw
    }
    GameOver();
    //if (!ai_) keyboard_listen.join();
}

void QGame::UpdateScore() {
    score_ += 1;
}
  
void QGame::CreateFood() {
    for (bool cell_ok = false; !cell_ok; ) {
      food = {rand()%(kColumns - 1) + 1, rand()%(kRows - 1) + 1};
      cell_ok = !snake_->Contains(food);
    }
    printf("Creating a new food at (%d, %d)\n", food.x, food.y);
}
