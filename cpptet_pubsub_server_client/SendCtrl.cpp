#include <SendCtrl.h>

SendCtrl::SendCtrl(Window *w, string n, int d)
{
    win = w;
    name = n;
    sd = d;
};

void SendCtrl::console_print(string s)
{
    win->printw(s);
}

extern void shutdown_whole_graph();

void SendCtrl::handle(Msg *msg)
{
  if ((msg->what & MSG_KEY) != MSG_KEY)
    return;

  char key = msg->key;

  int n;
  if ((n = write(sd, &key, 1)) < 0) {
    win->printw("SendCtrl: write error!!\n");
    shutdown_whole_graph();
  }
  string s(1, key);
  win->printw("SendCtrl: write " + s + "\n");
}