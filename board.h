class Board{
private:
  uint64_t black;
  uint64_t white;
  bool player;
  int turn;
public:
  Board();
  int stoneCount(uint64_t b);
  uint64_t getBoard(bool p);
  int isStone(uint64_t color, int y, int x);
  void printBoard();
  void debugBoard(uint64_t b);
  uint64_t checkLegal(bool p);
  uint64_t transferBoard(uint64_t pos, int i);
  void reverseStone(bool p, uint64_t pos);
  int checkPass(bool p);
  int checkFinish();
  void finishProcess();
};