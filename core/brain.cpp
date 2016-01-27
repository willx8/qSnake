#include "brain.h"

Directions Brain::Next(const vector<BodyPart>& body,
  Position head, Position food) {
  if (prev_food_ != food) {
    graph_.clear();
    for (auto &bp : body) {
      graph_[bp.position].block = true;
    }
    // New food. Got to recalculate the path to it!.
    next_ = graph_.find(head, food);
    prev_food_ = food;
  }
  auto re = next_.back();
  next_.pop_back();
  return re;
}
