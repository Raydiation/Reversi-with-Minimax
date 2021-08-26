#ifndef __MCTS__
#define __MCTS__

#include <time.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <random>
#include <memory>
#include <functional>
using namespace std;

bool check(int, int);
void print_board(vector<vector<int>>);
void Simulate_a_step(vector<vector<int>>& Board, vector<int>, bool);
vector<vector<int>> GetFlipPosition(vector<vector<int>>& Board, vector<vector<int>>, bool);

class Node
{
public:
    Node(vector<vector<int>>board, bool color)
    {
        board_state = board;
        Can_flip = GetFlipPosition(board_state, vector<vector<int>>(), color);
        is_black = color;
        total_simulation_time = 0;
        win_simulation_time = 0;
        fatherNode = nullptr;
    }
    Node* fatherNode;
    vector<Node*> children;
    vector<vector<int>> board_state;
    vector<vector<int>> Can_flip;
    vector<int> this_step;
    int total_simulation_time;
    int win_simulation_time;
    bool is_black;//what color next step play
};
class MCTS_UCT
{
public:
    MCTS_UCT(vector<vector<int>> board, bool is_black) :Root(new Node(board, is_black)) {}
    Node* Root;
    default_random_engine generator = default_random_engine(time(NULL));

    void Round(int);
    void Selection(Node*);
    void Expansion(Node*);
    void Simulation(Node*);
    void Backpropagation(Node*, bool);
    Node* UCB(Node*, double);
    bool Playout(vector<vector<int>>, bool);
    vector<int> Return_Best();
};


#endif // __MCTS__
