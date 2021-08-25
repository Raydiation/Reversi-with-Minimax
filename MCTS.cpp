#include <time.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <random>
#include <memory>
#include <functional>
#include "MCTS.h"
using namespace std;

const int dx[8] = { -1,-1,0,1,1,1,0,-1 };
const int dy[8] = { 0,1,1,1,0,-1,-1,-1 };

void MCTS_UCT::Round(int max_round)
{
    int count = 0;
    while (count++ < max_round)
        Selection(Root);
}

vector<int> MCTS_UCT::Return_Best()
{
    vector<int> best_step = Root->Can_flip[0];
    double win_rate = 0;
    for (int i = 0; i < Root->children.size(); i++)
    {
        Node *child = Root->children[i];
        if (10 * child->total_simulation_time >= Root->total_simulation_time && double(child->win_simulation_time) / double(child->total_simulation_time) > win_rate)
        {
            best_step = Root->Can_flip[i];
            win_rate = double(child->win_simulation_time) / double(child->total_simulation_time);
        }
    }
    return best_step;
}

void MCTS_UCT::Selection(Node *current_node)
{
    if (current_node->children.size() == 0)
    {
        Expansion(current_node);
    }
    else
    {
        //random pick?
        uniform_int_distribution<int> distribution_int(0, current_node->children.size() - 1);
        Selection(current_node->children[distribution_int(generator)]);//random move to a child
    }
    return;
}
void MCTS_UCT::Expansion(Node *current_node)
{
    vector<vector<int>> init = current_node->board_state;
    for (int i = 0; i < current_node->Can_flip.size(); i++)//decide how many child node will be generate
    {
        Simulate_a_step(current_node->board_state, current_node->Can_flip[i], current_node->is_black);
        Node *child_node = new Node(current_node->board_state, !(current_node->is_black));
        current_node->board_state = init;
        current_node->children.push_back(child_node);
        child_node->fatherNode = current_node;
        Simulation(child_node);
    }
}
void MCTS_UCT::Simulation(Node *current_node)
{
    bool result = Playout(current_node->board_state, current_node->is_black);
    Backpropagation(current_node, result);
}
void MCTS_UCT::Backpropagation(Node *current_node, bool result)
{
    current_node->total_simulation_time += 1;
    current_node->win_simulation_time += result;
    if (current_node != Root)
        Backpropagation(current_node->fatherNode, result);
}
bool MCTS_UCT::Playout(vector<vector<int>> board, bool is_black)
{
    bool current_player = is_black;
    bool both_no_flip = 0;
    vector<vector<int>> unoccupied;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (board[i][j] == 0)
                unoccupied.push_back({ i,j });
    while (unoccupied.size())
    {
        vector<vector<int>> Can_flip = GetFlipPosition(board, unoccupied, current_player);
        if (!Can_flip.size())//If both are no position to put?
        {
            if (both_no_flip)break;
            current_player = !current_player;
            both_no_flip = 1;
            continue;
        }
        both_no_flip = 0;
        uniform_int_distribution<int> distribution(0, Can_flip.size() - 1);
        int rand_pick = distribution(generator);
        for (int i = 0; i < unoccupied.size(); i++)
            if (Can_flip[rand_pick] == unoccupied[i])
            {
                unoccupied.erase(unoccupied.begin() + i);
                break;
            }
        Simulate_a_step(board, Can_flip[rand_pick], current_player);
        current_player = !current_player;
    }
    int result = 0;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            if (board[i][j])
            {
                if (board[i][j] + is_black == 2) result++;
                else result--;
            }
        }
    return (result > 0) ? 1 : 0;
}
bool check(int x, int y)//Check whether the location exceed the board.
{
    return 0 <= x && x < 8 && 0 <= y && y < 8;
}

void Simulate_a_step(vector<vector<int>>& Board, vector<int> position, bool is_black)//Simulate out step
{
    Board[position[0]][position[1]] = (is_black) ? 1 : 2;
    for (int i = 0; i < 8; i++)
    {
        int x = position[0] + dx[i];
        int y = position[1] + dy[i];
        if (is_black)
        {
            while (check(x, y) && Board[x][y] == 2)
            {
                x += dx[i];
                y += dy[i];
            }
            if (check(x, y) && Board[x][y] == 1)
            {
                x -= dx[i];
                y -= dy[i];
                while (Board[x][y] == 2)
                {
                    Board[x][y] = 1;
                    x -= dx[i];
                    y -= dy[i];
                }
            }
        }
        else
        {
            while (check(x, y) && Board[x][y] == 1)
            {
                x += dx[i];
                y += dy[i];
            }
            if (check(x, y) && Board[x][y] == 2)
            {
                x -= dx[i];
                y -= dy[i];
                while (Board[x][y] == 1)
                {
                    Board[x][y] = 2;
                    x -= dx[i];
                    y -= dy[i];
                }
            }
        }
    }
}

void print_board(vector<vector<int>> board)
{
    for (int i = 0; i < board.size(); i++)
    {
        for (int j = 0; j < board[0].size(); j++)
        {
            if (board[i][j] == 0) cout << "- ";
            if (board[i][j] == 1) cout << "O ";
            if (board[i][j] == 2) cout << "X ";
        }
        cout << endl;
    }
    return;
}
vector<vector<int>> GetFlipPosition(vector<vector<int>>& Board, vector<vector<int>> unoccupied_position, bool is_black)
{
    if (unoccupied_position.empty())
    {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                if (!Board[i][j])
                    unoccupied_position.push_back({ i,j });
    }
    vector<vector<int>> FlipPosition;
    for (vector<int> position : unoccupied_position)
    {
        int i = position[0];
        int j = position[1];
        for (int k = 0; k < 8; k++)
        {
            int ii = i + dx[k], jj = j + dy[k];
            if (check(ii, jj))
            {
                if (Board[ii][jj] == 2 && is_black)
                {
                    while (check(ii, jj) && Board[ii][jj] == 2)
                    {
                        ii += dx[k];
                        jj += dy[k];
                    }
                    if (check(ii, jj) && Board[ii][jj] == 1)
                    {
                        vector<int> temp = { i,j };
                        FlipPosition.push_back(temp);
                        break;
                    }
                }
                else if (Board[ii][jj] == 1 && !is_black)
                {
                    while (check(ii, jj) && Board[ii][jj] == 1)
                    {
                        ii += dx[k];
                        jj += dy[k];
                    }
                    if (check(ii, jj) && Board[ii][jj] == 2)
                    {
                        vector<int> temp = { i,j };
                        FlipPosition.push_back(temp);
                        break;
                    }
                }
            }
        }
    }
    return FlipPosition;
}
