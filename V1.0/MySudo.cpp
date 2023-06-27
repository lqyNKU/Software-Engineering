#include <iostream>
#include <string>
#include <random>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cmath>

using namespace std;

#define ROW_COUNT 9
#define COL_COUNT 9
int game_board[ROW_COUNT][COL_COUNT] = { 0 };
int row_valid[9];
int col_valid[9];
int grid_valid[9];

void init_valid()
{
	for (int i = 0; i < 9; i++)
	{
		row_valid[i] = 511;
		col_valid[i] = 511;
		grid_valid[i] = 511;
	}
}

void update_valid(int x, int y, int value)
{
	//cout << "511 ^ (1 << value) " << (511 ^ (1 << value) << endl;
	row_valid[x] ^= (1 << (value - 1));
	col_valid[y] ^= (1 << (value - 1));
	int row_grid = x / 3;
	int col_grid = y / 3;
	int my_grid = row_grid * 3 + col_grid;
	grid_valid[my_grid] ^= (1 << (value - 1));
}

void restore_valid(int x, int y, int value)
{
	row_valid[x] |= (1 << (value - 1));
	col_valid[y] |= (1 << (value - 1));
	int row_grid = x / 3;
	int col_grid = y / 3;
	int my_grid = row_grid * 3 + col_grid;
	grid_valid[my_grid] |= (1 << (value - 1));
}

random_device rd;
mt19937 rng(rd());

int total = 0;
int need;

bool fill_board(int x, int y)
{
	if (x == 9)
	{
		//return true;
		//printGameBoard();
		//total++;
		//cout << "Successfully generated the " << total << " game!" << endl;
		//cout << "Begin generating the " << total+1 << " game..." << endl;
		//return (total == need ? 1 : 0);
		return true;
	}
	if (y == 9)
	{
		return fill_board(x + 1, 0);
	}
	/* acquire valid values */
	int row_grid = x / 3;
	int col_grid = y / 3;
	int k = row_grid * 3 + col_grid;
	int valid = row_valid[x] & col_valid[y] & grid_valid[k];
	//cout << "valid: " << valid << endl;
	int tmp = 1;
	vector<int> valid_value_list;
	while (tmp < 10)
	{
		//cout << "pow: " << pow(2, tmp-1) << endl;
		//cout << (valid & (1 << tmp - 1)) << endl;
		if ((valid & (1 << tmp-1)) != 0)
		{
			//cout << "tmp: " << tmp << endl;
			valid_value_list.push_back(tmp);
		}
		tmp++;
	}
	/*for (int i = 0; i < valid_value_list.size(); i++)
	{
		cout << valid_value_list[i] << ' ';
	}*/
	shuffle(valid_value_list.begin(), valid_value_list.end(), rng);
	//cout << "size:" << valid_value_list.size() << endl;
	for (int i = 0; i < valid_value_list.size(); i++)
	{
		game_board[x][y] = valid_value_list[i];
		update_valid(x, y, valid_value_list[i]);
		if (fill_board(x, y + 1))
		{
			return true;
		}
		else
		{
			restore_valid(x, y, valid_value_list[i]);
		}
	}
	return false;
}

void printGameBoard()
{
	for (int i = 0; i < ROW_COUNT; i++)
	{
		for (int j = 0; j < COL_COUNT; j++)
		{
			cout << game_board[i][j] << ' ';
		}
		cout << endl;
	}
}

bool generateSudo(int count = 1)
{
	init_valid();
	return fill_board(0, 0);
}

bool generateSudoGame(int game_count, int mode=0, int subarg=0) 
{
	//cout << "Begin generating the first game..." << endl;
	
	for (int i = 0; i < game_count; i++) 
	{
		cout << "Begin generating the " << i + 1 << " game..." << endl;
		while (!generateSudo());
		/*int fail_time = 0;

		while (!generateSudo() && fail_time<10)
		{
			fail_time++;
		}
		if (fail_time == 10)
		{
			return false;
		}*/
		cout << "Successfully generated the " << i + 1 << " game!" << endl;
	}
	return true;
}

int main(int argc, char* argv[]) 
{
	generateSudoGame(1000);
	printGameBoard();
	/*
	
	if (argc < 2) 
	{
		cout << "Error: Invalid Input!" << endl;
		return -1;
	}	
	//srand((unsigned)time(NULL));
	string arg1 = argv[1];
	if (arg1 == "-c") 
	{
		if (argc == 3) 
		{
			string subarg1 = argv[2];
			int count = stoi(subarg1);
			generateSudo(count);
		}
		else
		{
			cout << "Error: Invalid parameter for '-c'!" << endl;
			return -1;
		}
	}
	else if (arg1 == "-s") 
	{
		if (argc == 3) 
		{
			
		}
		else
		{
			cout << "Error: Invalid parameter for '-s'!" << endl;
			return -1;
		}
	}
	else if (arg1 == "-n") 
	{
		string subarg1 = argv[2];
		int game_count = stoi(subarg1);
		// cout << game_count << endl;
		switch (argc)
		{
		case 3: // 只要求生成游戏数量 
		{
			generateSudoGame(game_count);
			break;
		}
		case 4: // 唯一解 -u
		{
			string arg2 = argv[3];
			bool flag = true;
			if (arg2 == "-u") 
			{
				generateSudoGame(game_count, 3);
			}
			else
			{
				cout << "Error: Invalid input!" << endl;
				return -1;
			}
			break;
		}

		case 5: // -m/-r
		{
			string arg2 = argv[3];
			string subarg2 = argv[4];
			if (arg2 == "-m") 
			{
				int difficulty = stoi(subarg2);
				generateSudoGame(game_count, 1, difficulty);
			}
			else if (arg2 == "-r")
			{
				int hole_count = stoi(subarg2);
				generateSudoGame(game_count, 2, hole_count);
			}
			else
			{
				cout << "Error: Invalid input!" << endl;
				return -1;
			}
			break;
		}
		default:
			cout << "Error: Invalid parameter for '-n'!" << endl;
			return -1;
		}
	}*/
	
	return 0;
}