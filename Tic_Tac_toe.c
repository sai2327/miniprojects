#include<stdio.h>

// Global variables
char board[3][3];             // 3x3 Tic-Tac-Toe board
char currentPlayer='X';       // Current player: 'X' or 'O'

// Function to initialize the board with empty spaces
void initializeBoard() {
    int i,j;
    for(i=0;i<3;i++){
        for(j=0;j<3;j++){
            board[i][j] = ' ';
        }
    }
}

// Function to print the current state of the board
void printBoard() {
    int i;
    printf("\n    0   1   2\n"); // Column headers
    for (i=0;i<3;i++){
        printf("  -------------\n");
        printf("%d | %c | %c | %c |\n",i,board[i][0],board[i][1],board[i][2]); // Each row
    }
    printf("  -------------\n\n");
}

// Function to check if the current player has won
int checkWin(){
    int i;
    for(i=0;i<3;i++){
        // Check rows and columns
        if (board[i][0]==currentPlayer && board[i][1]==currentPlayer && board[i][2]==currentPlayer) return 1;
        if (board[0][i]==currentPlayer && board[1][i]==currentPlayer && board[2][i]==currentPlayer) return 1;
    }
    // Check diagonals
    if(board[0][0] == currentPlayer && board[1][1] == currentPlayer && board[2][2] == currentPlayer) return 1;
    if(board[0][2] == currentPlayer && board[1][1] == currentPlayer && board[2][0] == currentPlayer) return 1;
    return 0; // No win found
}

// Function to check if the game is a draw
int isDraw() {
    int i,j;
    for(i=0;i<3;i++){
        for(j=0;j<3;j++){
            if(board[i][j]==' ') return 0; // If any cell is empty, not a draw
        }
    }
    return 1; // All cells filled and no winner => draw
}

// Function to switch the player
void switchPlayer() {
    currentPlayer=(currentPlayer=='X')?'O':'X';
}

// Main game function
int main(){
    initializeBoard(); // Start with an empty board
    int row, col;

    while (1) {
        printBoard(); // Show current board

        // Prompt current player for their move
        printf("Player %c, enter your move (row and column: 0 1 2): ",currentPlayer);
        scanf("%d%d",&row,&col);

        // Input validation: check for valid row and column
        if(row<0 || row>2 || col<0 || col>2){
            printf("Invalid input. Try again.\n");
            continue;
        }

        // Check if the cell is already occupied
        if(board[row][col]!=' '){
            printf("Cell already taken. Try another.\n");
            continue;
        }

        // Place the current player's mark
        board[row][col]=currentPlayer;

        // Check if current player won
        if(checkWin()){
            printBoard();
            printf("Player %c wins!\n", currentPlayer);
            break;
        }
        // Check for a draw
        else if(isDraw()){
            printBoard();
            printf("It's a draw!\n");
            break;
        }

        // Switch to the other player
        switchPlayer();
    }

    return 0; // End of game
}
