#include <iostream>
#include <string>
#include <random>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>

using namespace std;

#define ROW_COUNT 9
#define COL_COUNT 9
int game_board[ROW_COUNT][COL_COUNT] = { 0 };
int row_valid[9];
int col_valid[9];
int grid_valid[9];

void printGameBoard(int n)
{
	//cout << "Succesfully generated the " << n + 1 << " game!" << endl;
	cout << "***************************" << endl;
	for (int i = 0; i < ROW_COUNT; i++)
	{
		for (int j = 0; j < COL_COUNT; j++)
		{
			cout << game_board[i][j] << ' ';
		}
		cout << endl;
	}
	cout << "***************************" << endl;
	cout << endl;
}

void writeFile(string& filename)
{
	ofstream file(filename);
	file.open(filename, ios::app);
	if (file.is_open()) {
		for (int i = 0; i < ROW_COUNT; i++) {
			for (int j = 0; j < COL_COUNT; j++) {
				file << game_board[i][j] << " ";
			}
			file << endl;
		}
		file << endl << endl;
		file.close();
	}
	else
		cout << "无法打开文件:" << filename << endl;
}

void readFile(string& filename)
{
	// TODO
}

void initValid()
{
	for (int i = 0; i < 9; i++)
	{
		row_valid[i] = 511;
		col_valid[i] = 511;
		grid_valid[i] = 511;
	}
}

void updateValid(int x, int y, int value)
{
	//cout << "511 ^ (1 << value) " << (511 ^ (1 << value) << endl;
	row_valid[x] ^= (1 << (value - 1));
	col_valid[y] ^= (1 << (value - 1));
	int row_grid = x / 3;
	int col_grid = y / 3;
	int my_grid = row_grid * 3 + col_grid;
	grid_valid[my_grid] ^= (1 << (value - 1));
}

void restoreValid(int x, int y, int value)
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

bool fillBoard(int x, int y)
{
	if (x == 9)
	{
		return true;
	}
	if (y == 9)
	{
		return fillBoard(x + 1, 0);
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
		updateValid(x, y, valid_value_list[i]);
		if (fillBoard(x, y + 1))
		{
			return true;
		}
		else
		{
			restoreValid(x, y, valid_value_list[i]);
		}
	}
	return false;
}

bool generateSudo()
{
	initValid();
	return fillBoard(0, 0);
}

int generateSudoKu(int count)
{
	if (count < 1 || count>10000)
	{
		cout << "Error: Invalid parameter for game_count!" << endl;
		return -1;
	}
	for (int i = 0; i < count; i++)
	{
		cout << endl << "Begin generating the " << i + 1 << " game..." << endl;
		while (!generateSudo());
		printGameBoard(i);
	}
	return 0;
}

void digHoles(int hole_cnt)
{
	uniform_int_distribution<int> dist(0, 80);
	for (int i = 0; i < hole_cnt; i++)
	{
		int ran = dist(rng);
		//cout << ran << endl;
		
		while (game_board[ran / 9][ran % 9] == 0) // 生成了相同的随机数
		{
			ran = dist(rng);
		}
		game_board[ran / 9][ran % 9] = 0;
	}
}

int generateSudoGame(int game_count, int mode=0, int subarg=0) 
{
	for (int i = 0; i < game_count; i++) 
	{
		cout << endl << "Begin generating the " << i + 1 << " game..." << endl;
		while (!generateSudo());
		switch (mode)
		{
		case 0: // -n gamecount
		{
			uniform_int_distribution<int> dist(20, 55);
			int ran_hole_cnt = dist(rng);
			cout << ran_hole_cnt << endl;
			digHoles(ran_hole_cnt);
			break;
		}
		case 1: // -n -m "difficulty"mode
		{
			if (subarg == 1)
			{
				digHoles(20);
			}
			else if (subarg == 2)
			{
				digHoles(35);
			}
			else if (subarg == 3)
			{
				digHoles(55);
			}
			else
			{
				cout << "Error: Invalid parameter for difficulty!" << endl;
				return -1;
			}
			break;
		}
		case 2: // -n -r "holes"
		{
			if (subarg < 20 || subarg > 55)
			{
				cout << "Error: Invalid parameter for hole_count!" << endl;
				return -1;
			}
			digHoles(subarg);
			break;
		}
		case 3: // -n -u
		{

			break;
		}
		default:
			break;
		}

		printGameBoard(i);
	}
	return 0;
}

int main(int argc, char* argv[]) 
{
	//generateSudoGame(10);
	//printGameBoard(0);
	generateSudoGame(5, 0);
	
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
			return generateSudoKu(count);
		}
		else
		{
			cout << "Error: Invalid parameter for '-c'!" << endl;
			return -1;
		}
	}
	else if (arg1 == "-s") 
	{
		if (argc == 3) // 解文件中的数独
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
				return generateSudoGame(game_count, 3);
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
				return generateSudoGame(game_count, 1, difficulty);
			}
			else if (arg2 == "-r")
			{
				int hole_count = stoi(subarg2);
				return generateSudoGame(game_count, 2, hole_count);
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
	}
	
	return 0;
}