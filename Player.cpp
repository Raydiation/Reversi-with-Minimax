#include "MCTS.h"
#include "MiniMax.h"
#include <windows.h>
#include <stdlib.h>
using namespace std;

vector<int> GetStep(vector<vector<int>>& board, bool is_black)
{
    MCTS_UCT MCTS_solver(board, is_black);
    MCTS_solver.Round(15); //Adjust how many times do a MCTS round
    return MCTS_solver.Return_Best();
    /*
    MiniMax MiniMax_solver(board);
    vector<int> step = MiniMax_solver.miniMax(5, is_black); // Adjust how many layers minimax algorithm will search
    return step;
    */
}

int main()
{
    vector<vector<int>> play_board(8);
    play_board[0] = { 0,0,0,0,0,0,0,0 };
    play_board[1] = { 0,0,0,0,0,0,0,0 };
    play_board[2] = { 0,0,0,0,0,0,0,0 };
    play_board[3] = { 0,0,0,2,1,0,0,0 };
    play_board[4] = { 0,0,0,1,2,0,0,0 };
    play_board[5] = { 0,0,0,0,0,0,0,0 };
    play_board[6] = { 0,0,0,0,0,0,0,0 };
    play_board[7] = { 0,0,0,0,0,0,0,0 };
    MiniMax Manager(play_board);
    //
    bool is_black = 0;//1 for first player, 0 for last player
    while (1)
    {
        vector<vector<int>>Valid_Position = Manager.GetFlipPosition(!is_black);//AI tern
        if (Valid_Position.size())
        {
            vector<int> ai_step = GetStep(Manager.Board, !is_black);
            Manager.Makestep(vector<int>{ai_step[0], ai_step[1]}, !is_black);
            cout << "\n\nAI move-----------------\n\n";
        }

        Valid_Position = Manager.GetFlipPosition(is_black);//Player tern
        if (Valid_Position.size())
        {
            int x, y;
            while (1)
            {
                Manager.print_board(Manager.Board);
                cout << "--------------------\n";
                cout << "Enter a position : ";
                cin >> x >> y;
                bool valid = 0;
                for (vector<int> valid_pos : Valid_Position)
                {
                    if (x == valid_pos[0] && y == valid_pos[1])
                    {
                        valid = 1;
                        break;
                    }
                }
                if (valid)
                {
                    Manager.Makestep(vector<int>{x, y}, is_black);
                    system("CLS");
                    Manager.print_board(Manager.Board);
                    Sleep(500);
                    break;
                }
                else
                {
                    cout << "Invalid step\n";
                    Sleep(300);
                    system("CLS");
                }
            }
        }
        else
        {
            cout << "No valid step\n";
            Sleep(500);
            system("CLS");
        }
    }
}
