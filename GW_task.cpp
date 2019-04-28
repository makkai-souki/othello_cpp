#include <iostream>
#include <vector>
#include <string>
#include "board.h"

using namespace std;

uint64_t convertInput(){
  int offset = 1, x,y;
  uint64_t result = 1;
  cin >> y >> x;
  result = result << ((x-offset)*8);
  result = result << (y - offset);
  return result;
}

int main(int argc, char const *argv[])
{
  Board game;
  uint64_t position;
  bool player = false;
  game.printBoard();
  while(game.checkFinish() == 0){
    if(player){
      cout << "白(○)のターン" << endl;
    }else{
      cout << "黒(●)のターン" << endl;
    }
    if(game.checkPass(player) == 0){
      // cout << "if checkPass" << endl;
      lavel:
      game.debugBoard(game.checkLegal(player));
      position = convertInput();
      game.printBoard();
      if((game.checkLegal(player) & position) !=0){
        game.reverseStone(player, position);
      }else{
        cout << "invalid" << endl;
        goto lavel;
      }
    }else{
      cout << "pass" << endl;
    }
    game.printBoard();
    player = !player;
  }
  game.finishProcess();
  return 0;
}