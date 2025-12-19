#include<bits/stdc++.h>
using namespace std;
class Board{
    vector<vector<char>>board=vector<vector<char>>(3,vector<char>(3,' '));
    public:
    // Display the board
    void display() {
        cout << "\n";
        for(int i=0;i<3;i++){
            cout << " ";
            for(int j=0;j<3;j++){
                cout << board[i][j];
                if(j<2) cout << " | ";
            }
            cout << "\n";
            if(i<2) cout << "---+---+---\n";
        }
        cout << "\n";
    }
    //Make a move on the board
    bool makeMove(int row,int col,char player){
        if(row<0 or row>=3 or col<0 or col>=3 or board[row][col]!=' '){
            return false;
        }
        board[row][col]=player;
        return true;
    }
    //unmake a move on the board
    void unMakeMove(int row,int col){
        if(row>=0 and row<3 and col>=0 and col<3){
            board[row][col]=' ';
        }
    }
    //check if the board is full
    bool isFull(){
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                if(board[i][j]==' '){
                    return false;
                }
            }
        }
        return true;
    }
    //Check if the player has won or not
    bool checkWin(char player){
        //check rows and columns
        for(int i=0;i<3;i++){
            if(board[i][0]==player and board[i][1]==player and board[i][2]==player){
                return true;
            }
            if(board[0][i]==player and board[1][i]==player and board[2][i]==player){
                return true;
            }
        }
        //check diagonals
        if(board[0][0]==player and board[1][1]==player and board[2][2]==player){
            return true;
        }
        if(board[0][2]==player and board[1][1]==player and board[2][0]==player){
            return true;
        }
        return false;
    }
    //Get available moves
    vector<pair<int,int>>getavailableMoves(){
        vector<pair<int,int>>moves;
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                if(board[i][j]==' '){
                    moves.push_back({i,j});
                }
            }
        }
        return moves;
    }
    //Reset the board
    void reset(){
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                board[i][j]=' ';
            }
        }
    }
    //get the cell value at a position
    char getCell(int row,int col){
        if(row>=0 and row<3 and col>=0 and col<3){
            return board[row][col];
        }
        return ' ';
    }
};

class player{
    protected:
    char symbol; //'X' or 'O'
    public:
    player(char sym){
        symbol=sym;
    }
    virtual void makeMove(Board &board)=0; //pure virtual function
    char getSymbol(){
        return symbol;
    }
    //virtual destructor
    virtual ~player(){}
};

class HumanPlayer:public player{
    public:
    HumanPlayer(char sym):player(sym){}
    void makeMove(Board &board) override{
        int row,col;
        while(true){
            cout<<"Enter your move (row and column): ";
            cin>>row>>col;
            //if the move is valid, make the move and break
            if(board.makeMove(row,col,getSymbol())){
                break;
            }
            else{
                cout<<"Invalid move. Try again.\n";
            }
        }
    }
};

class AIPlayer:public player{
    public:
    AIPlayer(char sym):player(sym){}
    //Evaluate the board state
    int evaluate(Board & board){
        if(board.checkWin(getSymbol())){
            return +10;
        }
        char opponentSymbol=(getSymbol()=='X')?'O':'X';
        if(board.checkWin(opponentSymbol)){
            return -10;
        }
        return 0;
    }
    //Minimax algorithm
    int minimax(Board &board,int depth,bool isMax){
        //isMax indicates if the current move is for maximizer or minimizer
        //depth is used to prefer faster wins and slower losses
        int score=evaluate(board); //evaluate the board
        if(score==10){
            return score-depth;//prefer faster wins
        }
        if(score==-10){
            return score+depth;//prefer slower losses
        }
        if(board.isFull()){
            return 0; //draw
        }
        if(isMax){
            int best=INT_MIN; //initialize best to a very low value
            vector<pair<int,int>>moves=board.getavailableMoves();
            for(auto move:moves){
                board.makeMove(move.first,move.second,getSymbol());
                best=max(best,minimax(board,depth+1,false));
                board.unMakeMove(move.first,move.second);
            }
            return best;
        }
        else{
            int best=INT_MAX; //initialize best to a very high value
            char opponentSymbol=(getSymbol()=='X')?'O':'X';
            vector<pair<int,int>>moves=board.getavailableMoves();
            for(auto move:moves){
                board.makeMove(move.first,move.second,opponentSymbol);
                best=min(best,minimax(board,depth+1,true));
                board.unMakeMove(move.first,move.second);
            }
            return best;
        }
    }
    //Find the best move for AI
    pair<int,int> findBestMove(Board &board){
        int bestVal=INT_MIN;
        pair<int,int> bestMove={-1,-1};
        vector<pair<int,int>>moves=board.getavailableMoves();
        for(auto move:moves){
            board.makeMove(move.first,move.second,getSymbol());
            int moveVal=minimax(board,0,false);
            board.unMakeMove(move.first,move.second);
            if(moveVal>bestVal){
                bestMove=move;
                bestVal=moveVal;
            }
        }
        return bestMove;
    }
    //Make the move for AI
    void makeMove(Board &board) override{
        pair<int,int>bestMove=findBestMove(board);
        board.makeMove(bestMove.first,bestMove.second,getSymbol());
        cout<<"AI plays at ("<<bestMove.first<<", "<<bestMove.second<<")\n";
    }
    //virtual destructor
    virtual ~AIPlayer(){}
};
//class to manage the game
class Game{
    Board board;
    player *player1;
    player *player2;
    public:
    Game(player *p1,player *p2){
        player1=p1;
        player2=p2;
    }
    void play(){
        board.reset();
        player *currentPlayer=player1;
        while(true){
            board.display();
            currentPlayer->makeMove(board);
            if(board.checkWin(currentPlayer->getSymbol())){
                board.display();
                cout<<"Player "<<currentPlayer->getSymbol()<<" wins!\n";
                break;
            }
            if(board.isFull()){
                board.display();
                cout<<"It's a draw!\n";
                break;
            }
            //switch players
            if(currentPlayer==player1){
                currentPlayer=player2;
            }
            else{
                currentPlayer=player1;
            }
        }
    }
};

int main(){
    cout<<"Tic Tac Toe Game"<<endl;
    cout<<"Choose game mode:\n1. Human vs Human\n2. Human vs AI\n";
    int choice;
    cin>>choice;
    player *p1;
    player *p2;
    if(choice==1){
        p1=new HumanPlayer('X');
        p2=new HumanPlayer('O');
    }
    else{
        p1=new HumanPlayer('X');
        p2=new AIPlayer('O');
    }
    Game game(p1,p2);
    game.play();
    delete p1;
    delete p2;
}
