#ifndef __MINIMAX__
#define __MINIMAX__

#include <time.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <random>
#include <memory>
#include <functional>

class MiniMax
{
public:
	MiniMax(std::vector<std::vector<int>> board) :Board(board) {}

	bool check(int, int);
	void print_board(std::vector<std::vector<int>>);
	void Makestep(std::vector<int>, bool);
	int Evaluation(bool);
	std::vector<int> miniMax(int maxstep, bool is_black);
	int Min(int step, bool is_black, int alpha, int beta);
	int Max(int step, bool is_black, int alpha, int beta);
	std::vector<std::vector<int>> GetFlipPosition(bool);

	std::vector<std::vector<int>> Board;
};

#endif // __MINIMAX__
