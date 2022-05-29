#include <unistd.h>
#include <string>
#include <thread>
#include <vector>
#include <iostream>

#include "Window.h"
#include "View.h"
#include "Model.h"
#include "TimeCtrl.h"
#include "KbdCtrl.h"
#if 1
#include "Record.h"
#endif

using namespace std;

vector<Sub*> sub_list;
Msg msg_end(MSG_END, 0, NULL);

void init_screen() 
{
  setlocale(LC_ALL, ""); // for printing a box character
  initscr();         // initialize the curses screen
  start_color(); // start using colors
  // init_pair(index, fg color, bg color);
  init_pair(1, COLOR_RED,     COLOR_BLACK); 
  init_pair(2, COLOR_GREEN,   COLOR_BLACK);
  init_pair(3, COLOR_YELLOW,  COLOR_BLACK);
  init_pair(4, COLOR_BLUE,    COLOR_BLACK);
  init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(6, COLOR_CYAN,    COLOR_BLACK);
  init_pair(7, COLOR_WHITE,   COLOR_BLACK);
}

void close_screen() 
{
  endwin();
}

void shutdown_whole_graph()
{
  sleep(1);
   // send a self-terminating msg to each task
  for (int i=0; i < sub_list.size(); i++)
	  sub_list[i]->update(&msg_end);
}

int main(int argc, char *argv[]) 
{
  vector<thread*> task_list;
  thread *task;

#if 1 // added by khkim
  string str_record = "record";
  string str_replay = "replay";
  bool record_mode = false;
  bool replay_mode = false;

  if (argc != 2) {
    cout << "usage: " << argv[0] << " [record/replay]" << endl;
    exit(1);
  }

  if (str_record.compare(argv[1]) == 0) {
    record_mode = true;
    cout << "record mode on!" << endl;
  }
  else if (str_replay.compare(argv[1]) == 0) {
    replay_mode = true;
    cout << "replay mode on!" << endl;
  }
  else {
    cout << "usage: " << argv[0] << " [record/replay]" << endl;
    exit(1);
  }
#endif 

  init_screen();

  // newwin(nlines, ncolumns, begin_y, begin_x)
  Window bttm_win(newwin(8, 50, 21, 0));
  Window left_win(newwin(20, 30, 0, 0));
  Window rght_win(newwin(20, 30, 0, 40));
  //bttm_win.printw("This is the bottom window.\n");

  // create tasks to compose a graph
  View *left_view = new View(&left_win, &bttm_win, "left_view");
  sub_list.push_back(left_view);
#if 1 // added by khkim
  Record *left_record = new Record(&bttm_win, "left_record");
  sub_list.push_back(left_record);
  Model *left_model = new Model(&bttm_win, "left_model", record_mode, replay_mode);
#else
  Model *left_model = new Model(&bttm_win, "left_model");
#endif
  sub_list.push_back(left_model);
  TimeCtrl *time_ctrl = new TimeCtrl(&bttm_win, "time_ctrl");
  sub_list.push_back(time_ctrl);
  KbdCtrl *kbd_ctrl = new KbdCtrl(&bttm_win, "kbd_ctrl");
  sub_list.push_back(kbd_ctrl);

  // connect tasks to compose the graph
#if 1 // added by khkim
  left_model->addSubs(left_record);
#endif
  left_model->addSubs(left_view);
  time_ctrl->addSubs(left_model);
  kbd_ctrl->addSubs(left_model);

  // run a thread for each task
#if 1 // added by khkim
  task = new thread(&Record::run, left_record);
  task_list.push_back(task);
#endif
  task = new thread(&View::run, left_view);
  task_list.push_back(task);
  task = new thread(&Model::run, left_model);
  task_list.push_back(task);
  task = new thread(&TimeCtrl::run, time_ctrl);
  task_list.push_back(task);
  task = new thread(&KbdCtrl::run, kbd_ctrl);
  task_list.push_back(task);

  // message flow begins...
  //sleep(10);

  // message flow ends.
  //shutdown_whole_graph();

  // wait for each task to be terminated
  for (int i=0; i < task_list.size(); i++) 
	  task_list[i]->join(); 

  close_screen();

  return 0;
}
