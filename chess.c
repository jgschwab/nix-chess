#include <stdio.h>
#include <stdlib.h>

#define BLACK 0
#define WHITE 1


int main(){
    short board[8][8];
    void paintBoard();
    int move(char moveCmd[], int pieces[8][8][2], short color);
    char *(tiles[2]) = {"\e[100m", "\e[107m"};
    char *(pieceIcons[7][2]) = { // piece, pieceColor
        {"  ", "  "}, // empty
        {"\e[30m\u265F ", "\e[97m\u265F "}, // pawn
        {"\e[30m\u265D ", "\e[97m\u265D "}, // bishop
        {"\e[30m\u265E ", "\e[97m\u265E "}, // knight
        {"\e[30m\u265C ", "\e[97m\u265C "}, // rook
        {"\e[30m\u265B ", "\e[97m\u265B "}, // queen
        {"\e[30m\u265A ", "\e[97m\u265A "}, // king
    };
    
    int i, j;
    short color = WHITE;
    // initialize board tiles
    for(i = 0; i < 8; i++){
        for(j = 0; j < 8; j++){
            //board[i][j] = color ? "\e[7m  \e[27m" : "  ";
            board[i][j] = color;
            color = j == 7 ? color : color == WHITE ? BLACK : WHITE;
        }
    }
    
    /* initialize board pieces
    0 - empty
    1 - pawn
    2 - bishop
    3 - knight
    4 - rook
    5 - queen
    6 - king
    */
    int pieces[8][8][2] = {
        {{4,0},{3,0},{2,0},{5,0},{6,0},{2,0},{3,0},{4,0}},
        {{1,0},{1,0},{1,0},{1,0},{1,0},{1,0},{1,0},{1,0}},
        {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
        {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
        {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
        {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
        {{1,1},{1,1},{1,1},{1,1},{1,1},{1,1},{1,1},{1,1}},
        {{4,1},{3,1},{2,1},{5,1},{6,1},{2,1},{3,1},{4,1}},
    };
    
    paintBoard(board, pieces, tiles, pieceIcons);
    
    char *cmd = "g1f3";
    move(cmd, pieces, 1);
    paintBoard(board, pieces, tiles, pieceIcons);
}

/* Determine, based on the type and color of piece on the square 
   and background color of square, how to paint each square */
void paintBoard(short board[8][8], int pieces[8][8][2], char *(tiles[]), char *(pieceIcons[7][2])){
    
    int i, j;
    
    /* Print the header */
    printf("\e[36m   \u2554");
    for(i = 0; i < 12; i++)
        printf("\u2550");
    printf("\u2557\n   \u2551 *NIX Chess \u2551\n   \u255a");  
    for(i = 0; i < 12; i++)
        printf("\u2550");
    printf("\u255d\e[0m\n\n");
    
    /* Print the board */
    for(i = 0; i < 8; i++){
        printf(" %d ", 8 - i);
        for(j = 0; j < 8; j++){
            printf("\e[2m%s%s\e[0m", tiles[board[i][j]], 
                pieceIcons[pieces[i][j][0]][pieces[i][j][1]]);
        }
        printf("\n");
    }
    printf("   A/B/C/D/E/F/G/H/\n");
}

char toUpper(char c){
    if(c > 90)
        c -= 32;
    if(c < 65 || c > 90)
        return 0;
    return c;
}

void getValidMoves(int validMoves[][2], int pieces[8][8][2], short color, int i, int j){
    validMoves[0][0] = 5;
    validMoves[0][1] = 7;
    
    validMoves[1][0] = 5;
    validMoves[1][1] = 5;
    
    validMoves[2][0] = -1;
}


/* returns -1 if move is invalid */
int move(char moveCmd[], int pieces[8][8][2], short color){
    /* parse out initial location */
    int j = toUpper(moveCmd[0]);
    if(j == 0)
        return -1;
    j -= 65;
    int i = atoi(&moveCmd[1]);
    if(i == 0)
        return -1;
    i = 8 - i;
    int startLoc[] = {i,j};
    
    /* parse out future location */
    int y = toUpper(moveCmd[2]);
    if(y == 0)
        return -1;
    y -= 65;
    int x = atoi(&moveCmd[3]);
    if(x == 0)
        return -1;
    x = 8 - x;
    int endLoc[] = {x,y};
    
    /* get valid moves, store them in buffer */
    int validMoves[15][2];
    getValidMoves(validMoves, pieces, color, i, j);
    int k = 0;
    while(validMoves[k][0] != -1){
        if(validMoves[k][0] == x && validMoves[k][1] == y){
            /* actually move the piece in memory */
            pieces[x][y][0] = pieces[i][j][0];
            pieces[x][y][1] = pieces[i][j][1];
            pieces[i][j][0] = 0;
            pieces[i][j][1] = 0;
            return 0;
        }
        k++;
    }
    return -1;
    
}




