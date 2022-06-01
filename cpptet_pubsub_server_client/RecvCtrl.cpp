#include <RecvCtrl.h>

RecvCtrl::RecvCtrl(Window *w, string n, int d): key_msg(MSG_KEY, 0, NULL, NULL, n)
{
    win = w;
    name = n;
    sd = d;
};

extern void shutdown_whole_graph();
extern bool isDescriptorReadable(int fd);

void RecvCtrl::handle(Msg *msg)
{
  char key = 0;

  if (isDescriptorReadable(sd)) {
    int n;
    if ((n = read(sd, &key, 1)) < 0) {
      win->printw("RecvCtrl: read error!!\n");
      shutdown_whole_graph();
    } 
    else if (n == 0) {
      win->printw("RecvCtrl: read EOF!!\n");
      shutdown_whole_graph();
    }
    else {
      string s(1, key);
      win->printw("RecvCtrl: read " + s + "\n");
      key_msg.key = key;
      notifySubs(&key_msg);

      if (key == 'q') {
        win->printw("RecvCtrl: read q!!\n");
        shutdown_whole_graph();
      }
    }
  }
}

void RecvCtrl::run() 
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

    delete msg;
  }
  //win->printw(name + ".run() ends.\n");
};