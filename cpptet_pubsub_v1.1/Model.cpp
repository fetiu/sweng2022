#include "Model.h"

#define BOARD_HEIGHT 20
#define BOARD_WIDTH 15

#define MAX_BLK_TYPES 7
#define MAX_BLK_DEGREES 4

int T0D0[] = { 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, -1 };//I
int T0D1[] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, -1 };
int T0D2[] = { 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, -1 };
int T0D3[] = { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, -1 };

int T1D0[] = { 1, 0, 0, 1, 1, 1, 0, 0, 0, -1 };//J
int T1D1[] = { 0, 1, 1, 0, 1, 0, 0, 1, 0, -1 };
int T1D2[] = { 0, 0, 0, 1, 1, 1, 0, 0, 1, -1 };
int T1D3[] = { 0, 1, 0, 0, 1, 0, 1, 1, 0, -1 };

int T2D0[] = { 0, 0, 1, 1, 1, 1, 0, 0, 0, -1 };//L
int T2D1[] = { 0, 1, 0, 0, 1, 0, 0, 1, 1, -1 };
int T2D2[] = { 0, 0, 0, 1, 1, 1, 1, 0, 0, -1 };
int T2D3[] = { 1, 1, 0, 0, 1, 0, 0, 1, 0, -1 };

int T3D0[] = { 1, 1, 1, 1, -1 };//O
int T3D1[] = { 1, 1, 1, 1, -1 };
int T3D2[] = { 1, 1, 1, 1, -1 };
int T3D3[] = { 1, 1, 1, 1, -1 };

int T4D0[] = { 0, 1, 1, 1, 1, 0, 0, 0, 0, -1 };//S
int T4D1[] = { 0, 1, 0, 0, 1, 1, 0, 0, 1, -1 };
int T4D2[] = { 0, 0, 0, 0, 1, 1, 1, 1, 0, -1 };
int T4D3[] = { 1, 0, 0, 1, 1, 0, 0, 1, 0, -1 };

int T5D0[] = { 0, 1, 0, 1, 1, 1, 0, 0, 0, -1 };//T
int T5D1[] = { 0, 1, 0, 0, 1, 1, 0, 1, 0, -1 };
int T5D2[] = { 0, 0, 0, 1, 1, 1, 0, 1, 0, -1 };
int T5D3[] = { 0, 1, 0, 1, 1, 0, 0, 1, 0, -1 };

int T6D0[] = { 1, 1, 0, 0, 1, 1, 0, 0, 0, -1 };//Z
int T6D1[] = { 0, 0, 1, 0, 1, 1, 0, 1, 0, -1 };
int T6D2[] = { 0, 0, 0, 1, 1, 0, 0, 1, 1, -1 };
int T6D3[] = { 0, 1, 0, 1, 1, 0, 1, 0, 0, -1 };
 
int *setOfCBlockArrays[] = {
  T0D0, T0D1, T0D2, T0D3,
  T1D0, T1D1, T1D2, T1D3,
  T2D0, T2D1, T2D2, T2D3,
  T3D0, T3D1, T3D2, T3D3,
  T4D0, T4D1, T4D2, T4D3,
  T5D0, T5D1, T5D2, T5D3,
  T6D0, T6D1, T6D2, T6D3,
};

extern void shutdown_whole_graph();

Model::Model(Window *w, string n): omsg(MSG_MAT, 0, NULL)
{
  TetrisState state;
  char key;

  win = w;
  name = n;

  CTetris::init(setOfCBlockArrays, MAX_BLK_TYPES, MAX_BLK_DEGREES);
  board = new CTetris(BOARD_HEIGHT, BOARD_WIDTH);
  srand((unsigned int)time(NULL));
  key = (char)('0' + rand() % MAX_BLK_TYPES);

  state = board->accept(key);
  if (state == Finished) {
    win->printw("Model: The first tetris block causes the game to be terminated!!");
    shutdown_whole_graph();
  }
};

void Model::handle(Msg *msg)
{
  TetrisState state;
  char key;
  //win->printw(name + ".handle() called.\n");

  if (msg->what == MSG_KEY) {
    win->printw(name + ": key = " + msg->key + "\n");

    key = msg->key;
    if (key == 'q') 
      shutdown_whole_graph();

    state = board->accept(key);
    omsg.mat = board->oCScreen->clip(0, board->iScreenDw, BOARD_HEIGHT, board->iScreenDw+BOARD_WIDTH);
    notifySubs(&omsg);
    if (state == NewBlock) {
      key = (char)('0' + rand() % MAX_BLK_TYPES);
      state = board->accept(key);
      omsg.mat = board->oCScreen->clip(0, board->iScreenDw, BOARD_HEIGHT, board->iScreenDw+BOARD_WIDTH);
      notifySubs(&omsg);
      if (state == Finished) {
        win->printw("Model: Game over!!");
        shutdown_whole_graph();
      }
    }
    //omsg.mat = new Matrix(arrayScreen, 20, 15);
    //notifySubs(&omsg);
  }
}





 