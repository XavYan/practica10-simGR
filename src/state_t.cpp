#include "../include/state_t.hpp"

state_t::state_t (const unsigned id, const bool accept) : id_(id), accept_(accept) {}
state_t::state_t (const state_t& state) : id_(state.id()), accept_(state.is_accept()), next_(state.getNext()) {}
state_t::~state_t (void) {}

const unsigned state_t::id (void) const {
  return id_;
}

set<pair<char, unsigned> > state_t::getNext (void) const {
  return next_;
}

const bool state_t::is_accept (void) const {
  return accept_;
}

unsigned state_t::find_by_letter (const char c) {
  if (next_.empty()) return -1;
  for (set<pair<char,unsigned> >::iterator it = next_.begin(); it != next_.end(); it++) {
    if (get<0>(*it) == c) { return get<1>(*it); }
  }
  return -1;
}

void state_t::insert_pair (const char symbol, const unsigned id) {
  pair<char,unsigned> new_pair;
  get<0>(new_pair) = symbol;
  get<1>(new_pair) = id;
  next_.insert(new_pair);
}

void state_t::new_id (const unsigned& id) {
  id_ = id;
}

ostream& state_t::dbg_write (void) const {
  cout << "\t---------- ESTADO " << id_ << " -----------\n";
  cout << "\tEs un nodo de aceptacion?: " << accept_ << "\n";
  cout << "\tEstados conectados:\n";
  for (set<pair<char,unsigned>>::iterator it = next_.begin(); it != next_.end(); it++) {
    cout << "\tLetra: " << get<0>(*it);
    cout << "\tID: " << get<1>(*it) << "\n";
  }
  cout << "\t-------------------------------\n";
  return cout;
}

const bool state_t::operator< (const state_t state) const {
  return (id() < state.id());
}
