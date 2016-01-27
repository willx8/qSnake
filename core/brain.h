#ifndef BRAIN_H
#define BRAIN_H
#include "core/snake_graph.h"
#include <queue>
#include <vector>

using std::vector;
using std::queue;

class Brain {
 public:
  Brain(int columns, int lines) : graph_(columns, lines) {
    prev_food_ = {-1, -1};
  }

  Directions Next(const vector<BodyPart> &body,
      Position head, Position food);

  void clear() {
    graph_.clear();
    prev_food_ = {-1, -1};
    next_.clear();
  }

 private:
  vector<Directions> next_;
  Graph graph_;
  Position prev_food_;
};

#endif // BRAIN_H

