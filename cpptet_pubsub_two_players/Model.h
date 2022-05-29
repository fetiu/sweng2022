#pragma once

#include "MsgPubSub.h"
#include "Window.h"
#include "CTetris.h"

#if 1 // added by khkim
enum class ModelMode {
  RECORD,
  REPLAY
};
#endif

class Model: public Sub, public Pub {
 private:
  Window *win; // console window
  //Msg omsg; // msg with a matrix
  CTetris *board;  // tetris game board
#if 1 // added by khkim
  ModelMode mode;
  TetrisState state;
#endif

 public:
#if 1 // added by khkim
  Model(Window *w, string n, bool record_mode, bool replay_mode);
#else
  Model(Window *w, string n);
#endif
  void handle(Msg *msg);
  void handle_newblock(void);
  void output_message(char key);
  //void printwMatrix(Matrix *m);
};
