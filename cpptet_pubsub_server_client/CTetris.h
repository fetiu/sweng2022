#pragma once
#include "Tetris.h"

class CTetris : public Tetris {
 public:
  CTetris(int dy, int dx);
  static void init(int *setOfBlockArrays[], int blkTypes, int blkDegrees);
  TetrisState accept(char key);
  TetrisState accept(Matrix *deletedCLines);
  ~CTetris();

  Matrix* oCScreen;
  Matrix* oDeletedCLines;
  Matrix currCBlk;
  
 private:
  void deleteFullCLines();
  Matrix* iCScreen;
  static Matrix** setOfCBlockObjects;

};
