#include <iostream>
#include <string>
#include <vector>
#include "board.h"

// 1の時デバッグ用テストモード
// すぐに終わる
#define TESTMODE 1

// 合法手の判定に用いるマスク
#define H_MASK 0x7e7e7e7e7e7e7e7e
#define V_MASK 0x00FFFFFFFFFFFF00
#define A_MASK 0x007e7e7e7e7e7e00
// 石の反転に用いるマスク
#define RE_MASK_0 0xffffffffffff00
#define RE_MASK_1 0x7f7f7f7f7f7f00
#define RE_MASK_2 0x7f7f7f7f7f7f7f
#define RE_MASK_3 0x007f7f7f7f7f7f
#define RE_MASK_4 0x00ffffffffffff
#define RE_MASK_5 0x00fefefefefefe
#define RE_MASK_6 0xfefefefefefefe
#define RE_MASK_7 0xfefefefefefe00

using namespace std;

Board::Board(){
  if(TESTMODE){
    black = 0x061e02503c002c00;
    white = 0xf9e1fd2f43ff133f;
  }else{
    black = 0x0000001008000000;
    white = 0x0000000810000000;
  }
    
  // false:黒、true:白手番
  player = false;
  turn = 0;
}

uint64_t Board::getBoard(bool p){
  return p? white: black;
}

int Board::stoneCount(uint64_t b){
  b = (b & 0x5555555555555555) + (b >> 1 & 0x5555555555555555);
  b = (b & 0x3333333333333333) + (b >> 2 & 0x3333333333333333);
  b = (b & 0x0f0f0f0f0f0f0f0f) + (b >> 4 & 0x0f0f0f0f0f0f0f0f);
  b = (b & 0x00ff00ff00ff00ff) + (b >> 8 & 0x00ff00ff00ff00ff);
  b = (b & 0x0000ffff0000ffff) + (b >> 16 & 0x0000ffff0000ffff);
  return (b & 0x00000000ffffffff) + (b >> 32);
}

// x,yに石が存在するか
int Board::isStone(uint64_t color, int y, int x){
  return (color>>(8 * y + x)  & 1ull);
}

void Board::printBoard(){
  for (int i = 0; i < 8; i++){
    for (int j = 0; j < 8; j++){
      if (this->isStone(black, i, j) != 0){
        cout << "● ";
      }else if (this->isStone(white, i, j) != 0){
        cout << "○ ";
      }else{
        cout << "× ";
      }
    }
    cout << endl;
  }
  cout << endl;
}

void Board::debugBoard(uint64_t b){
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if (this->isStone(b, i,j))
      {
        cout << "○ ";
      }else{
        cout << "× ";
      }
    }
    cout << endl;
  }
  cout <<  endl;
}

uint64_t Board::checkLegal(bool p){
  uint64_t offence = p? white : black;
  uint64_t diffence = p? black : white;  
  uint64_t blank = ~(black | white);
  uint64_t h_watcher = diffence & H_MASK;
  uint64_t v_watcher = diffence & V_MASK;
  uint64_t a_watcher = diffence & A_MASK;
  uint64_t tmp = 0, legal_board = 0;

  // 左右
  tmp = h_watcher & (offence << 1);
  tmp |= h_watcher & (tmp << 1);
  tmp |= h_watcher & (tmp << 1);
  tmp |= h_watcher & (tmp << 1);
  tmp |= h_watcher & (tmp << 1);
  tmp |= h_watcher & (tmp << 1);
  legal_board = blank & (tmp << 1);
  tmp = h_watcher & (offence >> 1);
  tmp |= h_watcher & (tmp >> 1);
  tmp |= h_watcher & (tmp >> 1);
  tmp |= h_watcher & (tmp >> 1);
  tmp |= h_watcher & (tmp >> 1);
  tmp |= h_watcher & (tmp >> 1);
  legal_board |= blank & (tmp >> 1);
  //  上下
  tmp = v_watcher & (offence << 8);
  tmp |= v_watcher & (tmp << 8);
  tmp |= v_watcher & (tmp << 8);
  tmp |= v_watcher & (tmp << 8);
  tmp |= v_watcher & (tmp << 8);
  tmp |= v_watcher & (tmp << 8);
  legal_board |= blank & (tmp << 8);
  tmp = v_watcher & (offence >> 8);
  tmp |= v_watcher & (tmp >> 8);
  tmp |= v_watcher & (tmp >> 8);
  tmp |= v_watcher & (tmp >> 8);
  tmp |= v_watcher & (tmp >> 8);
  tmp |= v_watcher & (tmp >> 8);
  legal_board |= blank & (tmp >> 8);
  //  斜め
  tmp = a_watcher & (offence >> 9);
  tmp |= a_watcher & (tmp >> 9);
  tmp |= a_watcher & (tmp >> 9);
  tmp |= a_watcher & (tmp >> 9);
  tmp |= a_watcher & (tmp >> 9);
  tmp |= a_watcher & (tmp >> 9);
  legal_board |= blank & (tmp >> 9);
  tmp = a_watcher & (offence << 9);
  tmp |= a_watcher & (tmp << 9);
  tmp |= a_watcher & (tmp << 9);
  tmp |= a_watcher & (tmp << 9);
  tmp |= a_watcher & (tmp << 9);
  tmp |= a_watcher & (tmp << 9);
  legal_board |= blank & (tmp << 9);
  tmp = a_watcher & (offence >> 7);
  tmp |= a_watcher & (tmp >> 7);
  tmp |= a_watcher & (tmp >> 7);
  tmp |= a_watcher & (tmp >> 7);
  tmp |= a_watcher & (tmp >> 7);
  tmp |= a_watcher & (tmp >> 7);
  legal_board |= blank & (tmp >> 7);
  tmp = a_watcher & (offence << 7);
  tmp |= a_watcher & (tmp << 7);
  tmp |= a_watcher & (tmp << 7);
  tmp |= a_watcher & (tmp << 7);
  tmp |= a_watcher & (tmp << 7);
  tmp |= a_watcher & (tmp << 7);
  legal_board |= blank & (tmp << 7);
  return legal_board;
}

uint64_t Board::transferBoard(uint64_t pos, int i){
  switch(i){
    case 0:
      return (pos << 8) & RE_MASK_0;
    case 1:
      return (pos << 7) & RE_MASK_1;
    case 2:
      return (pos >> 1) & RE_MASK_2;
    case 3:
      return (pos >> 9) & RE_MASK_3;
    case 4:
      return (pos >> 8) & RE_MASK_4;
    case 5:
      return (pos >> 7) & RE_MASK_5;
    case 6:
      return (pos << 1) & RE_MASK_6;
    case 7:
      return (pos << 9) & RE_MASK_7;
    default:
      return 0;
  }
}

void Board::reverseStone(bool p, uint64_t pos){
  uint64_t offence = p? white: black;
  uint64_t diffence = p? black: white;
  uint64_t mask = 0, rev_tmp = 0, rev = 0;
  for (int i = 0; i < 8; i++)
  {
    mask = this->transferBoard(pos, i);
    rev_tmp = 0;
    while((mask != 0) && (mask & diffence) != 0){
      rev_tmp |= mask;
      mask = this->transferBoard(mask,i);
      if((mask & offence) != 0)
      {
        rev |= rev_tmp;
      }
    }
  }
  if (p){
    white ^= pos | rev;
    black ^= rev;
  }else{
    black ^= pos | rev;
    white ^= rev;
  }
}

int Board::checkPass(bool p){
  if(this->checkLegal(p) == 0){
    return 1;
  }else{
    return 0;
  }
}

int Board::checkFinish(){
  if((this->checkLegal(0) == 0)&&(this->checkLegal(1) == 0)){
    return 1;
  }else{
    return 0;
  }
}

void Board::finishProcess(){
  int white_count = this->stoneCount(white);
  int black_count = this->stoneCount(black);
  cout << "黒の個数：　" << white_count << endl;
  cout << "白の個数：　" << black_count << endl;
  if(white_count == black_count){
    cout << "引き分け" << endl;
  }else if(white_count < black_count){
    cout << "黒の勝ち" << endl;
  }else{
    cout << "白の勝ち" << endl;
  }
}

