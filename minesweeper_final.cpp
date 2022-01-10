#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <chrono>

using namespace std;

#define MAX_SIDE 26


int SIDE, MINES;
int computerBoard[MAX_SIDE][MAX_SIDE], playerBoard[MAX_SIDE][MAX_SIDE]; // computerBoard refers to the board that is randomly created with each game, playerBoard refers to the board visible to the user.
int gameover, flag_counter;
bool quit;
bool lose_game;


void count(int x, int y){ // recursive function to count the number of zeros on the board. cells with a zero do not have any mines next to them

    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
            if((x+i) >=0 && (x+i) <=(SIDE - 1) && (y+j) >=0 && (y+j)<=(SIDE - 1) && playerBoard[x+i][y+j] == (SIDE + 1)){ // nested for-loops to check every orthogonal and diagonal cell next to cell at coordinate (x,y). (SIDE+1) refers to a cell that has not been opened or flagged. This reference is created in the initialize function
                if(computerBoard[x+i][y+j] == 0){
                    playerBoard[x+i][y+j] = 0;
                    gameover++; // counts the number of revealed cells on the board. number of revealed cells cannot exceed (SIDE*SIDE - MINES)
                    count(x+i, y+j); // recursive function. if a cell is shown to have no mines next to it, the function is called again for a cell (given by the values of i and j at the time) next to it

                }
                else{
                    playerBoard[x+i][y+j] = computerBoard[x+i][y+j]; // if there is a non-zero value located on that cell, the playerBoard at that cell assigns itself the value that is present on the computerBoard on the same cell
                    gameover++; // number of revealed cells goes up
                }
            }
        }
    }
}


void printboard(){ // function to print the playerBoard that is shown to the user

    cout << "   ";
    cout << endl;
    cout << "   ";

    for(int i = 0; i < SIDE; i++){
        if(i > 9){
            cout << "  " << i/10; // as i is an integer, the values will only increment in whole numbers, starting from zero. for the case of the game, the values shown are only 0 and 1, but for larger board sizes, the shown value will increase
        }
        else{
            cout << "  " << "0"; // print zeros if the numbers are equal to or less than 9
        }
    }

    cout << endl;
    cout << "   ";

    for(int i = 0; i < SIDE; i++){

        cout << "  " << i % 10; // i%10 will give the remainder once you divide i by 10. the remainders will match up with the loop above this to produce the correct order of numbers, with each number taking two lines (meant to be read from top to bottom)
    }

    cout << endl;
    cout << "     ";

    for(int i = 0; i < SIDE; i++){ // to print the barrier between the displayed column numbers and the playerBoard
        cout << "_  ";
    }
    cout << endl;

    for(int i = 0; i < SIDE; i++){ // to print the barrier between the displayed row numbers and the playerBoard
        if(i >= 10){
            cout << i << "|  ";
        }
        else{
            cout << "0" << i << "|  ";
        }

        for(int j = 0; j < SIDE; j++){
            if(playerBoard[i][j] == (SIDE + 1)){ // (SIDE + 1) initialized to represent an unopened cell. any number outside the range of [0,SIDE] would be appropriate (including negative numbers) as a representative number
                cout << ".  ";
            }
            else if(playerBoard[i][j] == (SIDE + 2)){ //(SIDE + 2) initialized to represent a flagged cell.
                cout << "F  ";
            }
            else{
                cout << playerBoard[i][j] << "  ";
            }
        }
        cout << endl;
    }
    cout << endl;
}


void initialize(){ // initialize the boards

    srand(time(NULL));
    int lower_bound = 0;
    int upper_bound = SIDE-1;
    int random_x, random_y;
    int mine_count = 0;

    for(int i = 0; i < SIDE; i++){
        for(int j = 0; j < SIDE; j++){
            computerBoard[i][j] = -2; // computerBoard initialized to be completely filled with -2. this is important because we have set the number -1 to represent a mine.
            playerBoard[i][j] = SIDE + 1; // playerBoard initialized to be completely filled with (SIDE + 1). thus the board is filled with unrevealed cells
        }
    }

    gameover = 0;
    flag_counter = 0;
    quit = false;
    lose_game = false;


    for(int i = 0; i <= MINES; i++){ // loop to randomly place mines in the computerBoard
        random_x = rand() % (upper_bound + 1);
        random_y = rand() % (upper_bound + 1);

        computerBoard[random_x][random_y] = -1;
    }

    for(int i = 0; i < SIDE; i++){
        for(int j = 0; j < SIDE; j++){
            if(computerBoard[i][j] == -2){
                mine_count = 0;
                for(int k = -1; k <= 1; k++){
                    for(int l = -1; l <= 1; l++){
                        if((i+k) >= 0 && (j+l) >= 0 && (i+k) <= (SIDE-1) && (j+l) <= (SIDE-1) && (k != 0 || l != 0)){ // to ensure the cells are within bounds of the board. k or l not equaling to zero is a check for this, as i and j can have negative values (leading them off the board)
                            if(computerBoard[i+k][j+l] == -1){
                                mine_count++; // mine_count is the number of mines in contact with a cell
                            }
                        }
                    }
                }
                computerBoard[i][j] = mine_count; // this initializes the computerBoard with the number of mines next to each given cell that will be looked at by the playerBoard after it makes a move
            }
        }
    }
}



void chooseDifficulty(){ // function for the user to choose the difficulty (board size and number of mines) they want to play with

    int diff;

    cout << "Choose Difficulty Level (1/2/3): " << endl;
    cout << "1. Beginner (9x9 board with 10 mines)" << endl;
    cout << "2. Medium (16x16 board with 40 mines)" << endl;
    cout << "3. Hard (26x26 board with 115 mines)" << endl;

    while(true){
        if(cin >> diff && diff >=1 && diff <= 3){ // check to see if the inputted value is an integer (and can be stored into diff) and for it to be in bounds
            break;
        }
        else{
            cout << "Please enter an integer value (1-3) for your preferred difficulty level." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
        }
    } // input validation function used from 'https://www.delftstack.com/howto/cpp/cpp-input-validation/'

    if(diff == 1){
        SIDE = 9;
        MINES = 10;
    }
    if(diff == 2){
        SIDE = 16;
        MINES = 40;
    }
    if(diff == 3){
        SIDE = 26;
        MINES = 115;
    }

}

void header(){ // printing the header of the game

    cout << "MINESWEEPER IN C++" << endl << endl;
    cout << "HOW TO INPUT" << endl << endl;
    cout << "DECISION: Input 'r' if you would like to reveal a cell (Be careful! It could be a mine!), 'f' if you would like to flag a cell, 'u' if you would like to unflag a cell, or 'q' if you would like to quit the game you are playing." << endl;
    cout << "ROW: Input the x-coordinate of the cell you would like to interact with." << endl;
    cout << "COLUMN: Input the y-coordinate of the cell you would like to interact with." << endl;
    cout << "'F' is a flagged position." << endl << endl << endl;
    cout << "The number on a cell refers to how many mines are touching it, both orthogonally and diagonally." << endl;
    cout << endl << endl;
}

void reveal_board(){ // function to reveal the entire board when the game is over (either by winning, losing, or quitting)

    cout << "SOLVED BOARD: " << endl;
    cout << "   ";
    cout << "\33[2J\33[1;1H"; // escape statements to clear the board. found from: 'https://stackoverflow.com/questions/1348563/clearing-output-of-a-terminal-program-linux-c-c'
    cout << endl;
    cout << "   ";

// same formatting statements used in the printboard function

    for(int i = 0; i < SIDE; i++){
        if(i > 9){
            cout << "  " << i/10;
        }
        else{
            cout << "  " << "0";
        }
    }

    cout << endl;
    cout << "   ";

    for(int i = 0; i < SIDE; i++){
        cout << "  " << i % 10;
    }

    cout << endl;
    cout << "     ";

    for(int i = 0; i < SIDE; i++){
        cout << "_  ";
    }

    cout << endl;

    for(int i = 0; i < SIDE; i++){
        if(i >= 10){
            cout << i << "|  ";
        }
        else{
            cout << "0" << i << "|  ";
        }

            for(int j = 0; j < SIDE; j++){ // printing the values from the computerBoard

                if(computerBoard[i][j] == -1){
                    cout << "X  ";
                }
                else{
                    cout << computerBoard[i][j] << "  ";
                }
        }
        cout << endl;
    }
    cout << endl;
}

void play(){ // function actually play the game

    int x, y;
    char decision, playagain;

    playagain = 'y';

    while(playagain == 'y'){ // while loop to play the game. value of playagain changes if the user decides to stop playing
        chooseDifficulty();
        initialize();

        while(gameover != (SIDE*SIDE)){ // number of revealed cells cannot equal all the cells on the board (the entire board must be revealed for this to occur)
            cout << "\33[2J\33[1;1H";
            cout << "\nCurrent Board: " << endl;

            printboard();
            cout << "Flags available: " << (MINES - flag_counter) << endl; // number of flags user can still place on the board
            cout << "Input Decision (r/f/u/q): " << endl;

            while(true){
                if(cin >> decision && (decision == 'r' || decision == 'f' || decision == 'u' || decision == 'q')){
                    break; // break out of loop if input is correct
                }
                else{
                    cout << "Enter a valid decision (r/f/u/q): " << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(),'\n');
                }
            } // input validation function used from 'https://www.delftstack.com/howto/cpp/cpp-input-validation/'

            if(decision == 'q'){ // break out of the loop (will not take in row and column coordinates) if user decided to quit game
                cout << "You quit the game." << endl;
                quit = true;
                break;
            }

            cout << "ROW [0," << (SIDE-1) << "]: ";
            cin >> x;
            cout << "COLUMN [0," << (SIDE - 1) << "]: ";
            cin >> y;

            if(x < 0 || x > (SIDE-1) || y < 0 || y > (SIDE-1)){ // do nothing if the entered value is out of bounds
                continue;
            }
            if(playerBoard[x][y] != (SIDE+1) && playerBoard[x][y] != (SIDE+2)){ // do nothing if the entered value is out of bounds
                continue;
            }

            if(decision == 'f'){
                if(playerBoard[x][y] != (SIDE+1)){ // do nothing if the cell has a flag already or has been revealed
                    continue;
                }
                else if (flag_counter <= MINES){
                    playerBoard[x][y] = (SIDE+2); // place flag at this position
                    gameover++; // flagged cell contributes to total cells interacted with
                    flag_counter++;
                }
                else{ // do nothing if no flags are remaining but the game is not finished
                    continue;
                }
            }
            else if(decision == 'r'){
                if(playerBoard[x][y] != (SIDE+1)){ // do nothing if the cell has been revealed or has a flag placed on it
                    continue;
                }
                else if (computerBoard[x][y] == 0){ // if value is a zero at (x,y), go to the count function and count/reveal other zero value cells
                    playerBoard[x][y] = 0;
                    gameover++;
                    count(x,y);
                }
                else if (computerBoard[x][y] == -1){ // user has chosen a cell with a mine. the game is over
                    lose_game = true;
                    break;
                }
                else{
                    playerBoard[x][y] = computerBoard[x][y]; // assign the number of mines next to the chosen coordinate by looking at the computerBoard at the same coordinate
                    gameover++;
                }
            }
            else if(decision == 'u'){
                if(playerBoard[x][y] != (SIDE+2)){ // do nothing if there is no flag on chosen cell
                    continue;
                }
                else{
                    playerBoard[x][y] = (SIDE+1); // change the cell from flagged to unrevealed
                    gameover--; // reduce number of cells interacted with
                }
            }
        }

        cout << "GAMEOVER" << endl;

        reveal_board(); // show the board when game is over

        if(gameover == (SIDE*SIDE)){ // if all cells are revealed and mines are flagged, gameover is at its maximum value and the game is won
            cout << "You win." << endl;
        }
        else if(quit != true || lose_game == true){ // if the user did not quit and lose the game
            cout << "You lose." << endl;
        }

        cout << endl;

        do{ // check if the user wants to play again
            cout << endl;
            cout << "Do you wish to play again? (y/n)" << endl;
            cin >> playagain;

        }
        while(playagain != 'y' && playagain != 'n');
    }
}

int main(){

    header(); // print header
    play(); // play game

    return 0;

}
