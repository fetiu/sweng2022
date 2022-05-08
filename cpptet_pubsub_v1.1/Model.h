#pragma once

#include "MsgPubSub.h"
#include "Window.h"
#include "CTetris.h"

class Model: public Sub, public Pub {
 private:
  Window *win; // console window
  Msg omsg; // msg with a matrix
  CTetris *board;  // tetris game board

 public:
  Model(Window *w, string n);
  void handle(Msg *msg);
};
