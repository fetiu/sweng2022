#pragma once

#include <unistd.h>

#include "MsgPubSub.h"
#include "Window.h"
#include <arpa/inet.h>
#include <sys/socket.h>

class SendCtrl: public Sub, public Pub {
 private:
  Window *win; // console window
  int sd; // socket descriptor
  
 public:
  SendCtrl(Window *w, string n, int d);
  void handle(Msg *msg);
  void console_print(string s);
};