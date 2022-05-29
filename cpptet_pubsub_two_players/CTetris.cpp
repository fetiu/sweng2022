#include "CTetris.h"

Matrix** CTetris::setOfCBlockObjects = nullptr;

CTetris::CTetris(int dy, int dx) : Tetris::Tetris(dy, dx) {
  iCScreen = new Matrix(oScreen);
  iCScreen->mulc(nBlock.type + 1);
  oCScreen = new Matrix(iCScreen);
}

void CTetris::init(int *setOfBlockArrays[], int blkTypes, int blkDegrees) {
  Tetris::init(setOfBlockArrays, blkTypes, blkDegrees);
  setOfCBlockObjects = new Matrix*[nBlock.type];
  for(int i = 0; i < nBlock.type; ++i) {
    setOfCBlockObjects[i] = new Matrix[nBlock.degree];
  }

  for(int i = 0; i < nBlock.type; ++i) { 
    for(int j = 0; j < nBlock.degree; ++j) {
      setOfCBlockObjects[i][j] = Matrix(setOfBlockObjects[i][j]);
      setOfCBlockObjects[i][j].mulc(i + 1);
    }
  }
}

TetrisState CTetris::accept(Matrix *deletedCLines) {
  int old_y = iScreenD.y;
  TetrisState s = Tetris::accept(deletedCLines->binary());

  int dy = deletedCLines->get_dy();
  Matrix *tempCScreen = iCScreen->clip(dy, iScreenDw, old_y, iScreenDw + iScreenD.x);
  iCScreen->paste(tempCScreen, 0, iScreenDw);
  iCScreen->paste(deletedCLines, old_y - dy, iScreenDw);

  int currBlkStateBottom = currBlkState.top + currCBlk.get_dy();
  int currBlkStateRight = currBlkState.left + currCBlk.get_dx();
  Matrix *tempCBlk = iCScreen->clip(currBlkState.top, currBlkState.left, currBlkStateBottom, currBlkStateRight);
  tempCBlk = tempCBlk->add(&currCBlk);

  oCScreen->paste(iCScreen, 0, 0);
  oCScreen->paste(tempCBlk, currBlkState.top, currBlkState.left);
  return s;
}

TetrisState CTetris::accept(char key) {
  TetrisState s = Tetris::accept(key);
  if (key >= '0' && key <= ('0' + nBlock.type - 1)) {
    if (justStarted == false) {
      deleteFullCLines();
    }
    iCScreen->paste(oCScreen, 0, 0);
  }

  currCBlk = setOfCBlockObjects[currBlkState.shape.type][currBlkState.shape.degree];
  int currBlkStateBottom = currBlkState.top + currCBlk.get_dy();
  int currBlkStateRight = currBlkState.left + currCBlk.get_dx();
  Matrix tempCBlk = iCScreen->clip(currBlkState.top, currBlkState.left, currBlkStateBottom, currBlkStateRight);  
  tempCBlk = tempCBlk.add(&currCBlk);

  oCScreen->paste(iCScreen, 0, 0);
  oCScreen->paste(&tempCBlk, currBlkState.top, currBlkState.left);
  return s;
}

void CTetris::deleteFullCLines() {
  //TetrisState s = Tetris::deleteFullLines();
  iCScreen->paste(oCScreen, 0, 0);

  //extern void printwMatrix(Matrix *m);  
  vector<Matrix *> deletedCLines;
  deletedCLines.clear();
  Matrix *zeroBlk = new Matrix(1, iScreenD.x);
  for(const auto& line : deletedLineNums) {
    Matrix *tempCBlk = iCScreen->clip(0, iScreenDw, line, iScreenDw + iScreenD.x);
    Matrix *deletedCLine = iCScreen->clip(line, iScreenDw, line+1, iScreenDw + iScreenD.x);
    //printwMatrix(deletedCLine);
    deletedCLines.push_back(deletedCLine);
    iCScreen->paste(tempCBlk, 1, iScreenDw);
    iCScreen->paste(zeroBlk, 0, iScreenDw);
  }

  if (oDeletedCLines != NULL)
    delete oDeletedCLines;

  if (deletedCLines.size() > 0) {
    oDeletedCLines = new Matrix(deletedCLines.size(), iScreenD.x);
    for(int i = 0; i < deletedCLines.size(); i++) {
      //printwMatrix(deletedCLines[i]);
      oDeletedCLines->paste(deletedCLines[i], i, 0);
    }
  }
  else {
    oDeletedCLines = NULL;    
  }

  oCScreen->paste(iCScreen, 0, 0);
  //return s;
}

CTetris::~CTetris() {
  for(int i = 0; i < nBlock.type; ++i) {
    delete [] setOfCBlockObjects[i];
  }
  delete [] setOfCBlockObjects;
  delete oCScreen;
  delete iCScreen;
}