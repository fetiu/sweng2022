#include "Replay.h"

Replay::Replay(Window *w, string n): fin("record.txt")
{
  win = w;
  name = n;
}

void Replay::handle(Msg *msg)
{
  //win->printw(name + ".handle() called.\n");
  usleep(100 * 1000);

  if (fin.eof()) {
    return;
  }
  char key;
  // add noskipws for space(block drop) key
  fin >> noskipws >> key;
  Msg omsg(MSG_KEY, key, NULL, NULL);
  notifySubs(&omsg); // omsg will be copied
}

void Replay::run()
{
  //win->printw(name + ".run() begins.\n");
  Msg *msg;
  while (true) {
    mtx.lock();
    if (que.empty())
      msg = NULL;
    else {
      msg = que.front();
      que.pop();
    }
    mtx.unlock();
    //win->printw(name + ".run() wakes up.\n");

    if (msg && msg->what == MSG_END)
      break;

    handle(msg);
    delete msg;
  }
  //win->printw(name + ".run() ends.\n");
}
