#include "Record.h"

Record::Record(Window *w, string n) 
{
  win = w;
  name = n;
};

void Record::handle(Msg *msg)
{
  //win->printw(name + ".handle() called.\n");

  if ((msg->what & MSG_KEY) != MSG_KEY) // added by khkim
    return;

  char key = msg->key;
  win->printw(name + ": key = " + key + "\n");
}


