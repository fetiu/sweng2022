#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <fstream>

#include "MsgPubSub.h"
#include "Window.h"
#include "Matrix.h"

class Record: public Sub {
 private:
  Window *win;     // console window
  ofstream fout;

 public:
  Record(Window *w, string n);
  void handle(Msg *msg);
  void console_print(string s);
};
