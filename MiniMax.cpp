#include <time.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <random>
#include <memory>
#include <functional>
using namespace std;

const int INF = 1 << 15;
const int Score[8][8] = {
                        100,-80,40 ,10 ,10 ,40 ,-80,100,
						-80,-40,10 ,5  ,5  ,10 ,-40,-80,
						40 ,10 ,3  ,2  ,2  ,3  ,10 ,40 ,
						10 ,5  ,2  ,2  ,2  ,2  ,5  ,10 ,
						10 ,5  ,2  ,2  ,2  ,2  ,5  ,10 ,
						40 ,10 ,3  ,2  ,2  ,3  ,10 ,40 ,
						-80,-40,10 ,5  ,5  ,10 ,-40,-80,
						100,-80,40 ,10 ,10 ,40 ,-80,100};
/*
const int Score[8][8] = { 0,100,-5 ,10 ,10 ,-5 ,100,0  ,
						100,-45,-5 ,1  ,1  ,-5 ,-45,100,
						-5 ,-5 ,3  ,2  ,2  ,3  ,-5 ,-5 ,
						10 ,1  ,2  ,2  ,2  ,2  ,1  ,10 ,
						10 ,1  ,2  ,2  ,2  ,2  ,1  ,10 ,
						-5 ,-5 ,3  ,2  ,2  ,3  ,-5 ,-5 ,
						100,-45,-5 ,1  ,1  ,-5 ,-45,100,
						0  ,100,-5 ,10 ,10 ,-5 ,100,0 };
*/

const int dx[8] = { -1,-1,0,1,1,1,0,-1 };
const int dy[8] = { 0,1,1,1,0,-1,-1,-1 };

class MiniMax
{
public:
	MiniMax(vector<vector<int>> board) :Board(board) {}

	bool check(int, int);
	void print_board(vector<vector<int>>);
	void Makestep(vector<int>, bool);
	int Evaluation(bool);
	vector<int> miniMax(int maxstep, bool is_black);
	int Min(int step, bool is_black, int alpha, int beta);
	int Max(int step, bool is_black, int alpha, int beta);
	vector<vector<int>> GetFlipPosition(bool);

	vector<vector<int>> Board;
};

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

		value = max(value, Min(step - 1, is_black, alpha, beta));
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

vector<int> GetStep(vector<vector<int>>& board, bool is_black)
{
	MiniMax MiniMax_solver(board);
	vector<int> step = MiniMax_solver.miniMax(5, is_black);

	if (step[0] == -1 && step[1] == -1)//If we can't flip any chess by put somewhere, then out next step is the empty location with -maximum score.
	{
		default_random_engine generator = default_random_engine(time(NULL));
		uniform_int_distribution<int> distribution_int(1, 6);
		auto rand_int = bind(distribution_int, generator);

		int max_score = -INF;
		int x = 0;
		int y = 0;
		for (int i = 1; i < 7; i++)
			for (int j = 1; j < 7; j++)
				if (!board[i][j] && Score[i][j] > max_score)
				{
					max_score = Score[i][j];
					x = i; y = j;
				}
		if (!x && !y)
		{
			cout << "RAND\n";
			x = rand_int();
			y = rand_int();
			while (board[x][y])
			{
				x = rand_int();
				y = rand_int();
			}
		}
		step = { x,y };
	}
	return step;
}

void step(vector<vector<int>>& Board,int x,int y, bool is_black)//Simulate out step
{
    vector<int> position={x,y};
	Board[position[0]][position[1]] = (is_black) ? 1 : 2;
	for (int i = 0; i < 8; i++)
	{
		int x = position[0] + dx[i];
		int y = position[1] + dy[i];
		if (is_black)
		{
			while (0<=x&&x<8&&0<=y&&y<8&& Board[x][y] == 2)
			{
				x += dx[i];
				y += dy[i];
			}
			if (0<=x&&x<8&&0<=y&&y<8&& Board[x][y] == 1)
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
			while (0<=x&&x<8&&0<=y&&y<8&& Board[x][y] == 1)
			{
				x += dx[i];
				y += dy[i];
			}
			if (0<=x&&x<8&&0<=y&&y<8&& Board[x][y] == 2)
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
int main()
{
    vector<vector<int>> bd(8);
    //Initial board 0 is unoccupied,1 is black and 2 is white.
    bd[0]={0,0,0,0,0,0,0,0};
    bd[1]={0,0,0,0,0,0,0,0};
    bd[2]={0,0,0,0,0,0,0,0};
    bd[3]={0,0,0,2,1,0,0,0};
    bd[4]={0,0,0,1,2,0,0,0};
    bd[5]={0,0,0,0,0,0,0,0};
    bd[6]={0,0,0,0,0,0,0,0};
    bd[7]={0,0,0,0,0,0,0,0};
    //
    bool ib=1;//chess color :1 is black chess player and 0 is white chess player
    //GetStep(boardstate,chess_color) will return a optimal step by my searching.
    //step(x,y,chess_color) can simulate a step
    while(1)
    {
        int x,y;

        vector<int> s=GetStep(bd,1);
        step(bd,s[0],s[1],ib);
        cout<<"STEP: "<<s[0]<<" "<<s[1]<<endl;
        for(int i=0;i<8;i++)
        {
            for(int j=0;j<8;j++)
                cout<<bd[i][j]<<" ";
            cout<<endl;
        }
        cout<<"------------\n";
        cout<<"CIN: ";
        cin>>x>>y;//cin step,
        step(bd,x,y,!ib);
        for(int i=0;i<8;i++)
        {
            for(int j=0;j<8;j++)
                cout<<bd[i][j]<<" ";
            cout<<endl;
        }
        cout<<"------------\n";
    }

}
