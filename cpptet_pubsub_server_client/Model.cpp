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
Window *g_win = NULL;

Model::Model(Window *w, string n, bool play_mode, bool replay_mode, string keys) //: omsg(MSG_MAT, 0, NULL, NULL)
{
  win = w;
  name = n;
  if (play_mode ^ replay_mode == 0) {
    win->printw("Model: either PLAY or REPLAY can be specified!!");
    shutdown_whole_graph();
  }

  if (play_mode)
    mode = ModelMode::PLAY;
  else if (replay_mode)
    mode = ModelMode::REPLAY;

  CTetris::init(setOfCBlockArrays, MAX_BLK_TYPES, MAX_BLK_DEGREES);
  board = new CTetris(BOARD_HEIGHT, BOARD_WIDTH);
  state = TetrisState::NewBlock;
  if (mode == ModelMode::PLAY)
    srand((unsigned int)time(NULL));

  if (keys.length() != 6) {
    win->printw("Model: the number of keys is not 6!!");
    shutdown_whole_graph();
  }

  const char *p = keys.c_str();
  key_left = p[0];
  key_right = p[1];
  key_down = p[2];
  key_rotate = p[3];
  key_drop = p[4];
  key_tdown = p[5];

  g_win = win;
};

void printwMatrix(Matrix *m) {
  int **array = m->get_array();
  for (int y = 0; y < m->get_dy(); y++) {
    for (int x = 0; x < m->get_dx(); x++)
      g_win->printw(to_string(array[y][x]));
    g_win->printw("\n");
  }
}

void Model::output_message(char key)
{
    Msg omsg(0, 0, NULL, NULL, name);
    omsg.what = MSG_KEY_MAT_MAT2;
    omsg.mat = board->oCScreen->clip(0, board->iScreenDw, BOARD_HEIGHT, board->iScreenDw+BOARD_WIDTH);
    //win->printw("Model: state = " + to_string(state)  + "\n");
    if (state == TetrisState::RunningWDeletion) {
      omsg.mat2 = new Matrix(board->oDeletedCLines);
      //printwMatrix(board->oDeletedCLines);
      //board->accept(board->oDeletedCLines);
    }
    else
      omsg.mat2 = NULL;
    omsg.key = key;
    notifySubs(&omsg);
}

void Model::handle_newblock(void)
{
    char key = (char)('0' + rand() % MAX_BLK_TYPES);
    state = board->accept(key);
    output_message(key);
    if (state == TetrisState::Finished) {
      win->printw("Model: Game over!!\n");
      shutdown_whole_graph();
    }
}

void Model::console_print(string s) 
{
  win->printw(s);
}

void Model::handle(Msg *msg)
{
  if ((msg->what & MSG_KEY) != MSG_KEY)
    return;

  char key = msg->key;
  if (key == 'q') { 
    output_message(key);
    shutdown_whole_graph();
  }

  // if (key == 's' && key != key_down && msg->name == "kbd_ctrl")
  //   return; // accept 's' from kbd_ctrl only for left_model

  char key2;
  if (key == key_left) key2 = 'a';
  else if (key == key_right) key2 = 'd';
  else if (key == key_down) key2 = 's';
  else if (key == key_rotate) key2 = 'w';
  else if (key == key_drop) key2 = ' ';
  else if (key == key_tdown) key2 = 's';
  else key2 = key;

  if (state == TetrisState::NewBlock && mode == ModelMode::PLAY)
    handle_newblock();

  //win->printw(name + ": key = " + key + "\n");
  state = board->accept(key2);
  output_message(key2);
  if (state == TetrisState::Finished && mode == ModelMode::REPLAY) {
    win->printw("Model: Game over!!\n");
    shutdown_whole_graph();
  }

  if (state == TetrisState::NewBlock && mode == ModelMode::PLAY)
    handle_newblock();

}

