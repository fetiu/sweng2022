#include "Record.h"

Record::Record(Window *w, string n): fout("record.txt")
{
  win = w;
  name = n;
}

void Record::handle(Msg *msg)
{
  //win->printw(name + ".handle() called.\n");

  if ((msg->what & MSG_KEY) != MSG_KEY)
    return;

  char key = msg->key;
  win->printw(name + ": key = " + key + "\n");
  fout << key << endl;
}


