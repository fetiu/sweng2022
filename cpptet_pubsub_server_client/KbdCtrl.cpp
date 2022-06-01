#include "KbdCtrl.h"

KbdCtrl::KbdCtrl(Window *w, string n): omsg(MSG_KEY, 0, NULL, NULL, n)
{
  win = w;
  name = n;
}

extern bool isDescriptorReadable(int fd);
char KbdCtrl::getChar() 
{
  extern int tty_cbreak(int fd);
  extern int tty_reset(int fd);
  char ch = -1;
  int n;
  
  //while (true) {
  tty_cbreak(0);
  if (isDescriptorReadable(STDIN_FILENO)) {
    if ((n = read(0, &ch, 1)) < 0)
      win->printw(name + ": read error.\n");
    else if (n == 0)
      win->printw(name + ": read 0 bytes.\n");
    else { // if (n > 0)
      //tty_reset(0);
      //break;
    }
  }
  tty_reset(0);
  //}

  return ch;
}

//Msg key_msg(MSG_KEY, 's', NULL);

void KbdCtrl::handle(Msg *msg)
{
  //win->printw(name + ".handle() called.\n");
  char key = getChar();
  if (key >= 0) {
    omsg.key = key;
    notifySubs(&omsg);
  }
}

void KbdCtrl::run() 
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

    if (msg && msg->what == MSG_END) {
      win->printw(name + ".run() terminated.\n");
      break;
    }

    handle(msg);
  }
  //win->printw(name + ".run() ends.\n");
}
