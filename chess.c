#include <stdio.h>
#include <stdlib.h>
#include "chess.h"

#define BLACK 0
#define WHITE 1


int main(){
    short board[8][8];
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
    
    paintBoard(1, board, pieces, tiles, pieceIcons);
    
    /* TESTING
    move("d2a5", pieces, 1);
    paintBoard(board, pieces, tiles, pieceIcons);
    move("b1c3", pieces, 1);
    paintBoard(board, pieces, tiles, pieceIcons);
    move("d1d3", pieces, 1);
    paintBoard(board, pieces, tiles, pieceIcons);
    move("b7b6", pieces, 0);
    paintBoard(board, pieces, tiles, pieceIcons);
    move("b6a5", pieces, 0);
    paintBoard(board, pieces, tiles, pieceIcons);
    move("a5a4", pieces, 0);
    paintBoard(board, pieces, tiles, pieceIcons);
    move("c3a4", pieces, 1);
    paintBoard(board, pieces, tiles, pieceIcons);
    */
    
    
    int turn = 1;
    int gameOver = 0;
    char players[2][6] = {"Black", "White"};
    char errorMsg[5][40] = {"No piece at start location",
                            "Piece not owned by player",
                            "End location not in piece's range",
                            "Invalid start location",
                            "Invalid end location"};
    char cmd[100];
    int code;
    while(!gameOver){
        printf("%s >> ", players[turn]);
        scanf("%s", cmd);
        if((code = move(cmd, pieces, turn)) > 0){
            printf("%s\n", errorMsg[code-1]);
            continue;
        }
        turn = turn == 1 ? 0 : 1;
        paintBoard(turn, board, pieces, tiles, pieceIcons);
        if(code == -2){
            gameOver = 1;
            printf("*Checkmate*\n%s wins\n", players[1-turn]);   
        }else if(code == -1){
            printf("%s is in check\n", players[turn]);
        }
    }
    
    //TODO detect stalemate?
}

/* Determine, based on the type and color of piece on the square 
   and background color of square, how to paint each square.
   Also rotate the board to the perspective of the parameterized color */
void paintBoard(int color, short board[8][8], int pieces[8][8][2], char *(tiles[]), char *(pieceIcons[7][2])){
    
    int i, j;
    /* Print the header */
    printf("\e[36m    \u2554");
    for(i = 0; i < 12; i++)
        printf("\u2550");
    printf("\u2557\n    \u2551 *NIX Chess \u2551\n    \u255a");  
    for(i = 0; i < 12; i++)
        printf("\u2550");
    printf("\u255d\e[0m\n\n");
    
    if(color){
        /* Print from white's perspective */
        for(i = 0; i < 8; i++){
            printf(" %d ", 8 - i);
            for(j = 0; j < 8; j++){
                printf("\e[2m%s%s\e[0m", tiles[board[i][j]], 
                    pieceIcons[pieces[i][j][0]][pieces[i][j][1]]);
            }
            printf("\n");
        }
        printf("   A/B/C/D/E/F/G/H/\n");
    }else{
        /* Print from black's perspective */
        for(i = 7; i >= 0; i--){
            printf(" %d ", 8 - i);
            for(j = 7; j >= 0; j--){
                printf("\e[2m%s%s\e[0m", tiles[board[i][j]], 
                    pieceIcons[pieces[i][j][0]][pieces[i][j][1]]);
            }
            printf("\n");
        }
        printf("   H/G/F/E/D/C/B/A/\n");
    }
}

char toUpper(char c){
    if(c > 90)
        c -= 32;
    if(c < 65 || c > 90)
        return 0;
    return c;
}

int selfCheck(int pieces[8][8][2], int i, int j, int x, int y, short color){
    // make a copy of the pieces data so we don't fuck with the original
    int temp[8][8][2], a, b, c;
    for(a = 0; a < 8; a++)
        for(b = 0; b < 8; b++)   
            for(c = 0; c < 2; c++)
                temp[a][b][c] = pieces[a][b][c];
                
            
    // move the piece 
    temp[x][y][0] = temp[i][j][0];
    temp[x][y][1] = temp[i][j][1];
    temp[i][j][0] = 0;
    temp[i][j][1] = 0;
    
    
    
    // see if it puts the king in check 
    // first get all possible moves for all opposing pieces 
    int validMoves[64][2];
    for(a = 0; a < 8; a++){
        for(b = 0; b < 8; b++){
            if(temp[a][b][0] != 0 && temp[a][b][1] != color){
                printf("%d %d",temp[a][b][0],temp[a][b][1]);
                /*int moves[32][2];
                getValidMoves(moves, temp, color, a, b);
                c = 0;
                while(moves[c][0] != -1){
                    addToValidMoves(validMoves, moves[c][0], moves[c][1]);
                    c++;
                }
                */
                printf("\n");
            }
        }
    }
    return 0;
    /*
    c = 0;
    
    //locate player's king
    int found = 0;
    for(a = 0; a < 8; a++){
        for(b = 0; b < 8; b++){
            if(temp[a][b][0] == 6 && temp[a][b][1] == color){
                found = 1;
                break;
            }
        }
        if(found)
            break;
    }
    printf("3");
    // see if any of the opposing piece's possible moves are the
    // player's king's location
    while(validMoves[c][0] != -1){
        if(validMoves[c][0] == a && validMoves[c][1] == b)
            return 1; //king in check
        c++;
    }
    
    printf("4");
    return 0; //king not in check
    */
}

/*
 * Finds all valid spots to move for a given piece
 * Stores locati8ons in validMoves, an array of integer pairs
 */
void getValidMoves(int validMoves[][2], int pieces[8][8][2], short color, int i, int j){

    int x, y;
    
    if(pieces[i][j][0] == 0){ // empty space
        validMoves[0][0] = -1;
    } else if(pieces[i][j][0] == 1){ // pawn
        // pawn logic
        if(color){ // white pawn
            // white pawn is not on left edge or row 8
            // and can take piece to front left             
            if(j > 0 && i > 0 && pieces[i-1][j-1][1] != color 
                     && pieces[i-1][j-1][0] != 0
                     && !selfCheck(pieces,i,j,i-1,j-1,color)){
                addToValidMoves(validMoves, i-1, j-1);
            }
            // white pawn is not on right edge or row 8
            // and can take piece to front right
            if(j < 7 && i > 0 && pieces[i-1][j+1][1] != color 
                     && pieces[i-1][j+1][0] != 0
                     && !selfCheck(pieces,i,j,i-1,j+1,color)){
                addToValidMoves(validMoves, i-1, j+1);
            }
            // white pawn is not on row 8
            // and has empty spot in front of it
            if(i > 0 && pieces[i-1][j][0] == 0
                    && !selfCheck(pieces,i,j,i-1,j,color)){
                addToValidMoves(validMoves, i-1, j);
            }
            // white pawn is on home row
            // and has no pieces 1 or 2 tiles in front of it
            if(i == 6 && pieces[i-1][j][0] == 0 && pieces[i-2][j][0] == 0
                    && !selfCheck(pieces,i,j,i-2,j,color)){
                addToValidMoves(validMoves, i-2, j);
            }
        }else{ //black pawn
            // black pawn is not on left edge or row 1
            // and can take piece to front left             
            if(j > 0 && i < 7 && pieces[i+1][j-1][1] != color 
                     && pieces[i+1][j-1][0] != 0
                     && !selfCheck(pieces,i,j,i+1,j-1,color)){
                addToValidMoves(validMoves, i+1, j-1);
            }
            // black pawn is not on right edge or row 1
            // and can take piece to front right
            if(j < 7 && i < 7 && pieces[i+1][j+1][1] != color 
                     && pieces[i+1][j+1][0] != 0
                     && !selfCheck(pieces,i,j,i+1,j+1,color)){
                addToValidMoves(validMoves, i+1, j+1);
            }
            // black pawn is not on row 1
            // and has empty spot in front of it
            if(i < 7 && pieces[i+1][j][0] == 0
                    && !selfCheck(pieces,i,j,i+1,j,color)){
                addToValidMoves(validMoves, i+1, j);
            }
            // black pawn is on home row
            // and has no pieces 1 or 2 tiles in front of it
            if(i == 1 && pieces[i+1][j][0] == 0 && pieces[i+2][j][0] == 0
                    && !selfCheck(pieces,i,j,i+2,j,color)){
                addToValidMoves(validMoves, i+2, j);
            }
        } 
    } else if(pieces[i][j][0] == 2){ // bishop
        // bishop logic
        int xInc, yInc;
        for(xInc = -1; xInc <= 1; xInc += 2)
            for(yInc = -1; yInc <= 1; yInc += 2){
                x = i + xInc;
                y = j + yInc;
                while(x <= 7 && x >= 0 && y <= 7 && y >= 0
                             && pieces[x][y][0] == 0){
                    if(!selfCheck(pieces,i,j,x,y,color))    
                        addToValidMoves(validMoves, x, y);
                    x += xInc;
                    y += yInc;
                }
                if(x <= 7 && x >= 0 && y <= 7 && y >= 0
                             && pieces[x][y][1] != color
                             && !selfCheck(pieces,i,j,x,y,color)){
                    addToValidMoves(validMoves, x, y);
                }
            }    
    } else if(pieces[i][j][0] == 3){ // knight
        // knight logic
        int xInc, yInc;
        for(xInc = -2; xInc <= 2; xInc++)
            for(yInc = -2; yInc <= 2; yInc++){
                
                if(xInc == 0 || yInc == 0 || xInc == yInc || (-1*xInc) == yInc){
                    continue;
                }    
                
                x = i + xInc;
                y = j + yInc;
                
                if(x <= 7 && x >= 0 && y <= 7 && y >= 0
                             && (pieces[x][y][0] == 0
                             || pieces[x][y][1] != color)
                             && !selfCheck(pieces,i,j,x,y,color)){
                    addToValidMoves(validMoves, x, y);
                }
            }
    } else if(pieces[i][j][0] == 4){ // rook
        // rook logic
        int xInc, yInc;
        for(xInc = -1; xInc <= 1; xInc += 1)
            for(yInc = -1; yInc <= 1; yInc += 1){
                if(!(xInc == 0 || yInc == 0) || (xInc == 0 && yInc == 0))
                    continue;
                x = i + xInc;
                y = j + yInc;
                while(x <= 7 && x >= 0 && y <= 7 && y >= 0
                             && pieces[x][y][0] == 0){
                    if(!selfCheck(pieces,i,j,x,y,color))
                        addToValidMoves(validMoves, x, y);
                    x += xInc;
                    y += yInc;
                }
                if(x <= 7 && x >= 0 && y <= 7 && y >= 0
                             && pieces[x][y][1] != color
                             && !selfCheck(pieces,i,j,x,y,color)){
                    addToValidMoves(validMoves, x, y);
                }
            }
    } else if(pieces[i][j][0] == 5){ // queen
        // queen logic
        int xInc, yInc;
        for(xInc = -1; xInc <= 1; xInc += 1)
            for(yInc = -1; yInc <= 1; yInc += 1){
                if(xInc == 0 && yInc == 0)
                    continue;
                x = i + xInc;
                y = j + yInc;
                while(x <= 7 && x >= 0 && y <= 7 && y >= 0
                             && pieces[x][y][0] == 0){
                    if(!selfCheck(pieces,i,j,x,y,color))
                        addToValidMoves(validMoves, x, y);
                    x += xInc;
                    y += yInc;
                }
                if(x <= 7 && x >= 0 && y <= 7 && y >= 0
                             && pieces[x][y][1] != color
                             && !selfCheck(pieces,i,j,x,y,color)){
                    addToValidMoves(validMoves, x, y);
                }
            }
    } else if(pieces[i][j][0] == 6){ // king
        // king logic
        for(x = i - 1; x <= i + 1; x++){
            for(y = j - 1; y <= j + 1; y++){
                if(x < 0 || y < 0 || x > 7 || y > 7)
                    continue;
                if((pieces[x][y][0] == 0 || pieces[x][y][1] != color)
                        && !selfCheck(pieces,i,j,x,y,color)){
                    addToValidMoves(validMoves, x, y);
                }
            }
        }
    }
}

int inValidMoves(int validMoves[][2], int i, int j){
    int k = 0;
    while(validMoves[k][0] != -1){
        if(validMoves[k][0] == i && validMoves[k][1] == j){
            return 1;
        }
        k++;
    }
    return 0;
}

void addToValidMoves(int validMoves[][2], int i, int j){
    //check to see if it's already there
    int k = 0;
    while(validMoves[k][0] != -1){
        if(validMoves[k][0] == i && validMoves[k][1] == j){
            return;
        }
        k++;
    }
    // ASSERT: location not already in list, so add it
    validMoves[k][0] = i;
    validMoves[k][1] = j;
    validMoves[++k][0] = -1;
}


/* detects check or checkmate for the last move
 * return 0 for neither
 * return -1 for check
 * return -2 for checkmate
 */
int checkForCheckmate(int validMoves[][2], int pieces[8][8][2], int x, int y, short color){

    // find other player's king
    int i, j, found;
    found = 0;
    for(i = 0; i < 8; i++){
        for(j = 0; j < 8; j++){
            if(pieces[i][j][0] == 6 && pieces[i][j][1] != color){
                found = 1;
                break;
            }
        }
        if(found)
            break;
    }
    // ASSERT (i,j) is location of opponent's king
    
    if(!inValidMoves(validMoves, i, j)) // opponent not in check
        return 0;
    
    // ASSERT opponenet is in check
    //TODO check for checkmate
    
    //TODO opponent can block?
    // attacking piece is rook, bishop, or queen and
    // for any tile in between the attacking piece and the king,
    // an opponent piece can move there
    //TODO opponent can capture?
    // one of opponent's piece's valid moves contains attacker's location
    //TODO opponent can move? 
    // opponent's king's valid moves are all contained in player's
    // valid move set across all their pieces
    
     
     
    // ASSERT opponent in check, but not checkmate 
    return -1;

}

/* returns positive integer if error occurs;
 * returns -2 upon checkmate, -1 for check, else 0.
 * Error codes:
 * 1: No piece at start location
 * 2: Piece not owned by player
 * 3: End location not in piece's range
 * 4: Invalid start location
 * 5: Invalid end location
 */
int move(char moveCmd[], int pieces[8][8][2], short color){
    /* parse out initial location */
    int j = toUpper(moveCmd[0]);
    if(j == 0){
        return 4;
    }
    j -= 65;
    int i = atoi(&moveCmd[1]);
    if(i == 0){
        return 4;
    }    
    i = 8 - i;
    if(i < 0 || j < 0 || i > 7 || j > 7){
        return 4;
    }
    
    /* parse out future location */
    int y = toUpper(moveCmd[2]);
    if(y == 0)
        return 5;
    y -= 65;
    int x = atoi(&moveCmd[3]);
    if(x == 0)
        return 5;
    x = 8 - x;
    if(x < 0 || y < 0 || x > 7 || y > 7)
        return 5;
    
    /* check that a piece exists at location */
    if(pieces[i][j][0] == 0)
        return 1;
        
    /* check that piece belongs to the player */
    if(pieces[i][j][1]  != color)
        return 2;
    
    /* get valid moves, store them in buffer */
    int validMoves[64][2];
    validMoves[0][0] = -1;
    getValidMoves(validMoves, pieces, color, i, j);
    
    /* Debugging: print out all possible spaces that the piece could have gone */
    int k = 0; 
    while(validMoves[k][0] != -1){
        printf("%c%d ", validMoves[k][1] + 65, 8 - validMoves[k][0]);
        k++;
    }
    printf("\n");
    
    if(inValidMoves(validMoves, x, y)){
        /* actually move the piece in memory */
        pieces[x][y][0] = pieces[i][j][0];
        pieces[x][y][1] = pieces[i][j][1];
        pieces[i][j][0] = 0;
        pieces[i][j][1] = 0;
        getValidMoves(validMoves, pieces, color, x, y);
        return checkForCheckmate(validMoves, pieces, color, i, j);
    }
    
    return 3;
    //TODO detect putting yourself in check (in getValidMoves)
    //TODO detect checkmate
    
    //TODO pawn replacement on back row
    //TODO castling (keep a flag for if either color has already castled..)
    //TODO en passant?
}
