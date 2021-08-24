#include "MiniMax.h"
using namespace std;

const int INF = 1 << 15;
const int Score[8][8] = {
                        100,-80,40 ,10 ,10 ,40 ,-80,100,
						-80,-40,-10,5  ,5  ,-10,-40,-80,
						40 ,-10,3  ,2  ,2  ,3  ,-10,40 ,
						10 ,5  ,2  ,-1 ,-1 ,2  ,5  ,10 ,
						10 ,5  ,2  ,-1 ,-1 ,2  ,5  ,10 ,
						40 ,-10,3  ,2  ,2  ,3  ,-10,40 ,
						-80,-40,-10,5  ,5  ,-10,-40,-80,
						100,-80,40 ,10 ,10 ,40 ,-80,100};


const int dx[8] = { -1,-1,0,1,1,1,0,-1 };
const int dy[8] = { 0,1,1,1,0,-1,-1,-1 };

bool MiniMax::check(int x, int y)//Check whether the location exceed the board.
{
	return 0 <= x && x < 8 && 0 <= y && y < 8;
}

int MiniMax::Evaluation(bool is_black)//Evaluate the score of current board with the color we given.
{
	int score = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (Board[i][j] == -1)continue;
			if (Board[i][j] != 0)
			{
				if (is_black + Board[i][j] == 2)
				{
					score += Score[i][j];
				}
			}
		}
	}
	return score;
}

void MiniMax::Makestep(vector<int> position, bool is_black)//Simulate out step
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

void MiniMax::print_board(vector<vector<int>> board)
{
	for (int i = 0; i < board.size(); i++)
	{
		for (int j = 0; j < board[0].size(); j++)
			cout << board[i][j] << " ";
		cout << endl;
	}
	return;
}

vector<int> MiniMax::miniMax(int maxstep, bool is_black)//Run minmax.
{
	int value;
	int best = -INF;
	vector<int> best_step = { -1,-1 };
	vector<vector<int>> init = Board;
	vector<vector<int>> Can_flip = GetFlipPosition(is_black);

	for (int i = 0; i < Can_flip.size(); i++)
	{
		Makestep(Can_flip[i], is_black);

		value = Min(maxstep - 1, !is_black, best, INF);
		if (value > best)
		{
			best = value;
			best_step = Can_flip[i];
		}
		Board = init;
	}
	return best_step;
}

int MiniMax::Max(int step, bool is_black, int alpha, int beta)
{
	if (step <= 0) return Evaluation(is_black);
	int value = -INF;
	vector<vector<int>> init = Board;
	vector<vector<int>> Can_flip = GetFlipPosition(is_black);
	if (Can_flip.empty()) return Evaluation(is_black);

	for (int i = 0; i < Can_flip.size(); i++)
	{
		Makestep(Can_flip[i], is_black);

		value = max(value, Min(step - 1, !is_black, alpha, beta));
		if (value >= beta) return value;
		alpha = max(alpha, value);
		Board = init;
	}

	return value;
}

int MiniMax::Min(int step, bool is_black, int alpha, int beta)
{
	if (step <= 0) return Evaluation(is_black);
	int value = INF;
	vector<vector<int>> init = Board;
	vector<vector<int>> Can_flip = GetFlipPosition(is_black);
	if (Can_flip.empty()) return Evaluation(is_black);

	for (int i = 0; i < Can_flip.size(); i++)
	{
		Makestep(Can_flip[i], is_black);

		value = min(value, Max(step - 1, !is_black, alpha, beta));
		if (value <= alpha) return value;
		beta = min(beta, value);
		Board = init;
	}
	return value;
}

vector<vector<int>> MiniMax::GetFlipPosition(bool is_black)
{
	vector<vector<int>> FlipPosition;
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			if (Board[i][j])continue;
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


