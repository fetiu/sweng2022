#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

#include "Matrix.h"

#define MAX_SUBS 100
#define MSG_END (-1)
#define MSG_KEY  (1)
#define MSG_MAT (2)

class Msg {
 public:
  Msg(int w, char k, Matrix *m) { what = w; key = k; mat = m; }
  int what; // 1 (MSG_KEY), 2 (MSG_MAT)
  char key;
  Matrix *mat;
};

class Sub {
 protected:
  string name;           // object name 
  queue<Msg*> que;       // input message queue
  mutex mtx;             // mutex to protect 'que'
  condition_variable cv; // to wake up the thread of 'run()'

 public:
  void update(Msg *m);
  void run();
  virtual void handle(Msg *m) { }
};

class Pub {
 protected:
  Sub* subs[MAX_SUBS];
  int nsubs = 0;
  
 public:
  void addSubs(Sub* sub);
  void notifySubs(Msg *m);
};

