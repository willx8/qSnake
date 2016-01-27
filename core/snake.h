#ifndef SNAKE_H
#define SNAKE_H
#include <vector>
#include <mutex>
#include <cassert>
#include <queue>
#include <memory>
#include <thread>
#include <chrono>
#include <QObject>
#include "core/brain.h"

using std::vector;
using std::mutex;

class QGameBoard;

class Snake : public QObject {
    Q_OBJECT
 public:
  Snake(QObject *parent = 0) : QObject(parent) { direction_ = kRight; }

  void SetBorder(int columns, int lines) {
    columns_ = columns;
    lines_ = lines;
  }

  void SetHead(Position position);

  Position head() const { return body_.front().position; }

  Position prev_tail() const { return prev_tail_; }

  bool Contains(Position p) {
    for (auto &bp : body_)
      if (bp.position == p) return true;
    return false;
  }

  void clear() {
    body_.clear();
    if (brain_) brain_->clear();
  }

  void MoveAndEat(Position food_, bool *eaten, bool *suicided);

  void Ai (bool ai) {
    if ((ai_ = ai) == true) {
      brain_ = std::unique_ptr<Brain>(new Brain(columns_, lines_));
    } else {
        brain_ = nullptr;
    }
  }

  void registerDirGetter(QGameBoard* getter) {
      getter_ = getter;
  }

  Directions requestDirection();

signals:

public slots:
  void SetDirection(Directions direction);

 private:
  vector<BodyPart> body_;
  Directions direction_;
  Directions next_direction_; // store keyboard input.
  Position prev_tail_;
  bool ai_;
  std::unique_ptr<Brain> brain_;
  mutex mutex_;
  int columns_;
  int lines_;
  QGameBoard* getter_;
};

#endif
