#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

#include "MsgPubSub.h"
#include "Window.h"
#include "Matrix.h"

class Record: public Sub {
 private:
  Window *win;     // console window
  
 public:
  Record(Window *w, string n);
  void handle(Msg *msg);
};
