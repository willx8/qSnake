#include "snake.h"
#include "gui/qgameboard.h"
#include <ncurses.h>
#include <iostream>
#include <cassert>
#include <QDebug>

using std::vector;

void Snake::SetHead(Position position) {
  body_ = vector<BodyPart>(3);
  body_[0].position = position;
  body_[1].position = position + Position{-1, 0};
  body_[2].position = position + Position{-2, 0};
  prev_tail_ = body_[2].position;
  direction_ = kRight;
}

void Snake::MoveAndEat(Position food, bool *eaten, bool *suicided) {
  if (ai_) next_direction_ = brain_->Next(body_, head(), food);

  Position new_head = body_[0].position;
//  mutex_.lock();
  //qDebug() << "direction:" << (int) next_direction_;
  //direction_ = next_direction_;
  qDebug() << "request direction";
  Directions tmp = requestDirection();
  SetDirection(tmp);
  direction_ = next_direction_;
  qDebug() << "after request direction";
  switch(direction_) {
    case kUp:    new_head.y -= 1; break;
    case kDown:  new_head.y += 1; break;
    case kLeft:  new_head.x -= 1; break;
    case kRight: new_head.x += 1; break;
    case kNoChange: assert(false);
  }
//  mutex_.unlock();

  BodyPart tail = body_.back();
  prev_tail_ = body_.back().position;
  for (int i = body_.size() - 1; i > 0 ; --i) {
    body_[i] =  body_[i-1];
    if (body_[i].position == new_head) {
        qDebug() << "suicide:" << i << " " << new_head.x << " " << new_head.y;
        *suicided = true;
    }
  }

  if (new_head == food) {
    body_.push_back(tail);
    *eaten = true;
  }
  body_[0].position = new_head;
}

void Snake::SetDirection(Directions direction) {
//  mutex_.lock();
  qDebug() << "setting direction." << (int) direction;
  // The opposite input direction is invalid.
  switch(direction_) {
    case kUp:    if (direction == kDown) direction = kNoChange;  break;
    case kDown:  if (direction == kUp) direction = kNoChange;    break;
    case kLeft:  if (direction == kRight) direction = kNoChange; break;
    case kRight: if (direction == kLeft) direction = kNoChange;  break;
    default:;
  }
  // Write directly to direction_ will cause error, e.g.
  // inverse direction_, if direction_ first set to a legal 
  // one then the inverse. next_direction_ serves like a cache. 
  if (direction != kNoChange) next_direction_ = direction;
//  mutex_.unlock();
}

Directions Snake::requestDirection() {
    return getter_->dir();
}

