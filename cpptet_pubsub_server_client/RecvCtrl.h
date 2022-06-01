#pragma once

#include <unistd.h>

#include "MsgPubSub.h"
#include "Window.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

class RecvCtrl: public Sub, public Pub {
 private:
  Window *win; // console window
  Msg key_msg; // msg to publish
  int sd; // socket descriptor
  
 public:
  RecvCtrl(Window *w, string n, int d);
  void run();
  void handle(Msg *msg);
};