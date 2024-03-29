#include "View.h"

View::View(Window *w_main, Window *w_con, string n) 
{
  win = w_con;
  mwin = w_main;
  name = n;
};

void View::handle(Msg *msg)
{
  //win->printw(name + ".handle() called.\n");

  if ((msg->what & MSG_MAT) != MSG_MAT) // added by khkim
    return;

  Matrix *m = msg->mat;
  int dy = m->get_dy();
  int dx = m->get_dx();
  int **array = m->get_array();
  mwin->dowclear();

  for (int y = 0; y < dy; y++) {
    for (int x = 0; x < dx; x++) {
      if (array[y][x] == 0)
	      mwin->addStr(y, x, "□");
      else if (array[y][x] < 8)
	      mwin->addCstr(y, x, "■", array[y][x]);
      else 
	      mwin->addStr(y, x, "X");
    }
    cout << endl;
  }

  mwin->dowrefresh();
  win->dowrefresh(); // added by khkim
}


