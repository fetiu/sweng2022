#pragma once

#include <unistd.h>
#include <fstream>

#include "MsgPubSub.h"
#include "Window.h"

class Replay: public Sub, public Pub {
 private:
  Window *win; // console window
  ifstream fin;

 public:
  Replay(Window *w, string n, string path="record.txt");
  void run();
  void handle(Msg *msg);
};
