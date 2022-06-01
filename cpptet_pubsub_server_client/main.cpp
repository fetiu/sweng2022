#include <unistd.h>
#include <string>
#include <thread>
#include <vector>
#include <iostream>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

#include "Window.h"
#include "View.h"
#include "Model.h"
#include "TimeCtrl.h"
#include "KbdCtrl.h"

#include "Record.h"
#include "Replay.h"

#include "SendCtrl.h"
#include "RecvCtrl.h"

using namespace std;

vector<Sub*> sub_list;
Msg msg_end(MSG_END, 0, NULL, NULL, "main");

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

bool isDescriptorReadable(int fd)
{
  fd_set rdfs;
  struct timeval tv;
  tv.tv_sec = 1;
  tv.tv_usec = 0;

  FD_ZERO(&rdfs);
  FD_SET(fd, &rdfs);
  if (select(fd+1, &rdfs, NULL, NULL, &tv) < 0)
    cout << "select error" << endl;

  return FD_ISSET(fd, &rdfs);
}

int listen_socket(int port)
{
  struct sockaddr_in addr_server = {};
  memset(&addr_server, 0, sizeof(addr_server));
  addr_server.sin_family = AF_INET;
  addr_server.sin_port = htons(port);
  addr_server.sin_addr.s_addr = htonl(INADDR_ANY);

  int sd = socket(AF_INET, SOCK_STREAM, 0);
  if (sd < 0) {
    cout << "socket error" << endl;
    exit(1);
  }

  if (bind(sd, (sockaddr*) &addr_server, sizeof(addr_server)) < 0) {
    cout << "bind error" << endl;
    close(sd);
    exit(1);
  }

  if (listen(sd, 5) < 0) {
    cout << "listen error" << endl;
    close(sd);
    exit(1);
  }

  return sd;
}

int connect_socket(int port, const char *ip)
{
  struct sockaddr_in addr_server = {};
  memset(&addr_server, 0, sizeof(addr_server));
  addr_server.sin_family = AF_INET; 
  addr_server.sin_addr.s_addr = inet_addr(ip); 
  addr_server.sin_port = htons(port); 

  int cd = socket(AF_INET, SOCK_STREAM, 0); 

  if(cd < 0) {
    cout << "socket error" << endl;
    exit(1);
  }

  if (connect(cd, (struct sockaddr*) &addr_server, sizeof(addr_server)) < 0) {
    cout << "connect error" << endl;
    close(cd);
    exit(1);
  }

  return cd;
}

int main(int argc, char *argv[])
{
  vector<thread*> task_list;
  thread *task;

  string str_server ="server";
  string str_client = "client";
  bool server_mode = false;
  bool client_mode = false;
  bool play_mode = true;

  // if (argc != 3) {
  //   cout << "usage: " << argv[0] << " [server/client] port" << endl;
  //   exit(1);
  // }

  if (str_server.compare(argv[1]) == 0) {
    server_mode = true;
    cout << "server mode on!" << endl;
  }
  else if (str_client.compare(argv[1]) == 0) {
    client_mode = true;
    cout << "client mode on!" << endl;
  }
  else {
    cout << "usage: " << argv[0] << " client port ipaddr" << endl;
    cout << "usage: " << argv[0] << " server port" << endl;
    exit(1);
  }

  init_screen();

  // newwin(nlines, ncolumns, begin_y, begin_x)
  Window bttm_win(newwin(8, 50, 21, 0));
  Window left_win(newwin(20, 30, 0, 0));
  Window rght_win(newwin(20, 30, 0, 40));
  //bttm_win.printw("This is the bottom window.\n");

  if (server_mode) {
    struct sockaddr_in addr_client = {};
    socklen_t addr_client_len = sizeof(addr_client_len);
    int sd = listen_socket(atoi(argv[2]));

    int cd = accept(sd, (sockaddr*) &addr_client, &addr_client_len);
    if(cd < 0){
        cout << "accept error" << endl;
        exit(1);
    }

    SendCtrl *send_ctrl = new SendCtrl(&bttm_win, "SendCtrl", cd);
    sub_list.push_back(send_ctrl);
    RecvCtrl *recv_ctrl = new RecvCtrl(&bttm_win, "RecvCtrl", cd);
    sub_list.push_back(recv_ctrl);

    recv_ctrl->addSubs(send_ctrl);

    task = new thread(&SendCtrl::run, send_ctrl);
    task_list.push_back(task);
    task = new thread(&RecvCtrl::run, recv_ctrl);
    task_list.push_back(task);
  
    // wait for each task to be terminated
    for (int i=0; i < task_list.size(); i++)
      task_list[i]->join();
    
    close(cd);
    close(sd);
  }
  else if (client_mode) {

    int cd = connect_socket(atoi(argv[2]), argv[3]);

    string model_keys = "adsw s";
    //string right_model_keys = "jlki\rs";

    // create tasks to compose a graph
    View *left_view = new View(&left_win, &bttm_win, "LeftView");
    sub_list.push_back(left_view);
    View *right_view = new View(&rght_win, &bttm_win, "RightView");
    sub_list.push_back(right_view);
    Model *left_model = new Model(&bttm_win, "LeftModel", play_mode, !play_mode, model_keys);
    sub_list.push_back(left_model);
    Model *right_model = new Model(&bttm_win, "RightModel", !play_mode, play_mode, model_keys);
    sub_list.push_back(right_model);
    TimeCtrl *time_ctrl = new TimeCtrl(&bttm_win, "TimeCtrl");
    sub_list.push_back(time_ctrl);
    KbdCtrl *kbd_ctrl = new KbdCtrl(&bttm_win, "KbdCtrl");
    sub_list.push_back(kbd_ctrl);
    SendCtrl *send_ctrl = new SendCtrl(&bttm_win, "SendCtrl", cd);
    sub_list.push_back(send_ctrl);
    RecvCtrl *recv_ctrl = new RecvCtrl(&bttm_win, "RecvCtrl", cd);
    sub_list.push_back(recv_ctrl);

    // connect tasks to compose the graph
    left_model->addSubs(left_view);
    left_model->addSubs(send_ctrl);
    //left_model->addSubs(right_model);
    time_ctrl->addSubs(left_model);
    kbd_ctrl->addSubs(left_model);

    right_model->addSubs(right_view);
    //right_model->addSubs(left_model);
    recv_ctrl->addSubs(right_model);

    // run a thread for each task
    task = new thread(&View::run, left_view);
    task_list.push_back(task);
    task = new thread(&Model::run, left_model);
    task_list.push_back(task);
    task = new thread(&View::run, right_view);
    task_list.push_back(task);
    task = new thread(&Model::run, right_model);
    task_list.push_back(task);
    task = new thread(&TimeCtrl::run, time_ctrl);
    task_list.push_back(task);
    task = new thread(&KbdCtrl::run, kbd_ctrl);
    task_list.push_back(task);
    task = new thread(&SendCtrl::run, send_ctrl);
    task_list.push_back(task);
    task = new thread(&RecvCtrl::run, recv_ctrl);
    task_list.push_back(task);

    // wait for each task to be terminated
    for (int i=0; i < task_list.size(); i++)
      task_list[i]->join();
    
    close(cd);
  }

  close_screen();
  return 0;
}
