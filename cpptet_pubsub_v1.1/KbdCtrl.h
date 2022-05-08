#pragma once

#include <unistd.h>

#include "MsgPubSub.h"
#include "Window.h"

class KbdCtrl: public Sub, public Pub {
 private:
  Window *win; // console window
  Msg omsg; // msg with a key
  bool isStdinReadable();
  char getChar(); 

 public:
  KbdCtrl(Window *w, string n);
  void run();
  void handle(Msg *msg);
};
