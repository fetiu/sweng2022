#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

#include "Matrix.h"

#define MAX_SUBS 100
#define MSG_END (-1)
#define MSG_KEY  (1)
#define MSG_MAT (2)
#define MSG_MAT2 (4)
#define MSG_KEY_MAT (MSG_KEY | MSG_MAT)
#define MSG_KEY_MAT_MAT2 (MSG_KEY_MAT | MSG_MAT2)

enum class MsgPubId {
  UNKNOWN,
  KBD_CTRL,
  TIME_CTRL,
  MODEL
};

class Msg {
 public:
  Msg(int w, char k, Matrix *m, Matrix *m2) {
    from = MsgPubId::UNKNOWN;
    what = w; key = k;
    mat = m; mat2 = m2;
  }
#if 1 // added by khkim
  Msg(Msg *msg) { // copy constructor
    from = msg->from;
    what = msg->what;
    key = msg->key;
    if (msg->mat != NULL)
      mat = new Matrix(msg->mat);  // deep copy
    else
      mat = NULL;

    if (msg->mat2 != NULL)
      mat2 = new Matrix(msg->mat2);  // deep copy
    else
      mat2 = NULL;
  } 
  ~Msg() {  // destructor
    if (mat != NULL) 
      delete mat; 
    if (mat2 != NULL) 
      delete mat2; 
  }
#endif
  MsgPubId from;
  int what; // 1 (MSG_KEY), 2 (MSG_MAT)
  char key;
  Matrix *mat; // for oCScreen
  Matrix *mat2; // for oDeletedCLines
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

