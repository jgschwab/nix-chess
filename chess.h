void paintBoard(int color, short board[8][8], int pieces[8][8][2], char *(tiles[]), char *(pieceIcons[7][2]));
char toUpper(char c);
int selfCheck(int pieces[8][8][2], int i, int j, int x, int y);
void getValidMoves(int validMoves[][2], int pieces[8][8][2], int i, int j);
int inValidMoves(int validMoves[][2], int i, int j);
void addToValidMoves(int validMoves[][2], int i, int j);
int checkForCheckmate(int validMoves[][2], int pieces[8][8][2], int x, int y);
int move(char moveCmd[], int pieces[8][8][2], short color);
