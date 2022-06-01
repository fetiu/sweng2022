#pragma once

#include "MsgPubSub.h"
#include "Window.h"
#include "CTetris.h"


enum class ModelMode {
  PLAY,
  REPLAY
};


class Model: public Sub, public Pub {
 private:
  Window *win; // console window
  CTetris *board;  // tetris game board

  ModelMode mode;
  TetrisState state;
  char key_left;
  char key_right;
  char key_down;
  char key_rotate;
  char key_drop;
  char key_tdown;

 public:
  Model(Window *w, string n, bool play_mode, bool replay_mode, string keys);
  void handle(Msg *msg);
  void handle_newblock(void);
  void output_message(char key);
  void console_print(string s);
  //void printwMatrix(Matrix *m);
};
